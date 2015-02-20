#include "pokemon.h"
#include "pokemonwrapper.h"

#include "statuseffect.h"
#include "sleepeffect.h"
#include "substituteeffect.h"

#include "intrinsicability.h"
#include "pokemonparty.h"

Pokemon::Pokemon(BattleMechanics* mech, PokemonSpecies* species, PokemonNature* nature, std::string ability, std::string item, 
				 int gender, int level, intArray_t ivs, moveList_t moves, intArray_t ppUps, bool validate)
{

}

Pokemon::~Pokemon()
{

}

double Pokemon::getEffectiveness(statusEffectList_t statuses, PokemonType *move, PokemonType *pokemon, bool enemy)
{
	double expected = move->getMultiplier(pokemon);
	statusEffectList_t::iterator end = statuses.end();
	for(statusEffectList_t::iterator it = statuses.begin(); it != end; ++it)
	{
		StatusEffect* eff = (StatusEffect*)(*it);
		if(eff->isActive() && eff->isEffectivenessTransformer(enemy)) {
			double actual = eff->getEffectiveness(move, pokemon, enemy);
			if(actual != expected) { return actual; }
		}
	}

	return expected;
}

double Pokemon::getEffectiveness(fieldEffects_t statuses, PokemonType *move, PokemonType *pokemon, bool enemy)
{
	double expected = move->getMultiplier(pokemon);
	fieldEffects_t::iterator end = statuses.end();
	for(fieldEffects_t::iterator it = statuses.begin(); it != end; ++it)
	{
		StatusEffect* eff = (StatusEffect*)(*it);
		if(eff->isActive() && eff->isEffectivenessTransformer(enemy)) {
			double actual = eff->getEffectiveness(move, pokemon, enemy);
			if(actual != expected) { return actual; }
		}
	}

	return expected;
}

StatusEffect* Pokemon::addStatus( Pokemon* source, StatusEffect* eff )
{
	if(isRemoved() || m_fainted) {
		return NULL;
	}

	statusEffectList_t::iterator it;
	for(it = m_statuses.begin(); it != m_statuses.end(); ++it)
	{
		StatusEffect* j = (*it);
		if(!j->isRemovable() && (eff->equals(j) || eff->isExclusiveWith(j)) ) {
			return NULL;
		}
	}

	StatusEffect *applied = eff->clone();
	applied->activate();
	applied->setInducer(source);
	if((m_field != NULL) && !allowStatus(applied, source)) {
		return NULL;
	}

	if(applied->apply(this)) {
		m_statuses.push_back(applied);
		if(m_field != NULL) {
			//TODO: m_field->informStatusApplied(this, applied);
		}

		informStatusListeners(source, applied, true);
	}

	return applied;
}

void Pokemon::removeStatus( StatusEffect *eff )
{
	statusEffectList_t::iterator it;
	for(it = m_statuses.begin(); it != m_statuses.end(); ++it)
	{
		StatusEffect* effect = (*it);
		if(effect == eff) {
			unapplyEffect(eff);
			return;
		}
	}
}

void Pokemon::removeStatus( int lock )
{
	statusEffectList_t::iterator it;
	for(it = m_statuses.begin(); it != m_statuses.end(); ++it)
	{
		StatusEffect* effect = (*it);
		if(effect->getLock() == lock && !effect->isRemovable()) {
			unapplyEffect(effect);
		}
	}
}

/*
template< class C >
void Pokemon::removeStatus()
{
	statusEffectList_t list = m_statuses;
	statusEffectList_t::iterator it = list.begin();
	for(; it != list.end(); ++it) {
		StatusEffect *effect = (*it);
		if(!effect->isRemovable() && isAssignableFrom< C* >(effect)) {
			unapplyEffect(effect);
		}
	}
}*/



bool Pokemon::allowStatus( StatusEffect* eff, Pokemon* source )
{
	statusEffectList_t::iterator it;
	for(it = m_statuses.begin(); it != m_statuses.end(); ++it)
	{
		StatusEffect *clause = (*it);
		if(clause == NULL || !clause->isActive())
			continue;

		if(!clause->allowsStatus(eff, source, this))
			return false;
	}

	return true;
}

