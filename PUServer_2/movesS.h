#ifndef INC_MOVESS_H_
#define INC_MOVESS_H_

#include "definitions.h"
#include "statusmove.h"
#include "pokemontype.h"
#include "recoilmove.h"
#include "multiplehitmove.h"
#include "highcriticalhitmove.h"
#include "perfectaccuracymove.h"
#include "pokemonmove.h"
#include "weathermove.h"
#include "onehitkillmove.h"
#include "stockpilemove.h"

#include "pokemon.h"
#include "battlemechanics.h"
#include "movelistentry.h"
#include "statchangeeffect.h"
#include "burneffect.h"
#include "confuseeffect.h"
#include "sleepeffect.h"
#include "paralysiseffect.h"
#include "poisoneffect.h"
#include "percenteffect.h"
#include "chargeeffect.h"
#include "multiplestatchangeeffect.h"
#include "sandstormeffect.h"
#include "suneffect.h"
#include "restrainingeffect.h"
#include "trappingeffect.h"
#include "spikeseffect.h"
#include "substituteeffect.h"
#include "stealthrockeffect.h"
#include "suckerpuncheffect.h"
#include "stockpileeffect.h"
#include "safeguardeffect.h"
#include "snatcheffect.h"

struct Safeguard : public PokemonMove
{
	Safeguard() : PokemonMove(PokemonType::T_NORMAL, 0, 1.0, 15) {};

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender)
	{
		StatusEffect* eff = attacker->getField()->getEffectByType<SafeguardEffect>();
		if(eff == NULL) {
			attacker->getField()->applyEffect(new SafeguardEffect());
		}
		eff = attacker->getField()->getEffectByType<SafeguardEffect>();
		if(eff != NULL) {
			SafeguardEffect* effect = dynamic_cast<SafeguardEffect*>(eff);
			effect->activateParty(attacker);
		}

		return 0;
	};

	bool attemptHit(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender) {
		return true;
	}
};

struct SandAttack : public StatusMove
{
	SandAttack() {
		statusEffectList_t a1;
		a1.push_back(new StatChangeEffect(S_ACCURACY, false));
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_GROUND, 0, 1.0, 15, a1, a2, a3);
	};
};

struct SandTomb : public StatusMove
{
	SandTomb() {
		statusEffectList_t a1;
		a1.push_back(new RestrainingEffect("Sand Tomb", "trapped in a vortex"));
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_GROUND, 15, 0.70, 10, a1, a2, a3);
	};
};

struct Sandstorm : public WeatherMove<SandstormEffect>
{
	Sandstorm() : WeatherMove(PokemonType::T_ROCK, 5, "Smooth Rock") {};
};

struct SacredFire : public StatusMove
{
	SacredFire() {
		statusEffectList_t a1;
		a1.push_back(new BurnEffect());
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 0.5);

		StatusMove(PokemonType::T_FIRE, 100, 0.95, 5, a1, a2, a3);
	};
};

struct ScaryFace : public StatusMove
{
	ScaryFace() {
		statusEffectList_t a1;
		a1.push_back(new StatChangeEffect(S_SPEED, false, 2));
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_NORMAL, 0, 0.9, 10, a1, a2, a3);
	};
};

struct Screech : public StatusMove
{
	Screech() {
		statusEffectList_t a1;
		a1.push_back(new StatChangeEffect(S_DEFENCE, false, 2));
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_NORMAL, 0, 0.85, 40, a1, a2, a3);
	};
};

struct SecretPower : public PokemonMove
{
	SecretPower() : PokemonMove(PokemonType::T_NORMAL, 70, 1.0, 20) {};

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender)
	{
		int damage = mech->calculateDamage(this, attacker, defender);
		defender->changeHealth(-damage);

		int rnd = random_range(0, 100);
		//TODO: This should be based on terrain
		if(rnd <= 30) {
			defender->addStatus(attacker, new ParalysisEffect());
		}

		return damage;
	}
};

struct SeedFlare : public StatusMove
{
	SeedFlare() {
		statusEffectList_t a1;
		a1.push_back(new StatChangeEffect(S_SPDEFENCE, false));
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 0.4);

		StatusMove(PokemonType::T_GRASS, 120, 0.85, 5, a1, a2, a3);
	};
};

