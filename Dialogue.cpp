#include "Dialogue.h"

#include <iostream>
#include <thread>
#include <chrono>
#include <string>

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

    CONSOLE_SCREEN_BUFFER_INFO csbi;

    if (GetConsoleScreenBufferInfo(
        GetStdHandle(STD_OUTPUT_HANDLE),
        &csbi)) {

        return csbi.srWindow.Right - csbi.srWindow.Left + 1;
    }

#else

    struct winsize w;

    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0) {

        if (w.ws_col > 0) {
            return w.ws_col;
        }
    }

#endif

    // Fallback if the terminal size can't be detected.
    return 80;
}


// ============================================================
// INPUT
// ============================================================

bool enterPressed() {

#ifdef _WIN32

    if (_kbhit()) {

        int key = _getch();

        if (key == '\r') {
            return true;
        }
    }

#else

    fd_set set;
    struct timeval timeout;

    FD_ZERO(&set);
    FD_SET(STDIN_FILENO, &set);

    timeout.tv_sec = 0;
    timeout.tv_usec = 0;

    if (select(
        STDIN_FILENO + 1,
        &set,
        nullptr,
        nullptr,
        &timeout
    ) > 0) {

        char key;

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
// WORD WRAPPING
// ============================================================

std::string wrapText(const std::string& text, int width) {

    std::string result;

    std::string line;
    std::string word;

    int visibleLength = 0;

    bool inEscapeCode = false;


    auto addWord = [&]() {

        if (word.empty()) {
            return;
        }

        int wordLength = 0;

        // Count only visible characters.
        for (size_t i = 0; i < word.length(); ++i) {

            if (word[i] == '\033') {
                inEscapeCode = true;
                continue;
            }

            if (inEscapeCode) {

                if (word[i] == 'm') {
                    inEscapeCode = false;
                }

                continue;
            }

            wordLength++;
        }


        // Need a new line?
        if (!line.empty() &&
            visibleLength + 1 + wordLength > width) {

            result += line;
            result += '\n';

            line.clear();
            visibleLength = 0;
        }


        if (!line.empty()) {

            line += ' ';
            visibleLength++;
        }

        line += word;
        visibleLength += wordLength;

        word.clear();
        };


    for (size_t i = 0; i < text.length(); ++i) {

        char c = text[i];


        // ----------------------------------------------------
        // EXPLICIT NEWLINE
        // ----------------------------------------------------

        if (c == '\n') {

            addWord();

            // IMPORTANT:
            // Preserve the newline exactly.
            result += line;
            result += '\n';

            line.clear();
            visibleLength = 0;

            continue;
        }


        // ----------------------------------------------------
        // SPACE
        // ----------------------------------------------------

        if (c == ' ') {

            addWord();

            continue;
        }


        // ----------------------------------------------------
        // CHARACTER
        // ----------------------------------------------------

        word += c;
    }


    // Add anything remaining.
    addWord();

    result += line;

    return result;
}


// ============================================================
// TYPEWRITER
// ============================================================

bool typeText(const std::string& text, int speed) {

    int terminalWidth = getTerminalWidth();

    // Leave one character of breathing room so we don't
    // accidentally hit the terminal's automatic wrapping.
    int wrapWidth = terminalWidth - 1;

    if (wrapWidth < 20) {
        wrapWidth = 20;
    }

    std::string wrappedText =
        wrapText(text, wrapWidth);


    for (size_t i = 0; i < wrappedText.length(); ++i) {

        if (enterPressed()) {

            std::cout
                << wrappedText.substr(i)
                << std::flush;

            return true;
        }


        std::cout
            << wrappedText[i]
            << std::flush;


        std::this_thread::sleep_for(
            std::chrono::milliseconds(speed)
        );
    }

    return false;
}


// ============================================================
// WAIT FOR ENTER
// ============================================================

void waitForEnter() {

    const std::string prompt =
        "[Press Enter to continue]";

    std::cout
        << "\n\n"
        << prompt
        << std::flush;


    while (true) {

        if (enterPressed()) {

            std::cout
                << "\r"
                << std::string(
                    prompt.length(),
                    ' '
                )
                << "\r"
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

    auto start =
        std::chrono::steady_clock::now();


    while (true) {

        if (enterPressed()) {
            return;
        }


        auto now =
            std::chrono::steady_clock::now();


        auto elapsed =
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

void dialogue(
    const std::string& text,
    int speed
) {

    bool skipped =
        typeText(text, speed);


    if (!skipped) {
        waitForEnter();
    }
}