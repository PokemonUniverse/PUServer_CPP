#ifndef INC_BERRY_H_
#define INC_BERRY_H_

class Pokemon;

public class Berry
{
public:
	virtual void executeEffects(Pokemon *p) = 0;
};

#endif