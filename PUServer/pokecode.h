#ifndef _POKECODE_H_
#define _POKECODE_H_

#include <iostream>

using namespace std;

enum FunctionTypes
{
	F_WARP = 1,
	F_TEXT = 2
};

class PokeCode
{
public:

	static void Compile(string *outputStr, string inputStr);

private:
	static void getHeader(string out[], string in);

	static void TrimStr(string& str);

	static void process_Text(string& out, string in);
	static void process_Warp(string out[], string in);
};

#endif // _POKECODE_H_