struct SeismicToss : public PokemonMove
{
	SeismicToss() : PokemonMove(PokemonType::T_FIGHTING, 0, 1.0, 20) {};

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender) {
		if(getEffectiveness(attacker, defender) == 0.0) {
			std::string message = boost::str(boost::format("It doesn't affect %s...") % defender->getName());
			return 0;
		}

		int damage = attacker->getLevel();
		defender->changeHealth(-damage);
		return damage;
	};
};

struct ShadowBall : public StatusMove
{
	ShadowBall() {
		statusEffectList_t a1;
		a1.push_back(new StatChangeEffect(S_SPDEFENCE, false));
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 0.2);

		StatusMove(PokemonType::T_GHOST, 80, 1.0, 15, a1, a2, a3);
	};
};

struct ShadowClaw : public HighCriticalHitMove
{
	ShadowClaw() : HighCriticalHitMove(PokemonType::T_GHOST, 70, 1.0, 15) {};
};

struct ShadowForce : public StatusMove
{
	ShadowForce() {
		std::vector<std::string> list;
		statusEffectList_t a1;
		a1.push_back(new InvulnerableStateEffect(list));
		a1.push_back(new ChargeEffect(1, "dissappeared from sight!", new MoveListEntry("Shadow Force", new PokemonMove(PokemonType::T_GHOST, 120, 1.0, 5) ) ) );
		boolArray_t a2(2, true);
		doubleArray_t a3(2, 1.0);

		StatusMove(PokemonType::T_GHOST, 0, 1.0, 5, a1, a2, a3);
	};

	bool isAttack() { return true; }
	bool isDamaging() { return true; };
};

struct ShadowPunch : public PerfectAccuracyMove
{
	ShadowPunch() : PerfectAccuracyMove(PokemonType::T_GHOST, 60, 20) {};
};

struct ShadowSneak : public PriorityMove
{
	ShadowSneak() : PriorityMove(PokemonType::T_GHOST, 40, 1.0, 30, 1) {};
};

struct Sharpen : public StatusMove
{
	Sharpen() {
		statusEffectList_t a1;
		a1.push_back(new StatChangeEffect(S_ATTACK, true));
		boolArray_t a2(1, true);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_NORMAL, 0, 1.0, 30, a1, a2, a3);
	};
};

struct SheerCold : public OneHitKillMove
{
	SheerCold() : OneHitKillMove(PokemonType::T_ICE, 5) {};
};

struct ShockWave : public PerfectAccuracyMove
{
	ShockWave() : PerfectAccuracyMove(PokemonType::T_ELECTRIC, 60, 20) {};
};

struct SignalBeam : public StatusMove
{
	SignalBeam() {
		statusEffectList_t a1;
		a1.push_back(new ConfuseEffect());
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 0.1);

		StatusMove(PokemonType::T_BUG, 75, 1.0, 15, a1, a2, a3);
	};
};

struct SilverWind : public StatusMove
{
	SilverWind() {
		intArray_t stats;
		stats.push_back(S_ATTACK);
		stats.push_back(S_DEFENCE);
		stats.push_back(S_SPEED);
		stats.push_back(S_SPATTACK);
		stats.push_back(S_SPDEFENCE);

		statusEffectList_t a1;
		a1.push_back(new MultipleStatChangeEffect(stats));

		boolArray_t a2(1, true);
		doubleArray_t a3(1, 0.1);

		StatusMove(PokemonType::T_BUG, 60, 1.0, 5, a1, a2, a3);
	};
};

struct Sing : public StatusMove
{
	Sing() {
		statusEffectList_t a1;
		a1.push_back(new SleepEffect());
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_NORMAL, 0, 0.55, 15, a1, a2, a3);
	};
};

struct SkillSwap : public PokemonMove
{
	SkillSwap() : PokemonMove(PokemonType::T_PSYCHIC, 0, 1.0, 10) {};

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender) 
	{
		IntrinsicAbility* userAbility = attacker->getAbility()->clone();
		IntrinsicAbility* targetAbility = defender->getAbility();

		if(!userAbility->isSwappable() || !targetAbility->isSwappable()) {
			attacker->getField()->showMessage("But it failed!");
			return 0;
		}

		attacker->setAbility(targetAbility, false);
		defender->setAbility(userAbility, false);

		std::string message = boost::str(boost::format("%s swapped abilities with its opponent!") % attacker->getName());
		attacker->getField()->showMessage(message);

		return 0;
	};
};

