#include "movelist.h"

#include "movelistentry.h"
#include "pokemontype.h"
#include "pokemonmove.h"
#include "weathermove.h"
#include "statusmove.h"
#include "statchangeeffect.h"
#include "percenteffect.h"

#include "hiddenpowermove.h"

#include "movesA.h"
#include "movesB.h"
#include "movesC.h"
#include "movesD.h"
#include "movesE.h"
#include "movesF.h"
#include "movesG.h"
#include "movesH.h"
#include "movesI.h"
#include "movesJ.h"
#include "movesK.h"
#include "movesL.h"
#include "movesM.h"
#include "movesN.h"
#include "movesO.h"
#include "movesP.h"
#include "movesQ.h"
#include "movesR.h"
#include "movesS.h"
#include "movesT.h"
#include "movesU.h"
#include "movesV.h"
#include "movesW.h"
#include "movesY.h"
#include "movesZ.h"

MoveList *MoveList::m_instance = NULL;

MoveList::MoveList()
{
	m_instance = this;

	// Initialize here
	initNonStatusMoves();
	initStatusMoves();
	initNamesMoves();
}

MoveList::~MoveList()
{
	delete m_instance;
}

MoveListEntry* MoveList::getMove( std::string moveName )
{
	moveList_t::iterator it = m_moves.find(moveName);
	if(it != m_moves.end()) {
		return it->second;
	}

	return NULL;
}

void MoveList::initNonStatusMoves()
{
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Aqua Tail", new MoveListEntry("Aqua Tail",
		new PokemonMove(PokemonType::T_WATER, 90, 0.9, 10))));

	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Cut", new MoveListEntry("Cut",
		new PokemonMove(PokemonType::T_NORMAL, 50, 0.95, 30))));

	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Dragon Claw", new MoveListEntry("Dragon Claw",
		new PokemonMove(PokemonType::T_DRAGON, 80, 1.0, 15))));

	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Dragon Pulse", new MoveListEntry("Dragon Pulse",
		new PokemonMove(PokemonType::T_DRAGON, 90, 1.0, 10))));

	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Drill Peck", new MoveListEntry("Drill Peck",
		new PokemonMove(PokemonType::T_FLYING, 80, 1.0, 20))));

	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Earthquake", new MoveListEntry("Earthquake",
		new PokemonMove(PokemonType::T_GROUND, 100, 1.0, 10))));

	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Egg Bomb", new MoveListEntry("Egg Bomb",
		new PokemonMove(PokemonType::T_NORMAL, 75, 1.0, 10))));

	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Gust", new MoveListEntry("Gust",
		new PokemonMove(PokemonType::T_FLYING, 35, 1.0, 35))));

	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Horn Attack", new MoveListEntry("Horn Attack",
		new PokemonMove(PokemonType::T_NORMAL, 65, 1.0, 25))));

	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Hydro Pump", new MoveListEntry("Hydro Pump",
		new PokemonMove(PokemonType::T_WATER, 120, 0.8, 5))));

	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Hyper Voice", new MoveListEntry("Hyper Voice",
		new PokemonMove(PokemonType::T_NORMAL, 90, 1.0, 10))));

	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Megahorn", new MoveListEntry("Megahorn",
		new PokemonMove(PokemonType::T_BUG, 120, 0.85, 10))));

	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Mega Punch", new MoveListEntry("Mega Punch",
		new PokemonMove(PokemonType::T_NORMAL, 80, 0.85, 20))));

	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Mega Kick", new MoveListEntry("Mega Kick",
		new PokemonMove(PokemonType::T_NORMAL, 120, 0.75, 5))));

	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Peck", new MoveListEntry("Peck",
		new PokemonMove(PokemonType::T_FLYING, 35, 1.0, 35))));

	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Pound", new MoveListEntry("Pound",
		new PokemonMove(PokemonType::T_NORMAL, 40, 1.0, 35))));

	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Power Gem", new MoveListEntry("Power Gem",
		new PokemonMove(PokemonType::T_ROCK, 70, 1.0, 20))));

	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Power Whip", new MoveListEntry("Power Whip",
		new PokemonMove(PokemonType::T_GRASS, 120, 0.85, 10))));

	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Rock Throw", new MoveListEntry("Rock Throw",
		new PokemonMove(PokemonType::T_ROCK, 50, 0.9, 15))));

	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Rolling Kick", new MoveListEntry("Rolling Kick",
		new PokemonMove(PokemonType::T_NORMAL, 60, 0.85, 15))));

	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Scratch", new MoveListEntry("Scratch",
		new PokemonMove(PokemonType::T_NORMAL, 40, 1.0, 35))));

	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Seed Bomb", new MoveListEntry("Seed Bomb",
		new PokemonMove(PokemonType::T_GRASS, 80, 1.0, 15))));

	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Sky Uppercut", new MoveListEntry("Sky Uppercut",
		new PokemonMove(PokemonType::T_FIGHTING, 85, 0.9, 15))));

	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Slam", new MoveListEntry("Slam",
		new PokemonMove(PokemonType::T_NORMAL, 80, 0.75, 20))));

	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Surf", new MoveListEntry("Surf",
		new PokemonMove(PokemonType::T_WATER, 95, 1.0, 15))));

	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Tackle", new MoveListEntry("Tackle",
		new PokemonMove(PokemonType::T_NORMAL, 35, 0.95, 35))));

	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Vicegrip", new MoveListEntry("Vicegrip",
		new PokemonMove(PokemonType::T_NORMAL, 55, 1.0, 30))));

	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Vine Whip", new MoveListEntry("Vine Whip",
		new PokemonMove(PokemonType::T_GRASS, 35, 1.0, 10))));

	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Water Gun", new MoveListEntry("Water Gun",
		new PokemonMove(PokemonType::T_WATER, 40, 1.0, 25))));

	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Waterfall", new MoveListEntry("Waterfall",
		new PokemonMove(PokemonType::T_WATER, 80, 1.0, 15))));

	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Wing Attack", new MoveListEntry("Wing Attack",
		new PokemonMove(PokemonType::T_FLYING, 60, 1.0, 35))));

	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Strenght", new MoveListEntry("Strength",
		new PokemonMove(PokemonType::T_NORMAL, 80, 1.0, 15))));

	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Return", new MoveListEntry("Return",
		new PokemonMove(PokemonType::T_NORMAL, 102, 1.0, 20))));

	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Frustration", new MoveListEntry("Frustration",
		new PokemonMove(PokemonType::T_NORMAL, 102, 1.0, 20))));

	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Weather Ball", new MoveListEntry("Weather Ball",
		new PokemonMove(PokemonType::T_NORMAL, 50, 1.0, 10))));

	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("X-Scissor", new MoveListEntry("X-Scissor",
		new PokemonMove(PokemonType::T_BUG, 80, 1.0, 15))));
}

