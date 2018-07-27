#include <Creature.h>
#include <Dice.h>

void Creature::initRoll() {
	if (hasInitAdvantage && !hasInitDisadvantage) {
		init = d20adv() + dex;
	}
	else if (!hasInitAdvantage && hasInitDisadvantage) {
		init = d20dis() + dex;
	}
	else {
		init = d20() + dex;
	}
}

bool Creature::checkHit(int attack) {
	// just compare attack to ac for now. reactions pending
	return attack >= ac;
}

void Creature::takeDamage(int dmg, DMG_TYPE type) {
	// reduce temphp first
	int diff = temphp - dmg;
	if (diff > 0) {
		temphp -= dmg;
	}
	else if (diff == 0) {
		temphp = 0;
	}
	else {
		hp += diff;
	}
}

int Creature::attack() {
	return d20() + attackBonus;
}

int Creature::doDamage() {
	return dmgFunc();
}

void Player::takeTurn(std::vector<Creature*>& party, std::vector<Creature*>& foes) {
	// movement ignored for now

	// do action if able -- for now, choose a random foe and attack
	for (int attackNum = 0; attackNum < attacksPerAction; attackNum++) {
		Creature* foe = nullptr;
		size_t nFoes = foes.size();
		int index = randInt(nFoes) - 1;
		for (size_t iter = 0; iter < nFoes; ++iter) {
			if (foes[(iter + index) % nFoes]->alive) {
				foe = foes[(iter + index) % nFoes];
				break;
			}
		}

		if (foe != nullptr) {
			if (foe->checkHit(attack())) {
				foe->takeDamage(doDamage(), dmgType);
			}
		}
	}

	// bonus action ignored for now

}

void Player::takeDamage(int dmg, DMG_TYPE type) {
	Creature::takeDamage(dmg, type);
	if (hp <= 0) {
		alive = false;
		stable = false;
		deathSaves = 0;
		deathFails = 0;
	}
	if (hp < (-maxhp / 2)) dead = true;
}

bool Player::deathCheck() {
	int roll = d20();
	if (roll == 1) {
		deathFails += 2;
	}
	else if (roll == 20) {
		alive = true;
		hp = 1;
		return true;
	}
	else if (roll < 11) {
		deathFails += 1;
	}
	else {
		deathSaves += 1;
	}

	if (deathFails == 3) {
		dead = true;
	}
	if (deathSaves == 3) {
		deathSaves = 0;
		deathFails = 0;
		stable = true;
	}
	return false;
}

void Foe::takeTurn(std::vector<Creature*>& party, std::vector<Creature*>& foes) {
	// movement ignored for now

	// do action if able -- for now, choose a random player and attack
	for (int attackNum = 0; attackNum < attacksPerAction; attackNum++) {
		Creature* plyr = nullptr;
		size_t nPlyrs = party.size();
		int index = randInt(nPlyrs) - 1;
		for (size_t iter = 0; iter < nPlyrs; ++iter) {
			if (party[(iter + index) % nPlyrs]->alive) {
				plyr = party[(iter + index) % nPlyrs];
				break;
			}
		}

		if (plyr != nullptr) {
			if (plyr->checkHit(attack())) {
				plyr->takeDamage(doDamage(), dmgType);
			}
		}
	}

	// bonus action ignored for now

}

void Foe::takeDamage(int dmg, DMG_TYPE type) {
	Creature::takeDamage(dmg, type);
	if (hp <= 0) {
		alive = false; dead = true;
	}
}