struct SkullBash : public StatusMove
{
	SkullBash() {
		statusEffectList_t a1;
		a1.push_back(new StatChangeEffect(S_DEFENCE, true, 1));
		a1.push_back(new ChargeEffect(1, "lowered its head!", new MoveListEntry("Skull Bash", new PokemonMove(PokemonType::T_NORMAL, 100, 1.0, 15) ) ) );
		boolArray_t a2(2, true);
		doubleArray_t a3(2, 1.0);
		StatusMove(PokemonType::T_NORMAL, 0, 1.0, 15, a1, a2, a3);
	};

	bool isAttack() { return true; };
	bool isDamaging() { return true; };
};

struct SkyAttack : public StatusMove
{
	SkyAttack() {
		statusEffectList_t a1;
		a1.push_back(new ChargeEffect(1, "is glowing!", new MoveListEntry("Sky Attack", new PokemonMove(PokemonType::T_FLYING, 140, 0.9, 5) ) ) );
		boolArray_t a2(1, true);
		doubleArray_t a3(1, 1.0);
		StatusMove(PokemonType::T_NORMAL, 0, 1.0, 15, a1, a2, a3);
	};

	bool isAttack() { return true; };
	bool isDamaging() { return true; };
};

struct SlackOff : public StatusMove
{
	SlackOff() {
		statusEffectList_t a1;
		a1.push_back(new PercentEffect(0.5, false, -1, ""));
		boolArray_t a2(1, true);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_NORMAL, 0, 1.0, 10, a1, a2, a3);
	};
};

struct Slash : public HighCriticalHitMove
{
	Slash() : HighCriticalHitMove(PokemonType::T_NORMAL, 70, 1.0, 20) {};
};

struct SleepPowder : public StatusMove
{
	SleepPowder() {
		statusEffectList_t a1;
		a1.push_back(new SleepEffect());
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_GRASS, 0, 0.75, 15, a1, a2, a3);
	};
};

struct SleepTalk : public PokemonMove
{
	SleepTalk() : PokemonMove(PokemonType::T_NORMAL, 0, 1.0, 10) {};

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender)
	{
		if(!attacker->hasEffect<SleepEffect>()) {
			attacker->getField()->showMessage("But it failed!");
			return 0;
		}

		MoveListEntry* move;
		bool hasOtherMoves = false;
		for(int i = 0; i < 4; ++i) {
			if((move = attacker->getMove(i)) != NULL) {
				if(move->getName() != "Sleep Talk") {
					hasOtherMoves = true;
					break;
				}
			}
		}

		if(!hasOtherMoves) {
			attacker->getField()->showMessage("But it failed!");
			return 0;
		}

		std::string name;
		do {
			move = attacker->getMove(random_range(0,3));
			if(move != NULL) {
				name = move->getName();
				continue;
			} else {			
				continue;
			}
		} while(name == "Sleep Talk");

		if(name == "Focus Punch") {
			attacker->getField()->showMessage("But it failed!");
			return 0;
		}

		return attacker->useMove(move, defender);
	};

	SleepEffect* getStatusException() { return SleepEffect::getDummy(); };
};

struct Sludge : public StatusMove
{
	Sludge() {
		statusEffectList_t a1;
		a1.push_back(new PoisonEffect());
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 0.3);

		StatusMove(PokemonType::T_POISON, 65, 1.0, 20, a1, a2, a3);
	};
};

struct SludgeBomb : public StatusMove
{
	SludgeBomb() {
		statusEffectList_t a1;
		a1.push_back(new PoisonEffect());
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 0.1);

		StatusMove(PokemonType::T_POISON, 90, 1.0, 10, a1, a2, a3);
	};
};

