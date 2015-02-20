#include "configmanager.h"
#include <boost/lexical_cast.hpp>

ConfigManager::ConfigManager() 
{
}

ConfigManager::ConfigManager(string name, string parentDebugInfo) 
{
	debugInfo = (std::string)"hello";
	debugInfo = parentDebugInfo + ", " + name;
}

ConfigManager::ConfigManager(string configFile, char** envp) 
{
	debugInfo = "";
	while (*envp) 
	{
		string envEntry = *envp;
		size_t pos = envEntry.find('=');

		if (pos != string::npos) 
		{
			string name = envEntry.substr(0, pos);
			string value = envEntry.substr(pos+1, string::npos);
			envSymbols[name] = value;
			//cout << "environment symbol: '" << name << "' = '" << value << "'" << endl;
		}
		++envp;
	}

	loadFile(configFile);
}

ConfigManager::~ConfigManager() 
{
	for (map<string, ConfigManager*>::iterator i = groups.begin(); i != groups.end(); ++i) 
	{
		delete i->second;
	}
}

void ConfigManager::loadFile(string configFile)
{
	debugInfo = configFile;
	groupStack.push_front(this);

	FILE* in = fopen(configFile.c_str(), "r");
	if (!in) 
	{
		cerr << "[Config] Cannot open input file '" << configFile << "'" << endl;
	}

	char buff[1024];
	while (fgets(buff, 1024, in)) {

		string line=buff;
		if ( (line.length() > 2) && (line[0] != '#') && (line.find(')') == string::npos) ) {
			string name;
			string value;
			split(line, name, value, '=');

			if (value == "(") 
			{
				//cout << "   config: new group '" << name << "'" << endl;
				ConfigManager* newGroup = new ConfigManager(name, debugInfo);
				groupStack.front()->groups[name] = newGroup;
				groupStack.push_front(newGroup);
			} 
			else 
			{
				for (list<ConfigManager*>::reverse_iterator i = groupStack.rbegin(); i != groupStack.rend(); ++i) 
				{
					(*i)->symbolExpand(value);
				}

				envSymbolExpand(value);
				//cout << "   config: name = '" << name << "', value = '" << value << "'" << endl);
				groupStack.front()->add(name, value);
			}
		}
		if ( (line.length() > 0) && (line[0] != '#') && (line.find(')') != string::npos) ) 
		{
			//cout << "   end of group" << endl;
			groupStack.pop_front();
		}
	}

	fclose(in);
}

void ConfigManager::add(string name, string value) 
{
	symbols[name] = value;
}

void ConfigManager::split(string in, string& left, string& right, char c) 
{
	size_t pos = in.find_first_of(c);
	if(pos == string::npos) 
	{
		left = in;
		trim(left);
		right = "";
	} 
	else if (pos <= 1) 
	{
		left = "";
		right = in.substr(pos+1, string::npos);
		trim(right);
	} 
	else 
	{
		left = in.substr(0, pos);
		trim(left);
		right = in.substr(pos+1, string::npos);
		trim(right);
	}
}

void ConfigManager::trim(string& s) 
{
	while ( (s.length() > 1) && ( (s[0] == ' ') || (s[0] =='\t') ) ) 
	{
		s = s.substr(1, string::npos);
	}

	while ( (s.length() > 1) &&
			( (s[s.length()-1] == ' ') ||
			  (s[s.length()-1] == '\t') || 
			  (s[s.length()-1] == '\n') || 
			  (s[s.length()-1] == '\r') ) ) {
		s = s.substr(0, s.length()-1);
	}

	if ( (s.length() > 1) && (s[0] == '"') ) 
	{
		s = s.substr(1, string::npos);
	}

	if ( (s.length() > 1) && (s[s.length()-1] == '"') ) 
	{
		s = s.substr(0, s.length()-1);
	}
}

void ConfigManager::symbolExpand(string& s) 
{
	symbolExpand(symbols, s);
}

void ConfigManager::envSymbolExpand(string& s) 
{
	symbolExpand(envSymbols, s);
}

void ConfigManager::symbolExpand(map<string, string>& symbols, string& s) 
{
	bool expanded;
	do 
	{
		expanded = false;
		for (map<string, string>::iterator i = symbols.begin(); i != symbols.end(); ++i) 
		{
			string search = "%" + i->first + "%";
			string replace = i->second;
			size_t pos = s.find(search);
			if (pos != string::npos) {
				expanded = true;
				s.replace(pos, search.length(), replace);
			}
		}
	} while (expanded);
}

string ConfigManager::pString(string name) 
{
	map<string, string>::iterator i = symbols.find(name);

	if (i == symbols.end()) 
	{
		cout << "[ConfigError] Access of missing property '" << name << "' (" << debugInfo << ")" << endl;
	}

	return i->second;
}

bool ConfigManager::pBool(string name) 
{
	string val = pString(name);

	if ( (val == "yes") ||
	     (val == "Yes") ||
	     (val == "YES") ||
		 (val == "true") ||
	     (val == "True") ||
	     (val == "TRUE"))
	{
		return true;
	}

	return false;
}

double ConfigManager::pDouble(string name) 
{
	string val = pString(name);
	
	return boost::lexical_cast<double>(val);
}

int ConfigManager::pInt(string name) 
{
	string val = pString(name);

	return boost::lexical_cast<int>(val);
}