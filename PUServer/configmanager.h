#ifndef __configmanager_h_
#define __configmanager_h_

#include <iostream>
#include <string>
#include <map>
#include <list>
#include <stdio.h>

using namespace std;

class ConfigManager {
public:
	ConfigManager();
	ConfigManager(string configFile, char** envp = 0);

	~ConfigManager();

	void loadFile(string configFile);

	string pString(string name);

	/* get boolean config entry
	* A value of Yes/yes/YES/true/True/TRUE leads to true,
	* all other values leads to false.
	*/
	bool pBool(string name);

	double pDouble(string name);

	int pInt(string name);

	inline map<string, string>& getSymbols() {
		return symbols;
	}

	inline ConfigManager* group(string name) {
		return groups[name];
	}

	inline map<string, ConfigManager*>& getGroups() {
		return groups;
	}
	
private:
	ConfigManager(string name, string parentDebugInfo);

	// helper functions for parsing
	void add(string name, string value);
	void split(string in, string& left, string& right, char c);
	void trim(string& s);
	void symbolExpand(string& s);
	void symbolExpand(map<string, string>& symbols, string& s);
	void envSymbolExpand(string& s);

	map<string, string> symbols;
	map<string, string> envSymbols;
	map<string, ConfigManager*> groups;

	list<ConfigManager*> groupStack;

	std::string debugInfo;
};

#endif