struct Smellingsalt : public PokemonMove
{
	Smellingsalt() : PokemonMove(PokemonType::T_NORMAL, 60, 1.0, 10) {};

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender) {
		int power = getPower();

		if(!defender->hasSubstitute() && defender->hasEffect<ParalysisEffect>()) {
			setPower(2 * power);
			defender->removeStatus(SPECIAL_EFFECT_LOCK);

			std::string message = boost::str(boost::format("%s was cured of paralysis") % defender->getName());
			attacker->getField()->showMessage(message);
		}

		int damage = mech->calculateDamage(this, attacker, defender);
		setPower(power);
		defender->changeHealth(-damage);
		return damage;
	};
};

struct Smog : public StatusMove
{
	Smog() {
		statusEffectList_t a1;
		a1.push_back(new PoisonEffect());
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 0.3);

		StatusMove(PokemonType::T_POISON, 20, 0.7, 20, a1, a2, a3);
	};
};

struct Smokescreen : public StatusMove
{
	Smokescreen() {
		statusEffectList_t a1;
		a1.push_back(new StatChangeEffect(S_ACCURACY, false));
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_NORMAL, 0, 1.0, 20, a1, a2, a3);
	};
};

struct Snatch : public PokemonMove
{
	Snatch() : PokemonMove(PokemonType::T_DARK, 0, 1.0, 10) {};

	void beginTurn(battleTurnList_t turn, int index, Pokemon *source)
	{
		if(source->hasEffect<SleepEffect>() || source->hasEffect<FreezeEffect>()) {
			return;
		}

		Pokemon* opponent = source->getOpponent();
		BattleTurn* opp = turn.at(opponent->getMaster()->getGUID());
		if(!opp->isMoveTurn()) return;

		MoveListEntry* entry = opponent->getMove(opp->getId());
		PokemonMove* move = entry->getMove();
		if(!isAssignableFrom<StatusMove*>(move)) return;

		StatusMove* statusMove = dynamic_cast<StatusMove*>(move);
		if(statusMove->isAttack()) return;

		statusEffectList_t effects = statusMove->getEffects();
		source->addStatus(source, new SnatchEffect(effects));
	};

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender) 
	{
		std::string message = boost::str(boost::format("%s awaits the foe's move!") % attacker->getName());
		attacker->getField()->showMessage(message);
		SnatchEffect* eff = dynamic_cast<SnatchEffect*>(attacker->getEffect<SnatchEffect>());
		if(eff == NULL) {
			return 0;
		}

		statusEffectList_t effects = eff->getEffects();
		if(effects.size() > 0) {
			message = boost::str(boost::format("%s snatched the foe's effects") % attacker->getName());
			attacker->getField()->showMessage(message);
			for(int i = 0; i < effects.size(); ++i) {
				attacker->addStatus(attacker, effects.at(i));
			}

			return 0;
		}
	};

	bool attemptHit(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender) { return true; };
};

struct Snore : public PokemonMove
{
	Snore() : PokemonMove(PokemonType::T_NORMAL, 40, 1.0, 15) {};

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender) {
		if(!attacker->hasEffect<SleepEffect>()) {
			attacker->getField()->showMessage("But it failed!");
			return 0;
		}

		int damage = mech->calculateDamage(this, attacker, defender);
		defender->changeHealth(-damage);

		return damage;
	};

	SleepEffect* getStatusException() { return SleepEffect::getDummy(); };
};

struct Softboiled : public StatusMove
{
	Softboiled() {
		statusEffectList_t a1;
		a1.push_back(new PercentEffect(0.5, false, -1, ""));
		boolArray_t a2(1, true);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_NORMAL, 0, 1.0, 10, a1, a2, a3);
	};
};

struct Solarbeam : public StatusMove
{
	Solarbeam() {
		statusEffectList_t a1;
		a1.push_back(new ChargeEffect(1, "takes in sunlight", new MoveListEntry("Solarbeam", new PokemonMove(PokemonType::T_GRASS, 120, 1.0, 10) ) ) );
		boolArray_t a2(1, true);
		doubleArray_t a3(1, 1.0);
		
		StatusMove(PokemonType::T_GRASS, 0, 1.0, 10, a1, a2, a3);
	}

	bool isAttack() { return true; };
	bool isDamaging() { return true; };
};

struct Sonicboom : public PokemonMove
{
	Sonicboom() : PokemonMove(PokemonType::T_NORMAL, 0, 0.9, 20) {};

