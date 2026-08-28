#pragma once

#include <string>

bool typeText(const std::string& text, int speed = 15);
void waitForEnter();
void pauseFor(int milliseconds);
void dialogue(const std::string& text, int speed = 15);