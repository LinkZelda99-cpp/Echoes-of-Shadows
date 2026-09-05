#include "World.h"
#include "Battle.h"
#include "Dialogue.h"

#include <iostream>
#include <limits>
#include <string>

// ============================================================
// EXPLORE
// ============================================================

void explore(Player& player) {

    std::cout << "You decide you should look around. Where do you want to go?\n";
    std::cout << "1. Dark Cave\n";
    std::cout << "2. Ancient Forest\n";
    std::cout << "3. Abandoned Castle\n";

    int choice;

    if (!(std::cin >> choice)) {
        std::cin.clear();
        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(),
            '\n'
        );

        std::cout << "Invalid location.\n";
        return;
    }


    switch (choice) {

        // ========================================================
        // DARK CAVE
        // ========================================================

    case 1: {

        dialogue(
            R"(Hesitantly, you enter the dark cave. Your footsteps echo off the walls and bounce back to you. There are also other sounds. Is that? No, it can't be something else. You must be imagining it. But you can't help but wonder if you're really alone.)"
        );


        // --------------------------------------------------------
        // STUBBORN SWORD
        // --------------------------------------------------------

        bool sword_gotten = false;
        int diceRoll = randint(1, 5);

        if (diceRoll == 2) {

            dialogue(
                R"(Something shiny catches your eye. You walk over and pick it up. It's a rusty sword with one stubborn spot that remains shiny. You picked up the Stubborn Sword. (You can now view the Stubborn Sword from your inventory))"
            );

            player.addItem(Item(
                200,
                "Stubborn Sword",
                "A rusty sword with one stubborn shiny spot.",
                false
            ));

            sword_gotten = true;
        }


        // --------------------------------------------------------
        // THE CHEST
        // --------------------------------------------------------
        std::string caveIntro =
            R"(As you go deeper into the cave, you start to wonder if this was really the right decision. What useful thing could possibly be in this cave)";

        typeText(caveIntro);

        if (sword_gotten == true) {
            typeText(", besides the Stubborn Sword you just picked up");
        }

        typeText(
            "? Immediately, you get an answer. Across the room, a large chest is just sitting there nicely. "
            "\n4\033[3mToo\033[0m nicely.\n"
        );

        waitForEnter();


        // --------------------------------------------------------
        // THE CHEST OPENS
        // --------------------------------------------------------

        dialogue(
            R"(You step closer.
The chest is untouched. No dust. No damage. Almost as if someone placed it there moments ago.
You reach toward the lid.
The ground cracks beneath you.
Something pulls itself out of the earth.)"
);


        // --------------------------------------------------------
        // BATTLE
        // --------------------------------------------------------

        battle(player, 2);


        // ========================================================
        // POST-BATTLE NARRATIVE
        // ========================================================

        if (lastBattleEnemyId == 2) {

            if (lastBattleOutcome == BattleOutcome::Victory) {

                dialogue(
                    R"(The skeleton slowly crumbles onto the ground.)"
                );

            }

            else if (lastBattleOutcome == BattleOutcome::Ran) {

                dialogue(
                    R"(You run away from the skeleton just as it lashes out at you and cuts you. Hiding behind a rock, you miss the chest. No matter. What would you even use the gold on, anyway? There must be something...
The veneerless grotesquerie abnegates back into the besmirched clod, and you take an abysmal breath.
You're safe.
For now.)"
);

            }

            else if (lastBattleOutcome == BattleOutcome::PlayerDefeated) {

                // dialogue("[YOU DIED TO SKELETON STORY - WRITE HERE]");

            }


            // Continue deeper only after defeating the skeleton.
            if (lastBattleOutcome != BattleOutcome::Victory) {
                break;
            }

            // ====================================================
            // CAVE DESCENT
            // ====================================================

            typeText(
                R"(Deeper and deeper into the cave, you find that your steps bounce off the wall and then those sounds bounce off each other again and again before reaching your ear, filling the whole cave with endless noise. The darkness jumps at you, only to be only shadows.

Still going deeper and deeper, your thoughts begin to bounce off each other in your own head as well, and your head is full of noise. No thought is formed that's not corrupted by all the others. The shadows seem to grab you and pull you deeper and deeper into the cave, and it gets darker and darker. The walls of the cave are no longer stone, but mirrors. And you see everything in the cave all at once. The shadows ARE grabbing you. Running, you see the end of the cave. But it twists around you. And suddenly, you don't know which way is up and which is down. Everything is bouncing off of everything else.)",
35
);
            typeText(
                "\033[3mThe shadows overwhelm your senses... darker, darker, yet darker.\033[0m\n",
                45
            );

            pauseFor(1200);

            typeText(
                "\033[3mThe darkness grows stronger... until suddenly, it vanishes.\033[0m\n",
                55
            );

            pauseFor(1800);

            typeText(
                "You find yourself somewhere you've never been before.\n",
                65
            );

            typeText("----------------------\n");

            waitForEnter();

            ++loops;
        }

        break;
    }


          // ============================================================
          // ANCIENT FOREST
          // ============================================================

    case 2:

        dialogue(
            R"(The Ancient Forest is calm... too calm that its eerie. You walk as the huge trees loom ahead of you. You wonder how you even got here in the first place. Are you even-? CRACK! Oop. You know what that is. You turn around to unsuprisingly see a Goblin.)"
        );

        // Goblin encounter uses id 1
        battle(player, 1);

        break;


        // ============================================================
        // ABANDONED CASTLE
        // ============================================================

    case 3:

        dialogue(
            R"(The Abandoned Castle is very eerie... You hear strange noises coming from the upstairs as you walk in. You get ambusued by a goblin!)"
        );

        battle(player, 1);

        break;


        // ============================================================
        // INVALID LOCATION
        // ============================================================

    default:

        std::cout << "Invalid location.\n";
        break;
    }
}