	bool isAttack() { return true; };

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender) {
		if(getEffectiveness(attacker, defender) == 0.0) {
			std::string message = boost::str(boost::format("It doesn't affect %s...") % defender->getName());
			return 0;
		}

		int damage = 20;
		defender->changeHealth(-damage);
		return damage;
	};
};

struct SpacialRend : public HighCriticalHitMove
{
	SpacialRend() : HighCriticalHitMove(PokemonType::T_DRAGON, 100, 0.95, 5) {};
};

struct Spark : public StatusMove
{
	Spark() {
		statusEffectList_t a1;
		a1.push_back(new ParalysisEffect());
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 0.3);

		StatusMove(PokemonType::T_ELECTRIC, 65, 1.0, 20, a1, a2, a3);
	};
};

struct SpiderWeb : public StatusMove
{
	SpiderWeb() {
		statusEffectList_t a1;
		a1.push_back(new TrappingEffect("Spider Web"));
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_BUG, 0, 1.0, 5, a1, a2, a3);
	};
};

struct SpikeCannon : public MultipleHitMove
{
	SpikeCannon() : MultipleHitMove(PokemonType::T_NORMAL, 20, 1.0, 15) {};
};

struct Spikes : public PokemonMove
{
	Spikes() : PokemonMove(PokemonType::T_GROUND, 0, 1.0, 20) {};

	bool attemptHit(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender) { return true; };

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender) {
		BattleField* field = attacker->getField();
		SpikesEffect* spikes = SpikesEffect::getSpikes<SpikesEffect>(false);

		if(!spikes) {
			spikes = new SpikesEffect();
			field->applyEffect(spikes);
		}

		spikes->addSpikes(defender);
		return 0;
	};
};

struct SpitUp : public StockpileMove
{
	SpitUp() : StockpileMove(PokemonType::T_NORMAL, 0, 1.0, 10) {};

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender) 
	{
		int levels = getLevels(attacker);
		if(levels <= 0) {
			attacker->getField()->showMessage("But it failed to spit up anything!");
			return 0;
		}

		setPower(100 * levels);
		int damage = StockpileMove::use(mech, attacker, defender);
		setPower(0);
		attacker->removeStatus(getStockpileEffect(attacker));

		return damage;
	};

	bool isAttack() { return true; };
};

struct Spite : public PokemonMove
{
	Spite() : PokemonMove(PokemonType::T_GHOST, 0, 1.0, 10) {};

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender)
	{
		MoveListEntry* move = defender->getLastMove();
		if(move == NULL) {
			attacker->getField()->showMessage("But it failed!");
			return 0;
		}

		for(int i = 0; i < 4; ++i)
		{
			if(move->equals(defender->getMove(i)))
			{
				defender->setPp(i, defender->getPp(i) - 4);
				break;
			}
		}

		return 0;
	}
};

struct Splash : public PokemonMove
{
	Splash() : PokemonMove(PokemonType::T_NORMAL, 0, 1.0, 40) {};

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender) {
		attacker->getField()->showMessage("But nothing happened!");
		return 0;
	};
};

struct Spore : public StatusMove
{
	Spore() {
		statusEffectList_t a1;
		a1.push_back(new SleepEffect());
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_GRASS, 0, 1.0, 15, a1, a2, a3);
	};
};

struct StealthRock : public PokemonMove
{
	StealthRock() : PokemonMove(PokemonType::T_ROCK, 0, 1.0, 20) {};

	bool attemptHit(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender) { return true; };

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender)
	{
		BattleField* field = attacker->getField();
		StealthRockEffect* spikes = dynamic_cast<StealthRockEffect*>(SpikesEffect::getSpikes<StealthRockEffect>(field) );
		if(spikes == NULL) {
			spikes = new StealthRockEffect();
			field->applyEffect(spikes);
		}

		spikes->addSpikes(defender);
		return 0;
	};
};

struct SteelWing : public StatusMove
{
	SteelWing() {
		statusEffectList_t a1;
		a1.push_back(new StatChangeEffect(S_DEFENCE, true));
		boolArray_t a2(1, true);
		doubleArray_t a3(1, 0.1);

		StatusMove(PokemonType::T_STEEL, 70, 0.9, 25, a1, a2, a3);
	};
};

