#include "Save.h"

#include <fstream>
#include <sstream>
#include <iomanip>
#include <random>
#include <algorithm>
#include <stdexcept>
#include <cctype>

// ============================================================
// INTERNAL HELPERS
// ============================================================

namespace {

    const std::string SAVE_FILE = "save.dat";


    // --------------------------------------------------------
    // Convert a string to hexadecimal
    // --------------------------------------------------------

    std::string toHex(const std::string& input) {

        std::ostringstream output;

        for (unsigned char c : input) {
            output << std::hex
                << std::setw(2)
                << std::setfill('0')
                << static_cast<int>(c);
        }

        return output.str();
    }


    // --------------------------------------------------------
    // Convert hexadecimal back into a string
    // --------------------------------------------------------

    std::string fromHex(const std::string& input) {

        if (input.length() % 2 != 0) {
        throw std::invalid_argument("Invalid hexadecimal data length.");
    }

    for (unsigned char c : input) {
        if (!std::isxdigit(c)) {
            throw std::invalid_argument("Invalid hexadecimal character.");
        }
    }

    std::string output;

        for (size_t i = 0; i < input.length(); i += 2) {

            std::string byte = input.substr(i, 2);

            char c = static_cast<char>(
                std::stoi(byte, nullptr, 16)
                );

            output += c;
        }

        return output;
    }


    // --------------------------------------------------------
    // XOR transformation
    // --------------------------------------------------------

    std::string xorTransform(
        const std::string& input,
        unsigned char key
    ) {

        std::string output = input;

        for (char& c : output) {
            c ^= key;
        }

        return output;
    }


    // --------------------------------------------------------
    // Reverse a string
    // --------------------------------------------------------

    std::string reverseString(std::string text) {

        std::reverse(text.begin(), text.end());

        return text;
    }

}


// ============================================================
// SAVE GAME
// ============================================================

bool saveGame(const SaveData& data) {

    std::ofstream file(SAVE_FILE, std::ios::trunc);

    if (!file) {
        return false;
    }


    // ========================================================
    // LOOP 0-1: NORMAL
    // ========================================================

    if (data.loops <= 1) {

        file << "PLAYER_NAME=" << data.playerName << '\n';
        file << "LOOPS=" << data.loops << '\n';

        file << "# Echoes of Shadows save data\n";

    }


    // ========================================================
    // LOOP 2-3: SLIGHTLY STRANGE
    // ========================================================

    else if (data.loops <= 3) {

        file << "# SAVE DATA\n";
        file << "DATA_VERSION=2\n";
        file << "PLAYER=" << data.playerName << '\n';
        file << "ECHOES=" << data.loops << '\n';

    }


    // ========================================================
    // LOOP 4-6: HEX
    // ========================================================

    else if (data.loops <= 6) {

        std::string name =
            "PLAYER=" + data.playerName;

        std::string loops =
            "ECHOES=" + std::to_string(data.loops);

        file << "# 4F6E6365 796F7520 72656164 20746869 732E\n";

        file << toHex(name) << '\n';
        file << toHex(loops) << '\n';

    }


    // ========================================================
    // LOOP 7-10: XOR + HEX
    // ========================================================

    else if (data.loops <= 10) {

        std::string name =
            "PLAYER=" + data.playerName;

        std::string loops =
            "ECHOES=" + std::to_string(data.loops);

        name = xorTransform(name, 0x5A);
        loops = xorTransform(loops, 0x5A);

        file << "# DO NOT EDIT\n";
        file << "# 5A5A5A5A5A5A5A5A5A5A\n";

        file << toHex(name) << '\n';
        file << toHex(loops) << '\n';

    }


    // ========================================================
    // LOOP 11+: ABSOLUTE NONSENSE
    // ========================================================

    else {

        std::string dataString =
            "PLAYER=" + data.playerName +
            "|ECHOES=" + std::to_string(data.loops);

        // First transformation
        dataString = xorTransform(dataString, 0x5A);

        // Second transformation
        dataString = reverseString(dataString);

        // Third transformation
        dataString = toHex(dataString);

        file << "# ECHOES OF SHADOWS\n";
        file << "# --------------------------------\n";
        file << "# You probably shouldn't be reading this.\n";
        file << "# --------------------------------\n";

        file << dataString << '\n';

        // Occasionally leave something behind.
        if (data.loops % 5 == 0) {
            file << "# I REMEMBER.\n";
        }

        if (data.loops % 7 == 0) {
            file << "# WHY ARE YOU LOOKING HERE?\n";
        }

        if (data.loops % 11 == 0) {
            file << "# THIS ISN'T WHERE THE SAVE DATA IS.\n";
        }
    }


    return file.good();
}


// ============================================================
// LOAD GAME
// ============================================================

bool loadGame(SaveData& data) {

    std::ifstream file(SAVE_FILE);

    if (!file) {
        return false;
    }

    std::string line;

    std::string playerName;
    int loops = 0;
    bool foundPlayerName = false;
    bool foundLoops = false;


    while (std::getline(file, line)) {

        // Ignore comments
        if (line.empty() || line[0] == '#') {
            continue;
        }


        // ----------------------------------------------------
        // Normal format
        // ----------------------------------------------------

        if (line.rfind("PLAYER_NAME=", 0) == 0) {
            playerName = line.substr(12);
            foundPlayerName = true;
        }

        else if (line.rfind("LOOPS=", 0) == 0) {
            loops = std::stoi(line.substr(6));
            foundLoops = true;
        }


        // ----------------------------------------------------
        // Slightly strange format
        // ----------------------------------------------------

        else if (line.rfind("PLAYER=", 0) == 0) {
            playerName = line.substr(7);
            foundPlayerName = true;
        }

        else if (line.rfind("ECHOES=", 0) == 0) {
            try {
                size_t parsed = 0;
                loops = std::stoi(line.substr(7), &parsed);
                if (parsed != line.size() - 7 || loops < 0) {
                    return false;
                }
                foundLoops = true;
            }
            catch (...) {
                return false;
            }
        }


        // ----------------------------------------------------
        // Hex formats
        // ----------------------------------------------------

        else {

            try {

                std::string decoded = fromHex(line);


                // Try the direct hex format
                if (decoded.rfind("PLAYER=", 0) == 0) {

                    playerName = decoded.substr(7);
                    foundPlayerName = true;
                }

                else if (decoded.rfind("ECHOES=", 0) == 0) {

                    size_t parsed = 0;
                    loops = std::stoi(decoded.substr(7), &parsed);
                    if (parsed != decoded.size() - 7 || loops < 0) {
                        throw std::invalid_argument("Invalid loop count.");
                    }
                    foundLoops = true;
                }


                // Try the crazy format
                else {

                    decoded = reverseString(decoded);

                    decoded = xorTransform(decoded, 0x5A);

                    size_t separator =
                        decoded.rfind("|ECHOES=");

                    if (separator != std::string::npos) {

                        playerName =
                            decoded.substr(
                                7,
                                separator - 7
                            );

                        size_t parsed = 0;
                        loops = std::stoi(
                            decoded.substr(separator + 8), &parsed
                        );
                        if (parsed != decoded.size() - (separator + 8) || loops < 0) {
                            throw std::invalid_argument("Invalid loop count.");
                        }
                        foundPlayerName = true;
                        foundLoops = true;
                    }
                }

            }

            catch (...) {

                // If something isn't recognizable,
                // just ignore it.
            }
        }
    }


    if (!foundPlayerName || !foundLoops || playerName.empty() || loops < 0) {
        return false;
    }


    data.playerName = playerName;
    data.loops = loops;

    return true;
}