#include "Dialogue.h"

#include <iostream>
#include <thread>
#include <chrono>
#include <conio.h>


// ============================================================
// INPUT
// ============================================================

bool enterPressed() {

    if (_kbhit()) {

        int key = _getch();

        if (key == '\r') {
            return true;
        }
    }

    return false;
}


// ============================================================
// TYPEWRITER
// ============================================================

bool typeText(const std::string& text, int speed) {

    for (size_t i = 0; i < text.length(); ++i) {

        if (enterPressed()) {

            // Finish the text immediately
            std::cout << text.substr(i) << std::flush;

            return true;
        }

        std::cout << text[i] << std::flush;

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

    const std::string prompt = "[Press Enter to continue]";

    std::cout << "\n\n" << prompt << std::flush;

    while (true) {

        if (enterPressed()) {

            // Erase the prompt
            std::cout << "\r"
                << std::string(prompt.length(), ' ')
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

    auto start = std::chrono::steady_clock::now();

    while (true) {

        if (enterPressed()) {
            return;
        }

        auto now = std::chrono::steady_clock::now();

        auto elapsed =
            std::chrono::duration_cast<std::chrono::milliseconds>(
                now - start
            ).count();

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

    bool skipped = typeText(text, speed);

    // If Enter wasn't used to skip the text,
    // wait for Enter normally.
    if (!skipped) {
        waitForEnter();
    }
}