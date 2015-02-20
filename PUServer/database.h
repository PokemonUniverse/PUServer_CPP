#ifndef __database_h_
#define __database_h_

#include <iostream>
#include <map>
#include <string>
#include <boost/asio.hpp>
#include <mysql.h>
#include <errmsg.h>
#include "configmanager.h"

using namespace std;

extern ConfigManager g_config;

class Database
{
public:
	Database();
	~Database();
	
	bool connectDB();
	bool disconnectDB();

	int getLastID();

	bool isConnected() { return connected; }
	MYSQL *getHandle() { return &handle; }

	bool runQuery(std::string query);

private:
	
	bool connected;
	MYSQL handle;

	string databaseIP;
	string databaseUser;
	string databasePass;
	string databaseDB;

	//CRITICAL_SECTION mDBCriticalSection;
};

class Recordset
{
public:
	Recordset(Database *db, std::string query);
	~Recordset();

	bool moveNext();
	int numOfRows();

	unsigned long getInt(std::string field);
	std::string getString(std::string field);

private:

	MYSQL_RES* results;
	MYSQL_ROW currentRow;
	
	//CRITICAL_SECTION mCriticalSection;	

	std::map<std::string, int> fieldNames;
};

#endif