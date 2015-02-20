#include "multiplestatchangeeffect.h"

#include "pokemon.h"
#include "statchangeeffect.h"

bool MultipleStatChangeEffect::apply( Pokemon* p )
{
	for(int i = 0; i < m_stats.size(); ++i) {
		p->addStatus(getInducer(), new StatChangeEffect(m_stats[i], true, 1));
	}

	return false;
}