struct Stockpile : public PokemonMove
{
	Stockpile() : PokemonMove(PokemonType::T_NORMAL, 0, 1.0, 10) {};

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender)
	{
		StockpileEffect* eff = (StockpileEffect*)attacker->getEffect<StockpileEffect>();
		if(eff == NULL) {
			eff = (StockpileEffect*)attacker->addStatus(attacker, new StockpileEffect());
		}

		eff->incrementLevel(attacker);
		return 0;
	}
};

struct Stomp : public StatusMove
{
	Stomp() {
		statusEffectList_t a1;
		a1.push_back(new FlinchEffect());
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 0.3);

		StatusMove(PokemonType::T_NORMAL, 65, 1.0, 20, a1, a2, a3);
	};
};

struct StoneEdge : public HighCriticalHitMove
{
	StoneEdge() : HighCriticalHitMove(PokemonType::T_ROCK, 100, 0.8, 5) {};
};

struct StringShot : public StatusMove
{
	StringShot() {
		statusEffectList_t a1;
		a1.push_back(new StatChangeEffect(S_SPEED, false));
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_BUG, 0, 0.95, 40, a1, a2, a3);
	};
};

struct Struggle : public RecoilMove
{
	Struggle() : RecoilMove(PokemonType::T_TYPELESS, 50, 1.0, 1, 0.5) {};

	bool isProtected(Pokemon* p) { return false; }

	int getRecoil(Pokemon* p, int damage) {
		if(isAssignableFrom<JewelMechanics*>(p->getField()->getMechanics())) {
			return p->getStat(S_HP) / 4;
		}

		return RecoilMove::getRecoil(p, damage);
	}
};

struct StunSpore : public StatusMove
{
	StunSpore() {
		statusEffectList_t a1;
		a1.push_back(new ParalysisEffect());
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_GRASS, 0, 0.75, 30, a1, a2, a3);
	};
};

struct Submission : public RecoilMove
{
	Submission() : RecoilMove(PokemonType::T_FIGHTING, 80, 0.8, 25, 0.25) {};
};

struct Substitute : public PokemonMove
{
	Substitute() : PokemonMove(PokemonType::T_NORMAL, 0, 1.0, 10) {};

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender)
	{
		if(!attacker->createSubstitude()) {
			attacker->getField()->showMessage("But it failed!");
		} else {
			attacker->addStatus(attacker, new SubstituteEffect());
		}

		return 0;
	};

	bool attemptHit(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender) { return true; };
};

struct SuckerPunch : public PokemonMove
{
	SuckerPunch() : PokemonMove(PokemonType::T_DARK, 80, 1.0, 5) {};

	void beginTurn(battleTurnList_t turn, int index, Pokemon *source)
	{
		// Assume two pokemon
		battleTurnList_t::iterator it = turn.begin();
		if(source->getMaster()->getGUID() != it->first) {
			// User must be going first
			return;
		}

		Pokemon* opponent = source->getOpponent();
		uint64_t oppIndex = opponent->getMaster()->getGUID();
		BattleTurn* opp = turn.at(oppIndex);

		if(opp != NULL && opp->isMoveTurn() && opp->getMove(opponent)->isDamaging()) {
			source->addStatus(source, new SuckerPunchEffect());
		}
	};

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender)
	{
		if(!attacker->hasEffect<SuckerPunchEffect>()) {
			attacker->getField()->showMessage("But it failed!");
			return 0;
		}

		return PokemonMove::use(mech, attacker, defender);
	};

	int getPriority() { return 1; };
};

struct SunnyDay : public WeatherMove<SunEffect>
{
	SunnyDay() : WeatherMove(PokemonType::T_FIRE, 5, "Heat Rock") {};
};

struct SuperFang : public PokemonMove
{
	SuperFang() : PokemonMove(PokemonType::T_NORMAL, 0, 0.9, 10) {};

	bool isAttack() { return true; };

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender) {
		int damage = defender->getHealth() / 2;
		if(damage <= 0) damage = 1;

		defender->changeHealth(-damage);
		return damage;
	}
};

