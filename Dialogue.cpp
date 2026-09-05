#include "Dialogue.h"

#include <iostream>
#include <thread>
#include <chrono>
#include <string>
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
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        return csbi.srWindow.Right - csbi.srWindow.Left + 1;
    }
#else
    struct winsize w;

    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0 && w.ws_col > 0) {
        return w.ws_col;
    }
#endif

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
    struct timeval timeout{};

    FD_ZERO(&set);
    FD_SET(STDIN_FILENO, &set);

    if (select(STDIN_FILENO + 1, &set, nullptr, nullptr, &timeout) > 0) {
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
    if (width < 1) {
        return text;
    }

    std::string result;
    std::string line;
    int visibleLength = 0;

    auto flushLine = [&]() {
        result += line;
        line.clear();
        visibleLength = 0;
    };

    auto appendWord = [&](const std::string& word) {
        if (word.empty()) {
            return;
        }

        // Keep ANSI escape sequences out of the visible-width count.
        int wordLength = 0;
        bool inEscape = false;

        for (char c : word) {
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

            ++wordLength;
        }

        if (line.empty()) {
            // A single long word should still be broken instead of relying
            // on the terminal's automatic wrapping.
            if (wordLength <= width) {
                line = word;
                visibleLength = wordLength;
                return;
            }

            int visiblePart = 0;
            for (size_t i = 0; i < word.size(); ++i) {
                char c = word[i];

                if (c == '\033') {
                    line += c;
                    ++i;
                    while (i < word.size()) {
                        line += word[i];
                        if (word[i] == 'm') {
                            break;
                        }
                        ++i;
                    }
                    continue;
                }

                line += c;
                ++visiblePart;

                if (visiblePart == width && i + 1 < word.size()) {
                    result += line;
                    result += '\n';
                    line.clear();
                    visiblePart = 0;
                }
            }

            visibleLength = visiblePart;
            return;
        }

        if (visibleLength + 1 + wordLength > width) {
            // A wrapped line must end with a newline. Without it, the next
            // word is concatenated directly onto the previous line.
            flushLine();
            result += '\n';
            line = word;
            visibleLength = wordLength;
            return;
        }

        line += ' ';
        line += word;
        visibleLength += 1 + wordLength;
    };

    std::string word;

    for (char c : text) {
        if (c == '\n') {
            appendWord(word);
            word.clear();
            flushLine();
            result += '\n';
        } else if (std::isspace(static_cast<unsigned char>(c))) {
            appendWord(word);
            word.clear();
        } else {
            word += c;
        }
    }

    appendWord(word);
    flushLine();

    return result;
}

// ============================================================
// INSTANT TEXT
// ============================================================

void printText(const std::string& text) {
    int terminalWidth = getTerminalWidth();
    int wrapWidth = terminalWidth - 1;

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
    int wrapWidth = terminalWidth - 1;

    if (wrapWidth < 20) {
        wrapWidth = 20;
    }

    const std::string wrappedText = wrapText(text, wrapWidth);

    for (size_t i = 0; i < wrappedText.length(); ++i) {
        if (enterPressed()) {
            std::cout << wrappedText.substr(i) << std::flush;
            return true;
        }

        std::cout << wrappedText[i] << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(speed));
    }

    return false;
}

// ============================================================
// WAIT FOR ENTER
// ============================================================

void waitForEnter() {
    const std::string prompt = "[Press Enter to continue]";

    std::cout << "\n\n" << prompt << std::flush;

    while (true) {
        if (enterPressed()) {
            std::cout << "\r"
                      << std::string(prompt.length(), ' ')
                      << "\r"
                      << std::flush;
            return;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

// ============================================================
// PAUSE
// ============================================================

void pauseFor(int milliseconds) {
    const auto start = std::chrono::steady_clock::now();

    while (true) {
        if (enterPressed()) {
            return;
        }

        const auto now = std::chrono::steady_clock::now();
        const auto elapsed =
            std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count();

        if (elapsed >= milliseconds) {
            return;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

// ============================================================
// DIALOGUE
// ============================================================

void dialogue(const std::string& text, int speed) {
    const bool skipped = typeText(text, speed);

    if (!skipped) {
        waitForEnter();
    }
}