void Pokemon::informStatusListeners( Pokemon* source, StatusEffect* eff, bool applied )
{
	statusEffectList_t::iterator it;
	for(it = m_statuses.begin(); it != m_statuses.end(); ++it)
	{
		StatusEffect* j = (*it);
		if(j->isActive() && j->isStatusListener())
		{
			if(applied) {
				j->informStatusApplied(source, this, eff);
			} else {
				j->informStatusRemoved(this, eff);
			}
		}
	}
}

void Pokemon::switchIn()
{
	m_lastMove = NULL;
	m_firstTurn = true;

	moveList_t::iterator it;
	for(it = m_move.begin(); it != m_move.end(); ++it)
	{
		MoveListEntry *entry;
		if(entry = (*it)) {
			PokemonMove *move = NULL;
			if(move = entry->getMove()) {
				move->switchIn(this);
			}
		}
	}

	statusEffectList_t::iterator statIt;
	for(statIt = m_statuses.begin(); statIt != m_statuses.end(); ++statIt) {
		StatusEffect *eff = NULL;
		if(eff = (*statIt)) {
			eff->switchIn(this);
		}
	}

	getMaster()->getPokemonParty()->setActivePokemon(this);
}

void Pokemon::switchOut()
{
	statusEffectList_t::iterator it = m_statuses.begin();
	while(it != m_statuses.end())
	{
		StatusEffect* effect = (*it);
		if(effect->isActive() && effect->switchOut(this)) {
			unapplyEffect(effect, false);
			it = m_statuses.erase(it);

			continue;
		}

		++it;
	}

	setAbility(m_originalAbility, true);
	synchroniseStatuses();
}

void Pokemon::attachToField( BattleField* field )
{
	m_field = field;

	if(m_abilityName.length() != 0) {
		m_originalAbility = IntrinsicAbility::getInstance(m_abilityName);
		if(m_originalAbility != NULL) {
			m_ability = dynamic_cast<IntrinsicAbility*>(addStatus(this, m_originalAbility));
		}
	}

	//TODO: Hold Item
}

void Pokemon::unapplyEffect( StatusEffect *eff, bool disable /* = true */ )
{
	if(eff->isActive()) {
		eff->unapply(this);
	}

	if(disable) {
		eff->disable();
	}

	if(m_field != NULL) {
		m_field->informStatusRemoved(this, eff);
	}

	informStatusListeners(NULL, eff, false);
}

void Pokemon::setAbility( IntrinsicAbility *abl, bool ignoreTransferability )
{
	removeStatus(m_ability);

	if(abl != NULL) {
		m_abilityName = abl->getName();
		if(ignoreTransferability || abl->isEffectTransferrable()) {
			m_ability = dynamic_cast<IntrinsicAbility*>(addStatus(this, abl));
		}
		else {
			m_ability = NULL;
		}
	}
	else {
		m_abilityName = "";
	}
}

void Pokemon::synchroniseStatuses()
{
	statusEffectList_t::iterator it = m_statuses.begin();
	while(it != m_statuses.end()) 
	{
		StatusEffect* effect = (*it);
		if(effect->isRemovable()) {
			it = m_statuses.erase(it);
			continue;
		}

		++it;
	}
}

MoveListEntry* Pokemon::getMove( int32_t i )
{
	if(i == -1) {
		return BattleField::getStruggle();
	}
	else if(i < -1 || i > m_move.size() || m_move[i] == NULL) {
		return NULL;
	}

	return m_move[i];
}

void Pokemon::executeTurn( BattleTurn *turn )
{
	statusEffectList_t::iterator it = m_statuses.begin();
	for(; it != m_statuses.end(); ++it) {
		StatusEffect *j = (StatusEffect *)(*it);
		if(j == NULL || !j->isActive()) {
			continue;
		}

		j->executeTurn(this, turn);
	}
}

bool Pokemon::isImmobilised( SleepEffect *exception )
{
	std::sort(m_statuses.begin(), m_statuses.end(), PokemonWrapper::compareStatuses);

	statusEffectList_t::iterator it = m_statuses.begin();
	for(; it != m_statuses.end(); ++it)
	{
		StatusEffect *eff = (*it);
		if(eff->isActive() && eff->immobilises(this)) {
			if(exception == NULL || isAssignableFrom<SleepEffect*>(eff)) {
				m_lastMove = NULL;
				m_firstTurn = false;

				return true;
			}
		}
	}

	return false;
}

