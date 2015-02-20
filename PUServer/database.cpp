#include "database.h"

Database::Database()
{
	connected = false;
	
	if(!mysql_init(&handle))
	{
 		std::cout << "\n* Could not initialize MySQL" << std::endl;		
		return;
	}

	//InitializeCriticalSection(&mDBCriticalSection);
}

Database::~Database()
{
	disconnectDB();
}

bool Database::connectDB()
{
	if(isConnected())
		return true;
	
	databaseIP = g_config.pString("databaseIP");
	databaseUser = g_config.pString("databaseUser");
	databasePass = g_config.pString("databasePass");
	databaseDB = g_config.pString("databaseDB");

	my_bool param = 1;
	mysql_options(&handle, MYSQL_OPT_RECONNECT, &param);

	if(!mysql_real_connect(getHandle(), databaseIP.c_str(),  databaseUser.c_str(), databasePass.c_str(), databaseDB.c_str(), 3306, NULL, 0))
	{
		std::cout << "\n* Error connecting to MySQL database: " << mysql_error(getHandle()) << std::endl;
		return false;
	}

	connected = true;

	return true;
}

bool Database::disconnectDB()
{
	if(!isConnected() || getHandle() == NULL)
		return true;

	connected = false;
	mysql_close(getHandle());

	//LeaveCriticalSection(&mDBCriticalSection);

	return true;
}

bool Database::runQuery(std::string query)
{
	bool returnValue = true;

	if(!connected || getHandle() == NULL)
		returnValue = false;

	if(mysql_ping(getHandle()) != 0)
		returnValue =  false;
	
	if(mysql_real_query(getHandle(), query.c_str(), query.length()) != 0)
	{
		std::cout << "\n* Error executing query: " << query.c_str() << "\n* Error: " << mysql_error(&handle) << std::endl;
		int error = mysql_errno(getHandle());

		if(error == CR_SERVER_LOST || error == CR_SERVER_GONE_ERROR)
			connected = false;

		returnValue =  false;
	}

	return returnValue;
}

int Database::getLastID()
{
	//EnterCriticalSection(&mDBCriticalSection);
	int lastID = mysql_insert_id(getHandle());
	//LeaveCriticalSection(&mDBCriticalSection);

	return lastID;
}

Recordset::Recordset(Database *db, std::string query)
{	
	if(!db->isConnected() || mysql_ping(db->getHandle()) != 0) {
		return;
	}

	if(mysql_real_query(db->getHandle(), query.c_str(), query.length()) != 0)
		std::cout << "\n* Error opening recordset: " << query.c_str() << "\n* Error: " << mysql_error(db->getHandle()) << std::endl;

	results = mysql_store_result(db->getHandle());
	if(results)
	{
		fieldNames.clear();

		MYSQL_FIELD* field;
		for(int i = 0; field = mysql_fetch_field(results); i++)
		{
			fieldNames.insert(std::pair<std::string, int>(field->name,i));
		}
	}
}

Recordset::~Recordset()
{
	mysql_free_result(results);
}

bool Recordset::moveNext()
{
	if(!results)
		return false;

	if(results && mysql_num_rows(results) > 0)
	{
		currentRow = mysql_fetch_row(results);

		if(currentRow) {
			return true;
		}
		else {		
			return false;
		}
	}

	return false;
}

int Recordset::numOfRows()
{
	if(!results)
		return 0;
	
	int numRows = (int)mysql_num_rows(results);

	return numRows;
}

std::string Recordset::getString(std::string field)
{
	std::string returnStr = "";
	std::map<std::string,int>::iterator it = fieldNames.find(field);
	if(it != fieldNames.end())
	{
		if(!currentRow[it->second])
			returnStr = "";
		else
			returnStr = currentRow[it->second];
	}

	return returnStr;
}

unsigned long Recordset::getInt(std::string field)
{
	int returnInt = 0;
	std::map<std::string,int>::iterator it = fieldNames.find(field);
	if(it != fieldNames.end())
	{
		if(!currentRow[it->second])
			returnInt = 0;
		else
			returnInt = atoi(currentRow[it->second]);
	}

	return returnInt;
}