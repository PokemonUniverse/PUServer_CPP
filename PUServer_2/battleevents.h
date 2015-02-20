#ifndef __BATTLEEVENT_H_
#define __BATTLEEVENT_H_

#include "gametext.h"
#include "packet.h"

enum eventtypes_t
{
	EVENT_SLEEP,
	EVENT_TEXT
};

class BattleEvent
{
public:
	virtual eventtypes_t getType() const = 0;
	virtual Packet getPacket() const = 0;
};

class BattleEvent_Sleep : BattleEvent
{
public:
	BattleEvent_Sleep() {
		sleepTime = 1000;
	};
	~BattleEvent_Sleep() {};

	eventtypes_t getType() { return EVENT_SLEEP; };
	Packet getPacket();

	//************************************
	// Method:    setTicks
	// FullName:  BattleEvent_Sleep::setTicks
	// Access:    public 
	// Returns:   void
	// Qualifier: Sets the amount of time the client should wait before processing the next event. Time is in miliseconds.
	// Parameter: int _sleepTime
	//************************************
	void setTicks(int _sleepTime) { sleepTime = _sleepTime; };

private:
	long sleepTime;
};

class BattleEvent_Text : BattleEvent
{
public:
	BattleEvent_Text() {};
	~BattleEvent_Text() {};
	
	eventtypes_t getType() { return EVENT_TEXT; };
	Packet getPacket();

	//************************************
	// Method:    setGameText
	// FullName:  BattleEvent_Text::setGameText
	// Access:    public 
	// Returns:   void
	// Qualifier: Adds the text to the event, without variables
	// Parameter: PU_Text _text
	//************************************
	void setGameText(PU_Text _text) { text = _text; };
	//************************************
	// Method:    setGameText
	// FullName:  BattleEvent_Text::setGameText
	// Access:    public 
	// Returns:   void
	// Qualifier: Adds the text to the event, with variables
	// Parameter: PU_Text _text
	// Parameter: TextVarHolder _vars
	//************************************
	void setGameText(PU_Text _text, TextVarHolder _vars) {
		text = _text;
		vars = _vars;
	};

private:
	PU_Text text;
	TextVarHolder vars;
};
#endif