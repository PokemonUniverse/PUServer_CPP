#ifndef INC_TEAMINFO_H_
#define INC_TEAMINFO_H_

#include <boost/functional/hash.hpp>

struct teamInfo_s 
{
	teamInfo_s() {};
	teamInfo_s(uint8_t team, uint8_t position) : m_team(team), m_position(position) {};

	uint8_t m_team;
	uint8_t m_position;

	inline bool operator==(const teamInfo_s o1) const {
		return (o1.m_team == m_team && o1.m_position == m_position);
	};

	inline bool operator!=(const teamInfo_s o1) const {
		return (o1.m_team != m_team && o1.m_position != m_position);
	};

	friend std::size_t hash_value(teamInfo_s const &input) 
	{
		std::size_t seed = 0;
		boost::hash_combine(seed, input.m_team);
		boost::hash_combine(seed, input.m_position);
		return seed;
	};
};

#endif