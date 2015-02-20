#include "pokecode.h"

void PokeCode::Compile(string* outputStr, string inputStr)
{	
	// Get header type
	string headerType[2]; 
	getHeader(headerType, inputStr);
	
	string f_Header = headerType[0];
	string f_Body = headerType[1];

	if(f_Header.compare("WARP") == 0) 
	{
		string tmpOutput[3];
		process_Warp(tmpOutput, f_Body);

		outputStr[0] = "1";
		outputStr[1] = tmpOutput[0];
		outputStr[2] = tmpOutput[1];
		outputStr[3] = tmpOutput[2];
	}
	else if(f_Header.compare("TEXT") == 0) 
	{
		string tmpOutput;
		process_Text(tmpOutput, f_Body);

		outputStr[0] = "2";
		outputStr[1] = tmpOutput;
	}
	else 
	{
		outputStr = NULL;
	}

	return;
}

// Returns function header, like; WARP, TEXT
void PokeCode::getHeader(string out[], string in)
{	
	string::size_type locHeader = in.find_first_of('(');
	string::size_type locEnd = in.find_last_of(')');
	
	out[0] = in.substr(0, locHeader);
	out[1] = in.substr(locHeader + 1, (locEnd - (locHeader + 1))); // rest of the string

	return;
}

// Trim whitespaces
void PokeCode::TrimStr(string& str)
{
	string::size_type pos = str.find_last_not_of(' ');
	if(pos != string::npos) 
	{
		str.erase(pos + 1);
		pos = str.find_first_not_of(' ');

		if(pos != string::npos) str.erase(0, pos);
	}
	else 
	{
		str.erase(str.begin(), str.end());
	}
}

// Strips the text from the function
void PokeCode::process_Text(string& out, string in)
{
	TrimStr(in);
	out = in;

	return;
}

// Substracts the X and Y coordinates
void PokeCode::process_Warp(string out[], string in)
{
	string::size_type pos = in.find_first_of(',');
	string::size_type pos2 = in.find_last_of(',');
	string::size_type length = in.length();

	out[0] = in.substr(0, pos);
	out[1] = in.substr(pos + 1, pos2);
	out[2] = in.substr(pos2 + 1, length);

	return;
}