#ifndef __MOVESET_H_
#define __MOVESET_H_

class MoveSet
{
public:
	MoveSet();
	~MoveSet();

    /**
     * Return the textual name of a category of moves.
     */
	static std::string getMoveType(int i) {
        switch (i) {
            case 0: return "Level";
            case 1: return "Move Tutor";
            case 2: return "TM";
            case 3: return "HM";
            case 4: return "Egg Group";
        }
        return "";
    }

private:

};
#endif