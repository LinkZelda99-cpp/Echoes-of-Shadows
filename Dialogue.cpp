#include "Dialogue.h"

#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#include <vector>
#include <cctype>

#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#else
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#endif

// ============================================================
// TERMINAL WIDTH
// ============================================================

int getTerminalWidth() {
#ifdef _WIN32

    CONSOLE_SCREEN_BUFFER_INFO csbi{};

    if (GetConsoleScreenBufferInfo(
            GetStdHandle(STD_OUTPUT_HANDLE),
            &csbi
        )) {
        return csbi.srWindow.Right - csbi.srWindow.Left + 1;
    }

#else

    struct winsize w{};

    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0 && w.ws_col > 0) {
        return static_cast<int>(w.ws_col);
    }

#endif

    // Safe fallback if the terminal width cannot be detected.
    return 80;
}

// ============================================================
// INPUT
// ============================================================

bool enterPressed() {

#ifdef _WIN32

    if (_kbhit()) {
        int key = _getch();

        if (key == '\r' || key == '\n') {
            return true;
        }
    }

#else

    fd_set set;
    struct timeval timeout{};

    FD_ZERO(&set);
    FD_SET(STDIN_FILENO, &set);

    if (select(
            STDIN_FILENO + 1,
            &set,
            nullptr,
            nullptr,
            &timeout
        ) > 0) {

        char key{};

        if (read(STDIN_FILENO, &key, 1) > 0) {
            if (key == '\n' || key == '\r') {
                return true;
            }
        }
    }

#endif

    return false;
}

// ============================================================
// ANSI / VISIBLE LENGTH HELPERS
// ============================================================

// Returns the visible length of a string while ignoring ANSI
// escape sequences such as:
//
// \033[3m
// \033[0m
//
// This allows italic/color formatting without affecting wrapping.

int visibleLength(const std::string& text) {

    int length = 0;
    bool inEscape = false;

    for (char c : text) {

        if (c == '\033') {
            inEscape = true;
            continue;
        }

        if (inEscape) {
            if (c == 'm') {
                inEscape = false;
            }

            continue;
        }

        ++length;
    }

    return length;
}

// ============================================================
// WORD WRAPPING
// ============================================================

std::string wrapText(const std::string& text, int width) {

    if (width < 1) {
        return text;
    }

    std::string result;

    std::string currentLine;
    int currentLength = 0;

    auto flushLine = [&]() {

        result += currentLine;

        currentLine.clear();
        currentLength = 0;
    };

    // --------------------------------------------------------
    // Add a word to the current line.
    // --------------------------------------------------------

    auto addWord = [&](const std::string& word) {

        if (word.empty()) {
            return;
        }

        const int wordLength = visibleLength(word);

        // ----------------------------------------------------
        // First word on the line.
        // ----------------------------------------------------

        if (currentLine.empty()) {

            // If the word itself fits, simply add it.
            if (wordLength <= width) {

                currentLine = word;
                currentLength = wordLength;
                return;
            }

            // ------------------------------------------------
            // Long word.
            //
            // Break it so the terminal never has to perform
            // its own wrapping.
            // ------------------------------------------------

            int partLength = 0;
            bool inEscape = false;

            for (size_t i = 0; i < word.size(); ++i) {

                char c = word[i];

                // ANSI escape sequence
                if (c == '\033') {

                    currentLine += c;
                    inEscape = true;
                    continue;
                }

                if (inEscape) {

                    currentLine += c;

                    if (c == 'm') {
                        inEscape = false;
                    }

                    continue;
                }

                currentLine += c;
                ++partLength;

                if (partLength >= width && i + 1 < word.size()) {

                    result += currentLine;
                    result += '\n';

                    currentLine.clear();
                    partLength = 0;
                }
            }

            currentLength = partLength;
            return;
        }

        // ----------------------------------------------------
        // Word fits on the current line.
        // ----------------------------------------------------

        if (currentLength + 1 + wordLength <= width) {

            currentLine += ' ';
            currentLine += word;

            currentLength += 1 + wordLength;

            return;
        }

        // ----------------------------------------------------
        // Word does not fit.
        //
        // Finish the current line and put the word on the
        // next line.
        // ----------------------------------------------------

        flushLine();
        result += '\n';

        // If the word fits by itself, start a new line.
        if (wordLength <= width) {

            currentLine = word;
            currentLength = wordLength;
            return;
        }

        // Otherwise break the long word.
        int partLength = 0;
        bool inEscape = false;

        for (size_t i = 0; i < word.size(); ++i) {

            char c = word[i];

            if (c == '\033') {

                currentLine += c;
                inEscape = true;
                continue;
            }

            if (inEscape) {

                currentLine += c;

                if (c == 'm') {
                    inEscape = false;
                }

                continue;
            }

            currentLine += c;
            ++partLength;

            if (partLength >= width && i + 1 < word.size()) {

                result += currentLine;
                result += '\n';

                currentLine.clear();
                partLength = 0;
            }
        }

        currentLength = partLength;
    };

    // ========================================================
    // PROCESS TEXT
    // ========================================================

    std::string word;

    for (size_t i = 0; i < text.size(); ++i) {

        char c = text[i];

        // ----------------------------------------------------
        // Explicit newline.
        // ----------------------------------------------------

        if (c == '\n') {

            addWord(word);
            word.clear();

            flushLine();
            result += '\n';

            continue;
        }

        // ----------------------------------------------------
        // Other whitespace.
        //
        // Treat it as a normal word separator.
        // ----------------------------------------------------

        if (c == ' ' || c == '\t' || c == '\r') {

            addWord(word);
            word.clear();

            continue;
        }

        word += c;
    }

    // Add the final word.
    addWord(word);

    // Add anything remaining on the final line.
    if (!currentLine.empty()) {
        flushLine();
    }

    return result;
}