void MoveList::initStatusMoves()
{
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Absorb", new MoveListEntry("Absorb", new Absorb())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Acid", new MoveListEntry("Acid", new Acid())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Acid Armor", new MoveListEntry("Acid Armor", new AcidArmor())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Acupressure", new MoveListEntry("Acupressure", new Acupressure())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Aerial Ace", new MoveListEntry("Aerial Ace", new AerialAce())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Aeroblast", new MoveListEntry("Aeroblast", new Aeroblast())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Agility", new MoveListEntry("Agility", new Agility())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Air Cutter", new MoveListEntry("Air Cutter", new AirCutter())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Air Slash", new MoveListEntry("Air Slash", new AirSlash())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Amnesia", new MoveListEntry("Amnesia", new Amnesia())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Ancientpower", new MoveListEntry("Ancientpower", new Ancientpower())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Aqua Jet", new MoveListEntry("Aqua Jet", new AquaJet())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Aqua Ring", new MoveListEntry("Aqua Ring", new AquaRing())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Aqua Tail", new MoveListEntry("Aqua Tail", new AquaTail())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Arm Thrust", new MoveListEntry("Arm Thrust", new ArmThrust())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Assist", new MoveListEntry("Assist", new Assist())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Assurance", new MoveListEntry("Assurance", new Assurance())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Astonish", new MoveListEntry("Astonish", new Astonish())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Attack Order", new MoveListEntry("Attack Order", new AttackOrder())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Attract", new MoveListEntry("Attract", new Attract())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Aurora Beam", new MoveListEntry("Aurora Beam", new AuroraBeam())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Aura Sphere", new MoveListEntry("Aura Sphere", new AuraSphere())));

	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Barrage", new MoveListEntry("Barrage", new Barrage())));	
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Barrier", new MoveListEntry("Barrier", new Barrier())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Baton Pass", new MoveListEntry("Baton Pass", new BatonPass())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Beat Up", new MoveListEntry("Beat Up", new BeatUp())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Belly Drum", new MoveListEntry("Belly Drum", new BellyDrum())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Bide", new MoveListEntry("Bide", new Bide())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Bite", new MoveListEntry("Bite", new Bite())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Bind", new MoveListEntry("Bind", new Bind())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Blast Burn", new MoveListEntry("Blast Burn", new BlastBurn())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Blizzard", new MoveListEntry("Blizzard", new Blizzard())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Block", new MoveListEntry("Block", new Block())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Body Slam", new MoveListEntry("Body Slam", new BodySlam())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Bone Club", new MoveListEntry("Bone Club", new BoneClub())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Bone Rush", new MoveListEntry("Bone Rush", new BoneRush())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Bonemerang", new MoveListEntry("Bonemerang", new Bonemerang())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Bounce", new MoveListEntry("Bounce", new Bounce())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Brave Bird", new MoveListEntry("Brave Bird", new BraveBird())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Brick Break", new MoveListEntry("Brick Break", new BrickBreak())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Brine", new MoveListEntry("Brine", new Brine())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Bubble", new MoveListEntry("Bubble", new Bubble())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Bubblebeam", new MoveListEntry("Bubblebeam", new Bubblebeam())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Bug Bite", new MoveListEntry("Bug Bite", new BugBite())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Bug Buzz", new MoveListEntry("Bug Buzz", new BugBuzz())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Bulk Up", new MoveListEntry("Bulk Up", new BulkUp())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Bullet Punch", new MoveListEntry("Bullet Punch", new BulletPunch())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Bullet Seed", new MoveListEntry("Bullet Seed", new BulletSeed())));

	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Calm Mind", new MoveListEntry("Calm Mind", new CalmMind())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Camouflage", new MoveListEntry("Camouflage", new Camouflage())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Captivate", new MoveListEntry("Captivate", new Captivate())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Charge", new MoveListEntry("Charge", new Charge())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Charge Beam", new MoveListEntry("Charge Beam", new ChargeBeam())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Charm", new MoveListEntry("Charm", new Charm())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Chatter", new MoveListEntry("Chatter", new Chatter())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Clamp", new MoveListEntry("Clamp", new Clamp())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Close Combat", new MoveListEntry("Close Combat", new CloseCombat())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Comet Punch", new MoveListEntry("Comet Punch", new Confusion())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Constrict", new MoveListEntry("Constrict", new Constrict())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Confuse Ray", new MoveListEntry("Confuse Ray", new ConfuseRay())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Confusion", new MoveListEntry("Confusion", new Confusion())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Conversion", new MoveListEntry("Conversion", new Conversion())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Conversion 2", new MoveListEntry("Conversion 2", new Conversion2())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Cosmic Power", new MoveListEntry("Cosmic Power", new CosmicPower())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Cotton Spore", new MoveListEntry("Cotton Spore", new CottonSpore())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Counter", new MoveListEntry("Counter", new Counter())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Crabhammer", new MoveListEntry("Crabhammer", new Crabhammer())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Cross Chop", new MoveListEntry("Cross Chop", new CrossChop())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Cross Poison", new MoveListEntry("Cross Poison", new CrossPoison())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Crunch", new MoveListEntry("Crunch", new Crunch())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Crush Claw", new MoveListEntry("Crush Claw", new CrushClaw())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Crush Grip", new MoveListEntry("Crush Grip", new CrushGrip())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Curse", new MoveListEntry("Curse", new Curse())));

	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Dark Void", new MoveListEntry("Dark Void", new DarkVoid())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Dark Pulse", new MoveListEntry("Dark Pulse", new DarkPulse())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Defend Order", new MoveListEntry("Defend Order", new DefendOrder())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Defense Curl", new MoveListEntry("Defense Curl", new DefenseCurl())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Defog", new MoveListEntry("Defog", new Defog())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Destiny Bond", new MoveListEntry("Destiny Bond", new DestinyBond())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Detect", new MoveListEntry("Detect", new Detect())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Dig", new MoveListEntry("Dig", new Dig())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Disable", new MoveListEntry("Disable", new Disable())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Discharge", new MoveListEntry("Discharge", new Discharge())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Dive", new MoveListEntry("Dive", new Dive())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Dizzy Punch", new MoveListEntry("Dizzy Punch", new DizzyPunch())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Doom Desire", new MoveListEntry("Doom Desire", new DoomDesire())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Double-edge", new MoveListEntry("Double-edge", new DoubleEdge())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Double Hit", new MoveListEntry("Double Hit", new DoubleHit())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Double Kick", new MoveListEntry("Double Kick", new DoubleKick())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Double Team", new MoveListEntry("Double Team", new DoubleTeam())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Doubleslap", new MoveListEntry("Doubleslap", new Doubleslap())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Draco Meteor", new MoveListEntry("Draco Meteor", new DracoMeteor())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Dragonbreath", new MoveListEntry("Dragonbreath", new Dragonbreath())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Dragon Dance", new MoveListEntry("Dragon Dance", new DragonDance())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Dragon Rage", new MoveListEntry("Dragon Rage", new DragonRage())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Dragon Rush", new MoveListEntry("Dragon Rush", new DragonRush())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Drain Punch", new MoveListEntry("Drain Punch", new DrainPunch())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Dream Eater", new MoveListEntry("Dream Eater", new DreamEater())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Dynamicpunch", new MoveListEntry("Dynamicpunch", new Dynamicpunch())));
	
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Earth Power", new MoveListEntry("Earth Power", new EarthPower())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Embargo", new MoveListEntry("Embargo", new Embargo())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Ember", new MoveListEntry("Ember", new Ember())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Encore", new MoveListEntry("Encore", new Encore())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Energy Ball", new MoveListEntry("Energy Ball", new EnergyBall())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Endure", new MoveListEntry("Endure", new Endure())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Endeavor", new MoveListEntry("Endeavor", new Endeavor())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Eruption", new MoveListEntry("Eruption", new Eruption())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Explosion", new MoveListEntry("Explosion", new Explosion())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Extrasensory", new MoveListEntry("Extrasensory", new Extrasensory())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Extremespeed", new MoveListEntry("Extremespeed", new Extremespeed())));

	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Facade", new MoveListEntry("Facade", new Facade())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Faint Attack", new MoveListEntry("Faint Attack", new FaintAttack())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("False Swipe", new MoveListEntry("False Swipe", new FalseSwipe())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Fake Out", new MoveListEntry("Fake Out", new FakeOut())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Fake Tears", new MoveListEntry("Fake Tears", new FakeTears())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Featherdance", new MoveListEntry("Featherdance", new Featherdance())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Feint", new MoveListEntry("Feint", new Feint())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Fire Blast", new MoveListEntry("Fire Blast", new FireBlast())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Fire Fang", new MoveListEntry("Fire Fang", new FireFang())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Fire Punch", new MoveListEntry("Fire Punch", new FirePunch())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Fire Spin", new MoveListEntry("Fire Spin", new FireSpin())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Fissure", new MoveListEntry("Fissure", new Fissure())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Flail", new MoveListEntry("Flail", new Flail())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Flamethrower", new MoveListEntry("Flamethrower", new Flamethrower())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Flame Wheel", new MoveListEntry("Flame Wheel", new FlameWheel())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Flare Blitz", new MoveListEntry("Flare Blitz", new FlareBlitz())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Flash", new MoveListEntry("Flash", new Flash())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Flash Cannon", new MoveListEntry("Flash Cannon", new FlashCannon())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Flatter", new MoveListEntry("Flatter", new Flatter())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Fly", new MoveListEntry("Fly", new Fly())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Focus Blast", new MoveListEntry("Focus Blast", new FocusBlast())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Focus Punch", new MoveListEntry("Focus Punch", new FocusPunch())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Follow Me", new MoveListEntry("Follow Me", new FollowMe())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Force Palm", new MoveListEntry("Force Palm", new ForcePalm())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Foresight", new MoveListEntry("Foresight", new Foresight())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Frenzy Plant", new MoveListEntry("Frenzy Plant", new FrenzyPlant())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Fury Attack", new MoveListEntry("Fury Attack", new FuryAttack())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Fury Cutter", new MoveListEntry("Fury Cutter", new FuryCutter())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Fury Swipes", new MoveListEntry("Fury Swipes", new FurySwipes())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Future Sight", new MoveListEntry("Future Sight", new FutureSight())));

	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Gastro Acid", new MoveListEntry("Gastro Acid", new GastroAcid())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Giga Drain", new MoveListEntry("Giga Drain", new GigaDrain())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Giga Impact", new MoveListEntry("Giga Impact", new GigaImpact())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Glare", new MoveListEntry("Glare", new Glare())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Grasswhistle", new MoveListEntry("Grasswhistle", new Grasswhistle())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Gravity", new MoveListEntry("Gravity", new Gravity())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Growl", new MoveListEntry("Growl", new Growl())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Growth", new MoveListEntry("Growth", new Growth())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Grudge", new MoveListEntry("Grudge", new Grudge())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Guard Swap", new MoveListEntry("Guard Swap", new GuardSwap())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Guillotine", new MoveListEntry("Guillotine", new Guillotine())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Gunk Shot", new MoveListEntry("Gunk Shot", new GunkShot())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Gyro Ball", new MoveListEntry("Gyro Ball", new GyroBall())));

	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Hail", new MoveListEntry("Hail", new Hail())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Hammer Arm", new MoveListEntry("Hammer Arm", new HammerArm())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Harden", new MoveListEntry("Harden", new Harden())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Haze", new MoveListEntry("Haze", new Haze())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Headbutt", new MoveListEntry("Headbutt", new Headbutt())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Head Smash", new MoveListEntry("Head Smash", new HeadSmash())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Heart Swap", new MoveListEntry("Heart Swap", new HeartSwap())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Heat Wave", new MoveListEntry("Heat Wave", new HeatWave())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Heal Bell", new MoveListEntry("Heal Bell", new HealBell())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Heal Block", new MoveListEntry("Heal Block", new HealBlock())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Heal Order", new MoveListEntry("Heal Order", new HealOrder())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Healing Wish", new MoveListEntry("Healing Wish", new HealingWish())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Helping Hand", new MoveListEntry("Helping Hand", new HelpingHand())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Hi Jump Kick", new MoveListEntry("Hi Jump Kick", new HiJumpKick())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Hidden Power", new MoveListEntry("Hidden Power", new HiddenPowerMove())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Horn Drill", new MoveListEntry("Horn Drill", new HornDrill())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Howl", new MoveListEntry("Howl", new Howl())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Hydro Cannon", new MoveListEntry("Hydro Cannon", new HydroCannon())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Hyper Beam", new MoveListEntry("Hyper Beam", new HyperBeam())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Hyper Fang", new MoveListEntry("Hyper Fang", new HyperFang())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Hypnosis", new MoveListEntry("Hypnosis", new Hypnosis())));

	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Ice Ball", new MoveListEntry("Ice Ball", new IceBall())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Ice Beam", new MoveListEntry("Ice Beam", new IceBeam())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Ice Fang", new MoveListEntry("Ice Fang", new IceFang())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Ice Punch", new MoveListEntry("Ice Punch", new IcePunch())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Ice Shard", new MoveListEntry("Ice Shard", new IceShard())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Icicle Spear", new MoveListEntry("Icicle Spear", new IcicleSpear())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Icy Wind", new MoveListEntry("Icy Wind", new IcyWind())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Imprison", new MoveListEntry("Imprison", new Imprison())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Ingrain", new MoveListEntry("Ingrain", new Ingrain())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Iron Defense", new MoveListEntry("Iron Defense", new IronDefense())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Iron Head", new MoveListEntry("Iron Head", new IronHead())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Iron Tail", new MoveListEntry("Iron Tail", new IronTail())));

	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Judgment", new MoveListEntry("Judgment", new Judgment())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Jump Kick", new MoveListEntry("Jump Kick", new JumpKick())));

	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Karate Chop", new MoveListEntry("Karate Chop", new KarateChop())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Kinesis", new MoveListEntry("Kinesis", new Kinesis())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Knock Off", new MoveListEntry("Knock Off", new KnockOff())));

	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Last Resort", new MoveListEntry("Last Resort", new LastResort())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Lava Plume", new MoveListEntry("Lava Plume", new LavaPlume())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Leaf Blade", new MoveListEntry("Leaf Blade", new LeafBlade())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Leaf Storm", new MoveListEntry("Leaf Storm", new LeafStorm())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Leech Life", new MoveListEntry("Leech Life", new LeechLife())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Leech Seed", new MoveListEntry("Leech Seed", new LeechSeed())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Leer", new MoveListEntry("Leer", new Leer())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Lick", new MoveListEntry("Lick", new Lick())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Light Screen", new MoveListEntry("Light Screen", new LightScreen())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Lovely Kiss", new MoveListEntry("Lovely Kiss", new LovelyKiss())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Low Kick", new MoveListEntry("Low Kick", new LowKick())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Lucky Chant", new MoveListEntry("Lucky Chant", new LuckyChant())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Lunar Dance", new MoveListEntry("Lunar Dance", new LunarDance())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Luster Purge", new MoveListEntry("Luster Purge", new LusterPurge())));
	
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Mach Punch", new MoveListEntry("Mach Punch", new MachPunch())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Magic Coat", new MoveListEntry("Magic Coat", new MagicCoat())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Magical Leaf", new MoveListEntry("Magical Leaf", new MagicalLeaf())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Magma Storm", new MoveListEntry("Magma Storm", new MagmaStorm())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Magnet Bomb", new MoveListEntry("Magnet Bomb", new MagnetBomb())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Magnet Rise", new MoveListEntry("Magnet Rise", new MagnetRise())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Magnitude", new MoveListEntry("Magnitude", new Magnitude())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Me First", new MoveListEntry("Me First", new MeFirst())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Mean Look", new MoveListEntry("Mean Look", new MeanLook())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Meditate", new MoveListEntry("Meditate", new Meditate())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Mega Drain", new MoveListEntry("Mega Drain", new MegaDrain())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Memento", new MoveListEntry("Memento", new Memento())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Metal Burst", new MoveListEntry("Metal Burst", new MetalBurst())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Metal Claw", new MoveListEntry("Metal Claw", new MetalClaw())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Metal Sound", new MoveListEntry("Metal Sound", new MetalSound())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Meteor Mash", new MoveListEntry("Meteor Mash", new MeteorMash())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Metronome", new MoveListEntry("Metronome", new Metronome())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Milk Drink", new MoveListEntry("Milk Drink", new MilkDrink())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Mimic", new MoveListEntry("Mimic", new Mimic())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Minimize", new MoveListEntry("Minimize", new Minimize())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Miracle Eye", new MoveListEntry("Miracle Eye", new MiracleEye())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Mirror Coat", new MoveListEntry("Mirror Coat", new MirrorCoat())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Mirror Shot", new MoveListEntry("Mirror Shot", new MirrorShot())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Mist", new MoveListEntry("Mist", new Mist())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Mist Ball", new MoveListEntry("Mist Ball", new MistBall())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Mud Bomb", new MoveListEntry("Mud Bomb", new MudBomb())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Mud Shot", new MoveListEntry("Mud Shot", new MudShot())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Mud-Slap", new MoveListEntry("Mud-Slap", new MudSlap())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Mud Sport", new MoveListEntry("Mud Sport", new MudSport())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Muddy Water", new MoveListEntry("Muddy Water", new MuddyWater())));

	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Nasty Plot", new MoveListEntry("Nasty Plot", new NastyPlot())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Nature Power", new MoveListEntry("Nature Power", new NaturePower())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Needle Arm", new MoveListEntry("Needle Arm", new NeedleArm())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Nightmare", new MoveListEntry("Nightmare", new Nightmare())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Night Shade", new MoveListEntry("Night Shade", new NightShade())));

	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Octazooka", new MoveListEntry("Octazooka", new Octazooka())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Ominous Wind", new MoveListEntry("Ominous Wind", new OminousWind())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Outrage", new MoveListEntry("Outrage", new Outrage())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Overheat", new MoveListEntry("Overheat", new Overheat())));

	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Pain Split", new MoveListEntry("Pain Split", new PainSplit())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Pay Day", new MoveListEntry("Pay Day", new PayDay())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Perish Song", new MoveListEntry("Perish Song", new PerishSong())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Petal Dance", new MoveListEntry("Petal Dance", new PetalDance())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Pin Missile", new MoveListEntry("Pin Missile", new PinMissile())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Pluck", new MoveListEntry("Pluck", new Pluck())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Poison Fang", new MoveListEntry("Poison Fang", new PoisonFang())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Poison Gas", new MoveListEntry("Poison Gas", new PoisonGas())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Poison Jab", new MoveListEntry("Poison Jab", new PoisonJab())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Poison String", new MoveListEntry("Poison String", new PoisonString())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Poison Tail", new MoveListEntry("Poison Tail", new PoisonTail())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Poisonpowder", new MoveListEntry("Poisonpowder", new Poisonpowder())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Powder Snow", new MoveListEntry("Powder Snow", new PowderSnow())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Power Swap", new MoveListEntry("Power Swap", new PowerSwap())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Power Trick", new MoveListEntry("Power Trick", new PowerTrick())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Present", new MoveListEntry("Present", new Present())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Protect", new MoveListEntry("Protect", new Protect())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Psybeam", new MoveListEntry("Psybeam", new Psybeam())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Psych Up", new MoveListEntry("Psych Up", new PsychUp())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Psychic", new MoveListEntry("Psychic", new Psychic())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Psycho Boost", new MoveListEntry("Psycho Boost", new PsychoBoost())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Psycho Cut", new MoveListEntry("Psycho Cut", new PsychoCut())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Psycho Shift", new MoveListEntry("Psycho Shift", new PsychoShift())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Psywave", new MoveListEntry("Psywave", new Psywave())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Punishment", new MoveListEntry("Punishment", new Punishment())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Pursuit", new MoveListEntry("Pursuit", new Pursuit())));

	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Quick Attack", new MoveListEntry("Quick Attack", new QuickAttack())));

	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Rage", new MoveListEntry("Rage", new Rage())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Rain Dance", new MoveListEntry("Rain Dance", new RainDance())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Rapid Spin", new MoveListEntry("Rapid Spin", new RapidSpin())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Razor Leaf", new MoveListEntry("Razor Leaf", new RazorLeaf())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Razor Wind", new MoveListEntry("Razor Wind", new RazorWind())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Recover", new MoveListEntry("Recover", new Recover())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Reflect", new MoveListEntry("Reflect", new Reflect())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Refresh", new MoveListEntry("Refresh", new Refresh())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Rest", new MoveListEntry("Rest", new Rest())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Revenge", new MoveListEntry("Revenge", new Revenge())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Reversal", new MoveListEntry("Reversal", new Reversal())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Roar", new MoveListEntry("Roar", new Roar())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Roar of Time", new MoveListEntry("Roar of Time", new RoarOfTime())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Rock Blast", new MoveListEntry("Rock Blast", new RockBlast())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Rock Climb", new MoveListEntry("Rock Climb", new RockClimb())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Rock Polish", new MoveListEntry("Rock Polish", new RockPolish())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Rock Slide", new MoveListEntry("Rock Slide", new RockSlide())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Rock Smash", new MoveListEntry("Rock Smash", new RockSmash())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Rock Tomb", new MoveListEntry("Rock Tomb", new RockTomb())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Rock Smash", new MoveListEntry("Rock Wrecker", new RockSmash())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Role Play", new MoveListEntry("Role Play", new RolePlay())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Rollout", new MoveListEntry("Rollout", new Rollout())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Roost", new MoveListEntry("Roost", new Roost())));

	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Safeguard", new MoveListEntry("Safeguard", new Safeguard())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Sand-Attack", new MoveListEntry("Sand-Attack", new SandAttack())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Sand Tomb", new MoveListEntry("Sand Tomb", new SandTomb())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Sandstorm", new MoveListEntry("Sandstorm", new Sandstorm())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Sacred Fire", new MoveListEntry("Sacred Fire", new SacredFire())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Scary Face", new MoveListEntry("Scary Face", new ScaryFace())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Screech", new MoveListEntry("Screech", new Screech())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Secret Power", new MoveListEntry("Secret Power", new SecretPower())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Seed Flare", new MoveListEntry("Seed Flare", new SeedFlare())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Seismic Toss", new MoveListEntry("Seismic Toss", new SeismicToss())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Shadow Ball", new MoveListEntry("Shadow Ball", new ShadowBall())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Shadow Claw", new MoveListEntry("Shadow Claw", new ShadowClaw())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Shadow Force", new MoveListEntry("Shadow Force", new ShadowForce())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Shadow Punch", new MoveListEntry("Shadow Punch", new ShadowPunch())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Shadow Sneak", new MoveListEntry("Shadow Sneak", new ShadowSneak())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Sharpen", new MoveListEntry("Sharpen", new Sharpen())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Sheer Cold", new MoveListEntry("Sheer Cold", new SheerCold())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Shock Wave", new MoveListEntry("Shock Wave", new ShockWave())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Signal Beam", new MoveListEntry("Signal Beam", new SignalBeam())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Silver Wind", new MoveListEntry("Silver Wind", new SilverWind())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Sing", new MoveListEntry("Sing", new Sing())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Skill Swap", new MoveListEntry("Skill Swap", new SkillSwap())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Skull Bash", new MoveListEntry("Skull Bash", new SkullBash())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Sky Attack", new MoveListEntry("Sky Attack", new SkyAttack())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Slack Off", new MoveListEntry("Slack Off", new SlackOff())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Slash", new MoveListEntry("Slash", new Slash())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Sleep Talk", new MoveListEntry("Sleep Talk", new SleepTalk())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Sleep Powder", new MoveListEntry("Sleep Powder", new SleepPowder())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Sludge", new MoveListEntry("Sludge", new Sludge())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Sludge Bomb", new MoveListEntry("Sludge Bomb", new SludgeBomb())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Smellingsalt", new MoveListEntry("Smellingsalt", new Smellingsalt())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Smog", new MoveListEntry("Smog", new Smog())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Smokescreen", new MoveListEntry("Smokescreen", new Smokescreen())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Snore", new MoveListEntry("Snore", new Snore())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Softboiled", new MoveListEntry("Softboiled", new Softboiled())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Solarbeam", new MoveListEntry("Solarbeam", new Solarbeam())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Sonicboom", new MoveListEntry("Sonicboom", new Sonicboom())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Spacial Rend", new MoveListEntry("Spacial Rend", new SpacialRend())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Spark", new MoveListEntry("Spark", new Spark())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Spider Web", new MoveListEntry("Spider Web", new SpiderWeb())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Spike Cannon", new MoveListEntry("Spike Cannon", new SpikeCannon())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Spikes", new MoveListEntry("Spikes", new Spikes())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Spit Up", new MoveListEntry("Spit Up", new SpitUp())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Spite", new MoveListEntry("Spite", new Spite())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Splash", new MoveListEntry("Splash", new Splash())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Spore", new MoveListEntry("Spore", new Spore())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Stealth Rock", new MoveListEntry("Stealth Rock", new StealthRock())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Steel Wing", new MoveListEntry("Steel Wing", new SteelWing())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Stockpile", new MoveListEntry("Stockpile", new Stockpile())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Stomp", new MoveListEntry("Stomp", new Stomp())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Stone Edge", new MoveListEntry("StoneEdge", new StoneEdge())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("String Shot", new MoveListEntry("String Shot", new StringShot())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Struggle", new MoveListEntry("Struggle", new Struggle())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Stun Spore", new MoveListEntry("Stun Spore", new StunSpore())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Submission", new MoveListEntry("Submission", new Submission())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Substitute", new MoveListEntry("Substitute", new Substitute())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Sucker Punch", new MoveListEntry("Sucker Punch", new SuckerPunch())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Sunny Day", new MoveListEntry("Sunny Day", new SunnyDay())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Super Fang", new MoveListEntry("Super Fang", new SuperFang())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Superpower", new MoveListEntry("Superpower", new Superpower())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Supersonic", new MoveListEntry("Supersonic", new Supersonic())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Swagger", new MoveListEntry("Swagger", new Swagger())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Swallow", new MoveListEntry("Swallow", new Swagger())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Sweet Kiss", new MoveListEntry("Sweet Kiss", new SweetKiss())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Sweet Scent", new MoveListEntry("Sweet Scent", new SweetScent())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Swift", new MoveListEntry("Swift", new Swift())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Switcheroo", new MoveListEntry("Switcheroo", new Switcheroo())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Swords Dance", new MoveListEntry("Swords Dance", new SwordsDance())));

	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Taunt", new MoveListEntry("Taunt", new Taunt())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Tail Glow", new MoveListEntry("Tail Glow", new TailGlow())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Tail Whip", new MoveListEntry("Tail Whip", new TailWhip())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Tailwind", new MoveListEntry("Tailwind", new Tailwind())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Take Down", new MoveListEntry("Take Down", new TakeDown())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Teeter Dance", new MoveListEntry("Teeter Dance", new TeeterDance())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Teleport", new MoveListEntry("Teleport", new Teleport())));
	/* Thief - Check bottom of method */
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Thrash", new MoveListEntry("Thrash", new Thrash())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Thunder", new MoveListEntry("Thunder", new Thunder())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Thunder Fang", new MoveListEntry("Thunder Fang", new ThunderFang())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Thunder Wave", new MoveListEntry("Thunder Wave", new ThunderWave())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Thunderbolt", new MoveListEntry("Thunderbolt", new Thunderbolt())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Thunderpunch", new MoveListEntry("Thunderpunch", new Thunderpunch())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Thundershock", new MoveListEntry("Thundershock", new Thundershock())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Tickle", new MoveListEntry("Tickle", new Tickle())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Torment", new MoveListEntry("Torment", new Torment())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Toxic", new MoveListEntry("Toxic", new Toxic())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Toxic Spikes", new MoveListEntry("Toxic Spikes", new ToxicSpikes())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Tri Attack", new MoveListEntry("Tri Attack", new TriAttack())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Trick Room", new MoveListEntry("Trick Room", new TrickRoom())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Triple Kick", new MoveListEntry("Triple Kick", new TripleKick())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Trick", new MoveListEntry("Trick", new Trick())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Trump Card", new MoveListEntry("Trump Card", new TrumpCard())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Twineedle", new MoveListEntry("Twineedle", new Twineedle())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Twister", new MoveListEntry("Twister", new Twister())));

	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Uproar", new MoveListEntry("Uproar", new Uproar())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("U-turn", new MoveListEntry("U-turn", new UTurn())));

	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Vacuum Wave", new MoveListEntry("Vacuum Wave", new VacuumWave())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Vital Throw", new MoveListEntry("Vital Throw", new VitalThrow())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Volt Tackle", new MoveListEntry("Volt Tackle", new VoltTackle())));

	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Wake-Up Slap", new MoveListEntry("Wake-Up Slap", new WakeupSlap())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Water Pulse", new MoveListEntry("Water Pulse", new WaterPulse())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Water Spout", new MoveListEntry("Water Spout", new WaterSpout())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Water Sport", new MoveListEntry("Water Sport", new WaterSport())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Whirlpool", new MoveListEntry("Whirlpool", new Whirlpool())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Will-O-Wisp", new MoveListEntry("Will-O-Wisp", new Willowisp())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Withdraw", new MoveListEntry("Withdraw", new Withdraw())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("WoodHammer", new MoveListEntry("WoodHammer", new WoodHammer())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Worry Seed", new MoveListEntry("Worry Seed", new WorrySeed())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Wrap", new MoveListEntry("Wrap", new Wrap())));

	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Yawn", new MoveListEntry("Yawn", new Yawn())));

	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Zap Cannon", new MoveListEntry("Zap Cannon", new ZapCannon())));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Zen Headbutt", new MoveListEntry("Zen Headbutt", new ZenHeadbutt())));

	statusEffectList_t a1;
	a1.push_back(new PercentEffect(0.5, false, -1, ""));
	boolArray_t a2(1, true);
	doubleArray_t a3(1, 1.0);	
	StatusMove* healing = new StatusMove(PokemonType::T_NORMAL, 0, 10, 5, a1, a2, a3);
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Moonlight", new MoveListEntry("Moonlight", healing)));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Morning Sun", new MoveListEntry("Morning Sun", healing->clone() )));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Synthesis", new MoveListEntry("Synthesis", healing->clone() )));

	PokemonMove* healBell = new HealBell();
	PokemonMove* arometherapy = healBell->clone();
	arometherapy->setType(PokemonType::T_GRASS);
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Arometherapy", new MoveListEntry("Aromatherapy", arometherapy)));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Heal Bell", new MoveListEntry("Heal Bell", healBell)));

	PokemonMove* foresight = new Foresight();
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Forsight", new MoveListEntry("Forsight", foresight)));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Odor Sleuth", new MoveListEntry("Odor Sleuth", foresight->clone())));

	PokemonMove* explosion = new Explosion();
	PokemonMove* selfDestruct = explosion->clone();
	selfDestruct->setPower(200);
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Explosion", new MoveListEntry("Explosion", explosion)));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Selfdestruct", new MoveListEntry("Selfdestruct", selfDestruct)));

	PokemonMove* thief = new Thief();
	PokemonMove* covet = thief->clone();
	covet->setType(PokemonType::T_NORMAL);
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Thief", new MoveListEntry("Thief", thief)));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Covet", new MoveListEntry("Covet", covet)));

	PokemonMove* roar = new Roar();
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Roar", new MoveListEntry("Roar", roar)));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Whirlwind", new MoveListEntry("Whirlwind", (PokemonMove*)roar->clone())));

	PokemonMove* crushGrip = new CrushGrip();
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Crush Grip", new MoveListEntry("Crush Grip", crushGrip)));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Wring Out", new MoveListEntry("Wring Out", (PokemonMove*)crushGrip->clone())));

	DamageListenerMove* payback = new Payback();
	DamageListenerMove* avalance = (DamageListenerMove*)payback->clone();
	avalance->setType(PokemonType::T_ICE);
	avalance->setPower(60);
	avalance->setPriority(-3);
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Payback", new MoveListEntry("Payback", payback)));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Avalance", new MoveListEntry("Avalance", avalance)));

	PokemonMove* mirrorMove = new MirrorMove();
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Mirror Move", new MoveListEntry("Mirror Move", payback)));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Copycat", new MoveListEntry("Copycat", mirrorMove->clone())));

	PokemonMove* lockonMove = new LockOn();
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Lock-on", new MoveListEntry("Lock-on", lockonMove)));
	m_moves.insert(std::make_pair<std::string, MoveListEntry*>("Mind Reader", new MoveListEntry("Mind Reader", lockonMove->clone())));
}

void MoveList::initNamesMoves()
{

}