int Pokemon::useMove( int32_t move, Pokemon *target )
{
	if(move == -1) {
		MoveListEntry *pmove = BattleField::getStruggle();
		int ret = useMove(pmove, target);
		m_lastMove = pmove;
		m_firstTurn = false;

		return ret;
	}

	if((move >= m_move.size()) || (m_move[move] == NULL)) {
		return 0;
	}
	if(m_pp[move] == 0) { 
		return 0;
	}

	MoveListEntry *entry = m_move[move];
	PokemonMove *pmove = entry->getMove();

	const int cost = (target->hasAbility("Pressure") && pmove->isAttack()) ? 2 : 1;
	m_pp[move] -= cost;
	if(m_pp[move] < 0) m_pp[move] = 0;

	int ret = useMove(entry, target);
	m_lastMove = entry;
	m_firstTurn = false;

	return ret;
}

int Pokemon::useMove( MoveListEntry *move, Pokemon *target )
{
	PokemonMove *pmove = move->getMove();
	move = getTransformedMove(move, false);

	if(move) {
		if(target != this) {
			if((move = target->getTransformedMove(move, true)) == NULL) {
				return 0;
			}
		}

		m_field->informUseMove(this, move->getName());
		int hp = target->getHealth();
		
		useMove(pmove, target);
		int damage = hp - target->getHealth();
		if(damage > 0) {
			target->informDamaged(this, move, damage);
		}

		return damage;
	}

	return 0;
}

int Pokemon::useMove( PokemonMove *move, Pokemon *target )
{
	if(!move->attemptHit(m_mech, this, target)) {
		return 0;
	}

	return move->use(m_mech, this, target);
}

MoveListEntry* Pokemon::getTransformedMove( MoveListEntry* move, bool enemy )
{
	statusEffectList_t::iterator it = m_statuses.begin();
	for (; it != m_statuses.end(); ++it)
	{
		StatusEffect *eff = (*it);
		if(eff->isActive() && eff->isMoveTransformer(enemy)) {
			move = eff->getMove(this, move->clone(), enemy);
			if(!move) {
				return NULL;
			}
		}
	}

	return move;
}

void Pokemon::informDamaged( Pokemon* source, MoveListEntry* move, int damage )
{
	statusEffectList_t list = m_statuses;
	for(statusEffectList_t::iterator it = list.begin(); it != list.end(); ++it) {
		StatusEffect *eff = (*it);
		if(eff->isActive() && eff->isListener()) {
			eff->informDamaged(source, this, move, damage);
		}
	}
}

bool Pokemon::hasAbility( const std::string name )
{
	if(m_ability == NULL) {
		return false;
	}

	return (m_ability->isActive() && m_ability->getName() == name);
}

void Pokemon::beginStatusTicks()
{
	statusEffectList_t list = m_statuses;
	statusEffectList_t::iterator it = list.begin();
	for(; it != list.end(); ++it) {
		StatusEffect *effect = (*it);
		effect->beginTick();
	}
}

statusEffectList_t Pokemon::getStatusesByTier( int tier )
{
	statusEffectList_t ret;
	statusEffectList_t::iterator it = m_statuses.begin();
	for(; it != m_statuses.end(); ++it) {
		StatusEffect *effect = (*it);
		if(effect->isActive() && (effect->getTier() == tier)) {
			ret.push_back(effect);
		}
	}

	return ret;
}

