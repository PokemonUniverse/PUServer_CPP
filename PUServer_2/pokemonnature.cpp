#include "pokemonnature.h"

naturesList_t PokemonNature::m_natures;

const PokemonNature* PokemonNature::N_LONELY = new PokemonNature("Lonely", S_ATTACK, S_DEFENCE);
const PokemonNature* PokemonNature::N_BRAVE = new PokemonNature("Brave", S_ATTACK, S_SPEED);
const PokemonNature* PokemonNature::N_ADAMANT = new PokemonNature("Adamant", S_ATTACK, S_SPATTACK);
const PokemonNature* PokemonNature::N_NAUGHTY = new PokemonNature("Naughty", S_ATTACK, S_SPDEFENCE);
const PokemonNature* PokemonNature::N_BOLD = new PokemonNature("Bold", S_DEFENCE, S_ATTACK);
const PokemonNature* PokemonNature::N_RELAXED = new PokemonNature("Relaxed", S_DEFENCE, S_SPEED);
const PokemonNature* PokemonNature::N_IMPISH = new PokemonNature("Impish", S_DEFENCE, S_SPATTACK);
const PokemonNature* PokemonNature::N_LAX = new PokemonNature("Lax", S_DEFENCE, S_SPDEFENCE);
const PokemonNature* PokemonNature::N_TIMID = new PokemonNature("Timid", S_SPEED, S_ATTACK);
const PokemonNature* PokemonNature::N_HASTY = new PokemonNature("Hasty", S_SPEED, S_DEFENCE);
const PokemonNature* PokemonNature::N_JOLLY = new PokemonNature("Jolly", S_SPEED, S_SPATTACK);
const PokemonNature* PokemonNature::N_NAIVE = new PokemonNature("Naive", S_SPEED, S_SPDEFENCE);
const PokemonNature* PokemonNature::N_MODEST = new PokemonNature("Modest", S_SPATTACK, S_ATTACK);
const PokemonNature* PokemonNature::N_MILD = new PokemonNature("Mild", S_SPATTACK, S_DEFENCE);
const PokemonNature* PokemonNature::N_QUIET = new PokemonNature("Quiet", S_SPATTACK, S_SPEED);
const PokemonNature* PokemonNature::N_RASH = new PokemonNature("Rash", S_SPATTACK, S_SPDEFENCE);
const PokemonNature* PokemonNature::N_CALM = new PokemonNature("Calm", S_SPDEFENCE, S_ATTACK);
const PokemonNature* PokemonNature::N_GENTLE = new PokemonNature("Gentle", S_SPDEFENCE, S_DEFENCE);
const PokemonNature* PokemonNature::N_SASSY = new PokemonNature("Sassy", S_SPDEFENCE, S_SPEED);
const PokemonNature* PokemonNature::N_CAREFUL = new PokemonNature("Careful", S_SPDEFENCE, S_SPATTACK);
const PokemonNature* PokemonNature::N_QUIRKY = new PokemonNature("Quirky", S_NONE, S_NONE);
const PokemonNature* PokemonNature::N_HARDY = new PokemonNature("Hardy", S_NONE, S_NONE);
const PokemonNature* PokemonNature::N_SERIOUS = new PokemonNature("Serious", S_NONE, S_NONE);
const PokemonNature* PokemonNature::N_BASHFUL = new PokemonNature("Bashful", S_NONE, S_NONE);
const PokemonNature* PokemonNature::N_DOCILE = new PokemonNature("Docile", S_NONE, S_NONE);