// ============================================================
// INSTANT TEXT
// ============================================================

void printText(const std::string& text) {

    int terminalWidth = getTerminalWidth();

    // Leave one character of breathing room so we don't depend
    // on the terminal's automatic edge wrapping.
    int wrapWidth = terminalWidth - 2;

    if (wrapWidth < 20) {
        wrapWidth = 20;
    }

    std::cout << wrapText(text, wrapWidth) << std::flush;
}

// ============================================================
// TYPEWRITER
// ============================================================

bool typeText(const std::string& text, int speed) {

    int terminalWidth = getTerminalWidth();

    int wrapWidth = terminalWidth - 2;

    if (wrapWidth < 20) {
        wrapWidth = 20;
    }

    // IMPORTANT:
    //
    // Wrapping happens BEFORE the typewriter starts.
    // Therefore normal typing AND pressing Enter use the
    // exact same layout.
    //
    const std::string wrappedText =
        wrapText(text, wrapWidth);

    for (size_t i = 0; i < wrappedText.length(); ++i) {

        // ----------------------------------------------------
        // ENTER = FINISH TYPING
        // ----------------------------------------------------

        if (enterPressed()) {

            // Print the remainder of the ALREADY-WRAPPED text.
            //
            // This does NOT re-wrap it and therefore cannot
            // change the layout.
            std::cout << wrappedText.substr(i) << std::flush;

            return true;
        }

        // ----------------------------------------------------
        // Normal typewriter output
        // ----------------------------------------------------

        std::cout << wrappedText[i] << std::flush;

        // Don't unnecessarily sleep after the final character.
        if (i + 1 < wrappedText.length()) {
            std::this_thread::sleep_for(
                std::chrono::milliseconds(speed)
            );
        }
    }

    return false;
}

// ============================================================
// WAIT FOR ENTER
// ============================================================

void waitForEnter() {

    const std::string prompt =
        "[Press Enter to continue]";

    std::cout << "\n\n"
              << prompt
              << std::flush;

    while (true) {

        if (enterPressed()) {

            // Erase the prompt.
            std::cout << '\r'
                      << std::string(prompt.length(), ' ')
                      << '\r'
                      << std::flush;

            return;
        }

        std::this_thread::sleep_for(
            std::chrono::milliseconds(10)
        );
    }
}

// ============================================================
// PAUSE
// ============================================================

void pauseFor(int milliseconds) {

    const auto start =
        std::chrono::steady_clock::now();

    while (true) {

        // Enter skips the pause.
        if (enterPressed()) {
            return;
        }

        const auto now =
            std::chrono::steady_clock::now();

        const auto elapsed =
            std::chrono::duration_cast<
                std::chrono::milliseconds
            >(now - start).count();

        if (elapsed >= milliseconds) {
            return;
        }

        std::this_thread::sleep_for(
            std::chrono::milliseconds(10)
        );
    }
}

// ============================================================
// DIALOGUE
// ============================================================

void dialogue(const std::string& text, int speed) {
    const bool skipped = typeText(text, speed);

    std::cout << '\n' << std::flush;

    if (!skipped) {
        waitForEnter();
    }
}