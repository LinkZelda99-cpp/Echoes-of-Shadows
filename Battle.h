#pragma once

#include "Player.h"

void battle(Player& player, int id);
int randint(int min, int max);

enum class BattleOutcome {
	None,
	Victory,
	Ran,
	PlayerDefeated
};

extern BattleOutcome lastBattleOutcome;
extern int lastBattleEnemyId;
