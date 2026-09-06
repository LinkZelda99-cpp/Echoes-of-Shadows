#pragma once

#include <string>

bool typeText(const std::string& text, int speed = 15);
void printText(const std::string& text);
void waitForEnter();
void pauseFor(int milliseconds);
void dialogue(const std::string& text, int speed = 15);
std::string wrapText(const std::string& text, int width = 80);