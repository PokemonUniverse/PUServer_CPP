#ifndef __STATMULTIPLIER_H_
#define __STATMULTIPLIER_H_

#include "definitions.h"

/**
* This class represents a stat multiplier. Such a multiplier might be instated
* because of a move, but hold items and intrinsic abilities do not use this
* class. They might later, but they don't now.
*/	
class StatMultiplier
{
public:
	StatMultiplier(bool bAccuracy = false) {
		m_position = 6;
		m_secondary = 1.0;

		if(bAccuracy) {
			m_multipliers.insert(m_multipliers.begin(), m_acc, m_acc+13);
		}
		else {
			m_multipliers.insert(m_multipliers.begin(), m_stats, m_stats+13);
		}
	}

	~StatMultiplier() {};

	void multiplyBy(double factor) {
		m_secondary *= factor;
	};

	void divideBy(double factor) {
		m_secondary /= factor;
	};

	void setSecondMultiplier(double m) {
		m_secondary = m;
	};

	double getSecondMultiplier() {
		return m_secondary;
	};

	double getMultiplier() {
		return (m_multipliers[m_position] * m_secondary);
	};

	bool decreaseMultiplier() {
		if(m_position == 12) return false;
		m_position++;
		return true;
	};

	bool increaseMultiplier() {
		if(m_position == 0) return false;
		m_position--;
		return true;
	};
private:
	/**
	* Multipliers used for statistics.
	*/
	static const double m_stats[];

	/**
	* Multipliers used for accuracy and evasion.
	*/
	static const double m_acc[];

	int m_position; // Centre of the stat multipliers.

	doubleArray_t m_multipliers;
	double m_secondary;
};

#endif