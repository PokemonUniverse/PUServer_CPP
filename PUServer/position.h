#ifndef __position_h_
#define __position_h_

//////////////////////////////////////////////////
// represents a map position
// for now just a 3d point
//
// Z represents the inside (1) or outside (0)
#include <stdlib.h>
#include <cmath>
#include <iostream>
#include <sstream>
#include <vector>
#include <boost/format.hpp>

class Position
{
public:
	template<int deltax, int deltay, int deltaz>
	inline static bool areInRange(const Position& p1, const Position& p2) {
		if(std::abs((double)(p1.z - p2.z)) > deltaz || std::abs((double)(p1.x - p2.x)) > deltax || std::abs((double)(p1.y - p2.y)) > deltay) {
			return false;
		}
		return true;
	}

	template<int deltax, int deltay>
	inline static bool areInRange(const Position& p1, const Position& p2) {
		if(std::abs((double)(p1.x - p2.x)) > deltax || std::abs((double)(p1.y - p2.y)) > deltay) {
			return false;
		}
		return true;
	}
	
	// for now we just initialise the position to a startpoint
	Position() : x(-368), y(-101), z(0) { };
	Position(int _x, int _y, int _z) : x(_x), y(_y), z(_z) { };
	Position(std::string _position)
	{
		std::stringstream wordStream(_position);
		std::vector<std::string> wordContainer;
		std::string word;
		while(std::getline(wordStream, word, ';')){
			wordContainer.push_back(word);
		}

		if(wordContainer.size() < 3) {
			x = -368;
			y = -101;
			z = 0;
		}
		else {
			x = atoi(wordContainer[0].c_str());
			y = atoi(wordContainer[1].c_str());
			z = atoi(wordContainer[2].c_str());
		}
	}

	~Position() {};

	int x,y,z;

	std::string toString()
	{
		return boost::str(boost::format("%d;%d;%d") % x % y % z);
	}

	bool operator<(const Position& p) const {
		if(z < p.z)
			return true;
		if(z > p.z)
			return false;

		if(y < p.y)
			return true;
		if(y > p.y)
			return false;

		if(x < p.x)
			return true;
		if(x > p.x)
			return false;

		return false;
	}

	bool operator>(const Position& p) const {
		return ! (*this < p);
	}

	bool operator==(const Position p)  const {
		if(p.x==x && p.y==y && p.z ==z)
			return true;
		else
			return false;
	}

	bool operator!=(const Position p)  const {
		if(p.x==x && p.y==y && p.z ==z)
			return false;
		else
			return true;
	}

	Position operator-(const Position p1){
		return Position(x-p1.x, y-p1.y,z-p1.z);
	}

	bool operator==(const Position p){
		return (x==p.x && y==p.x && z==p.z);
	}

	static Position PosNULL() {
		return Position(0,0,0);
	}
	/*
	void dist(){
		x=abs(x);
		y=abs(y);
		z=abs(z);
	}
	*/
};

#endif