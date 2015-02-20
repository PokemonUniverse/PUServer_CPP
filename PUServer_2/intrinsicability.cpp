#include "intrinsicability.h"
#include "intrinsicabilitydata.h"
#include "criticaltypeability.h"
#include "compoundeyesability.h"

IntrinsicAbilityMap_t IntrinsicAbility::m_map;
bool IntrinsicAbility::m_isLoaded = false;

void IntrinsicAbility::load()
{
	new IntrinsicAbility("Battle Armor");
	new IntrinsicAbility("Chlorophyll");
	new IntrinsicAbility("Rock Head");
	new IntrinsicAbility("Clear Body");
	new IntrinsicAbility("White Smoke");

	new IA_ColorChange();
	new CriticalTypeAbility("Blaze", PokemonType::T_FIRE);
	new CriticalTypeAbility("Overgrow", PokemonType::T_GRASS);
	new CriticalTypeAbility("Swarm", PokemonType::T_BUG);
	new CriticalTypeAbility("Torrent", PokemonType::T_WATER);

	new CompoundEyesAbility();

	new IntrinsicAbility("Damp");
}