void Pokemon::changeHealth( int hp, bool throughSubstitude /*= false*/ )
{
	if(m_fainted)
		return;

	std::string message;

	if(!hasSubstitute() || throughSubstitude || (hp > 0)){
		if(throughSubstitude && (hp < 0) && hasAbility("Magic Guard"))
			return;

		int max = m_stat[S_HP];
		int display = hp;
		int result  = m_hp + hp;

		//TODO: Uncomment -> PendaticDamageClause
		/*
		if(hasEffect<PendanticDamageClause>()) {
			if(result > max) {
				display = max - m_hp;
			} else if(result < 0) {
				display = -m_hp;
			}
		}*/


		m_field->informPokemonHealthChanged(this, display);
		if((result <= 0) && !throughSubstitude)
		{
			bool live = false;

			//TODO: Uncomment -> EndureanceEffect and hasItem(), setItem()
			/*
			if(hasEffect<EnduranceEffect>()) {
				message = boost::str(boost::format("%s endured the attack!") % getName());
				m_field->showMessage(message);
				live = true;
			}
			else if(m_hp == max && hasItem("Focus Sash")) {
				message = boost::str(boost::format("%s hung on using its Focus Sash!") % getName());
				m_field->showMessage(message);				
				live = true;
				//TODO: setItem(NULL);
			}
			else if(hasItem("Focus Band")) {
				int rnd = random_range(0,10);
				if(rnd <= 1) {
					message = boost::str(boost::format("%s hung on using its Focus Band!"));
					m_field->showMessage(message);
					live = true;
				}
			}*/


			if(live) {
				hp = -m_hp + 1;
			}
		}

		m_hp += hp;
		if(m_hp <= 0) {
			faint();
		} else if(m_hp > max) {
			m_hp = max;
		}
	}
	else
	{
		m_substitute += hp;
		message = boost::str(boost::format("The substitute took damage for %1%!") % getName());
		m_field->showMessage(message);

		if(m_substitute <= 0) {
			message = boost::str(boost::format("%1%'s substitude faded!") % getName());
			m_field->showMessage(message);
			m_substitute = 0;
			removeStatus<SubstituteEffect>();
		}
	}
}

void Pokemon::faint()
{
	m_hp = 0;
	m_fainted = true;
	if(m_field != NULL) {
		m_field->informPokemonFainted(getMaster(), this);
		m_field->checkBattleEnd(getMaster());
	}
}

StatMultiplier* Pokemon::getMultiplier( int i )
{
	if(i >= 0 && i < 6)
		return m_multiplier.at(i);

	if(i == S_ACCURACY)
		return m_accuracy;

	if(i == S_EVASION)
		return m_evasion;

	return NULL;
}

Pokemon* Pokemon::getOpponent()
{
	if(m_field == NULL) {
		return NULL;
	}

	Creature* opponent = m_field->getOpponent(getMaster());
	if(opponent == NULL) {
		return NULL;
	}

	return opponent->getPokemonParty()->getActivePokemon();
}

StatusEffect* Pokemon::getEffect( int lock )
{
	statusEffectList_t::iterator it = m_statuses.begin();
	for(; it != m_statuses.end(); ++it) {
		StatusEffect *eff = (*it);
		if(!eff || !eff->isActive()) {
			continue;
		}
		if(eff->getLock() == lock) {
			return eff;
		}
	}

	return NULL;
}

IntrinsicAbility* Pokemon::getAbility()
{
	if(m_ability == NULL && m_abilityName.size() > 0) {
		return IntrinsicAbility::getInstance(m_abilityName);
	}

	return m_ability;
}

pokemonList_t Pokemon::getTeammates()
{
	//TODO: Pokemon::getTeammates implementation
	pokemonList_t dummy;
	return dummy;
}

statusEffectList_t Pokemon::getNormalStatuses( int lock )
{
	statusEffectList_t ret;

	statusEffectList_t::iterator it = m_statuses.begin();
	statusEffectList_t::iterator end = m_statuses.end();
	for(; it != end; ++it) 
	{
		StatusEffect* effect = (*it);
		if(!effect->isActive()) continue;

		// NOTE: Hold item is a subclass of IntrinsicAbility
		if(!isAssignableFrom<IntrinsicAbility*>(effect)) {
			int effLock = effect->getLock();
			if(effLock == 0 || effLock == lock) {
				ret.push_back(effect);
			}
		}
	}

	return ret;
}

bool Pokemon::createSubstitude()
{
	if(hasSubstitute()) { return false; }

	int quarter = m_stat[S_HP] / 4;
	if(quarter >= m_hp) {
		return false;
	}

	changeHealth(-quarter);
	m_substitute = quarter;

	return true;
}