struct Superpower : public StatusMove
{
	Superpower() {
		statusEffectList_t a1;
		a1.push_back(new StatChangeEffect(S_ATTACK, false));
		a1.push_back(new StatChangeEffect(S_DEFENCE, false));
		boolArray_t a2(2, true);
		doubleArray_t a3(2, 1.0);

		StatusMove(PokemonType::T_FIGHTING, 120, 1.0, 5, a1, a2, a3);
	};
};

struct Supersonic : public StatusMove
{
	Supersonic() {
		statusEffectList_t a1;
		a1.push_back(new ConfuseEffect());
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_NORMAL, 0, 0.55, 30, a1, a2, a3);
	};
};

struct Swagger : public StatusMove
{
	Swagger() {
		statusEffectList_t a1;
		a1.push_back(new StatChangeEffect(S_ATTACK, true, 2));
		a1.push_back(new ConfuseEffect());
		boolArray_t a2(2, false);
		doubleArray_t a3(2, 1.0);

		StatusMove(PokemonType::T_NORMAL, 0, 0.9, 15, a1, a2, a3);
	};
};

struct Swallow : public StockpileMove
{
	Swallow() : StockpileMove(PokemonType::T_NORMAL, 0, 1.0, 10) {};

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender)
	{
		int levels = getLevels(attacker);
		if(levels <= 0) {
			attacker->getField()->showMessage("But it failed to swallow anything!");
			return 0;
		}

		double percents[] = { 0.0, 0.25, 0.5, 1.0 };
		double percent = percents[levels];

		attacker->addStatus(attacker, new PercentEffect(percent, false, -1, ""));
		attacker->removeStatus(getStockpileEffect(attacker));

		return 0;
	};
};

struct SweetKiss : public StatusMove
{
	SweetKiss() {
		statusEffectList_t a1;
		a1.push_back(new ConfuseEffect());
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_NORMAL, 0, 0.75, 10, a1, a2, a3);
	};
};

struct SweetScent : public StatusMove
{
	SweetScent() {
		statusEffectList_t a1;
		a1.push_back(new StatChangeEffect(S_EVASION, false));
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_NORMAL, 0, 1.0, 30, a1, a2, a3);
	};
};

struct Swift : public PerfectAccuracyMove
{
	Swift() : PerfectAccuracyMove(PokemonType::T_NORMAL, 60, 20) {};
};

struct Switcheroo : public PokemonMove
{
	Switcheroo() : PokemonMove(PokemonType::T_DARK, 0, 1.0, 10) {};
	
	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender)
	{
		if(defender->hasSubstitute()) {
			attacker->getField()->showMessage("But it failed!");
			return 0;
		}

		if(defender->hasAbility("Sticky Hold") || attacker->hasAbility("Sticky Hold")) {
			std::string message;

			if(attacker->hasAbility("Skicky Hold")) {
				message = boost::str(boost::format("%s hung on with its Sticky Hold!") % attacker->getName());
				attacker->getField()->showMessage(message);
			}
			if(defender->hasAbility("Skicky Hold")) {
				message = boost::str(boost::format("%s hung on with its Sticky Hold!") % defender->getName());
				attacker->getField()->showMessage(message);
			}

			return 0;
		}

		//TODO: Implement when HoldItem is done
		/*
		HoldItem *defenderItem = defender->getItem();
		HoltItem *userItem = (attacker->getItem() == NULL) ? NULL : attacker->getItem()->clone();
		attacker->setItem(defenderItem);
		defender->setItem(userItem);

		std::string message;
		if(defenderItem != NULL) {
			message = boost::str(boost::format("%s obtained %s!") % attacker->getName() % defenderItem->getName());
			attacker->getField->showMessage(message);
		}
		if(userItem != NULL) {
			message = boost::str(boost::format("%s obtained %s!") % defender->getName() % userItem->getName());
			attacker->getField->showMessage(message);
		}
		*/

		return 0;
	};

	bool isAttack() { return true; };
	bool isDamaging() { return false; };
};

struct SwordsDance : public StatusMove
{
	SwordsDance() {
		statusEffectList_t a1;
		a1.push_back(new StatChangeEffect(S_ATTACK, true, 2));
		boolArray_t a2(1, true);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_NORMAL, 0, 1.0, 20, a1, a2, a3);
	};
};

#endif