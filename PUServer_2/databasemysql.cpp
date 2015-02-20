#include "definitions.h"

#ifdef __USE_MYSQL__

#include "database.h"
#include "databasemysql.h"

#include "configmanager.h"
extern ConfigManager g_config;

#include "logger.h"

/** DatabaseMySQL definitions */

DatabaseMySQL::DatabaseMySQL()
{
	m_connected = false;

	// connection handle initialization
	if(!mysql_init(&m_handle)){
		Logger::consoleOutput(COUTPUT_ERROR, "Failed to initialize MySQL connection handle.");
		return;
	}

	// automatic reconnect
	my_bool reconnect = true;
	mysql_options(&m_handle, MYSQL_OPT_RECONNECT, &reconnect);

	std::string SQL_HOST = g_config.pString("DATABASE_IP");
	std::string SQL_USER = g_config.pString("DATABASE_USER");
	std::string SQL_PASS = g_config.pString("DATABASE_PASS");
	std::string SQL_DB	 = g_config.pString("DATABASE_TABLE");

	// connects to database
	if(!mysql_real_connect(&m_handle, SQL_HOST.c_str(), SQL_USER.c_str(), SQL_PASS.c_str(), SQL_DB.c_str(), 3306, NULL, 0)){
		std::stringstream error;
		error << "Failed to connect to database. MYSQL ERROR: " << mysql_error(&m_handle);
		Logger::consoleOutput(COUTPUT_ERROR, error.str());
		return;
	}

	if(MYSQL_VERSION_ID < 50019){
		//mySQL servers < 5.0.19 has a bug where MYSQL_OPT_RECONNECT is (incorrectly) reset by mysql_real_connect calls
		//See http://dev.mysql.com/doc/refman/5.0/en/mysql-options.html for more information.
		mysql_options(&m_handle, MYSQL_OPT_RECONNECT, &reconnect);
		Logger::consoleOutput(COUTPUT_WARNING, "Outdated mySQL server detected. Consider upgrading to a newer version.");
	}

	m_connected = true;
}

DatabaseMySQL::~DatabaseMySQL()
{
	mysql_close(&m_handle);
}

bool DatabaseMySQL::getParam(DBParam_t param)
{
	switch(param){
	case DBPARAM_MULTIINSERT:
		return true;
		break;
	default:
		return false;
		break;
	}
}

bool DatabaseMySQL::beginTransaction()
{
	return executeQuery("BEGIN");
}

bool DatabaseMySQL::rollback()
{
	if(!m_connected)
		return false;

#ifdef __DEBUG_SQL__
	std::cout << "ROLLBACK" << std::endl;
#endif

	if( mysql_rollback(&m_handle) != 0){
		std::cout << "mysql_rollback(): MYSQL ERROR: " << mysql_error(&m_handle) << std::endl;
		return false;
	}

	return true;
}

bool DatabaseMySQL::commit()
{
	if(!m_connected)
		return false;

#ifdef __DEBUG_SQL__
	std::cout << "COMMIT" << std::endl;
#endif
	if( mysql_commit(&m_handle) != 0){
		std::cout << "mysql_commit(): MYSQL ERROR: " << mysql_error(&m_handle) << std::endl;
		return false;
	}

	return true;
}

bool DatabaseMySQL::executeQuery(const std::string &query)
{
	if(!m_connected)
		return false;

#ifdef __DEBUG_SQL__
	std::cout << "MYSQL QUERY: " << query << std::endl;
#endif

	bool state = true;

	// executes the query
	if(mysql_real_query(&m_handle, query.c_str(), query.length()) != 0){
		std::cout << "mysql_real_query(): " << query << ": MYSQL ERROR: " << mysql_error(&m_handle) << std::endl;
		int error = mysql_errno(&m_handle);

		if(error == CR_SERVER_LOST || error == CR_SERVER_GONE_ERROR){
			m_connected = false;
		}

		state = false;
	}

	// we should call that every time as someone would call executeQuery('SELECT...')
	// as it is described in MySQL manual: "it doesn't hurt" :P
	MYSQL_RES* m_res = mysql_store_result(&m_handle);

	if(m_res)
		mysql_free_result(m_res);

	return state;
}

DBResult* DatabaseMySQL::storeQuery(const std::string &query)
{
	if(!m_connected)
		return NULL;

#ifdef __DEBUG_SQL__
	std::cout << "MYSQL QUERY: " << query << std::endl;
#endif

	// executes the query
	if(mysql_real_query(&m_handle, query.c_str(), query.length()) != 0)
	{
		std::stringstream errormsg;
		errormsg << "mysql_real_query(): " << query << ": MYSQL ERROR: " << mysql_error(&m_handle) << std::endl;
		Logger::consoleOutput(COUTPUT_SQL, errormsg.str());

		int error = mysql_errno(&m_handle);
		if(error == CR_SERVER_LOST || error == CR_SERVER_GONE_ERROR){
			m_connected = false;
		}

		return NULL;
	}

	// we should call that every time as someone would call executeQuery('SELECT...')
	// as it is described in MySQL manual: "it doesn't hurt" :P
	MYSQL_RES* m_res = mysql_store_result(&m_handle);

	// error occured
	if(!m_res)
	{
		std::stringstream errormsg;
		errormsg << "mysql_store_result(): " << query << ": MYSQL ERROR: " << mysql_error(&m_handle) << std::endl;
		Logger::consoleOutput(COUTPUT_SQL, errormsg.str());

		int error = mysql_errno(&m_handle);
		if(error == CR_SERVER_LOST || error == CR_SERVER_GONE_ERROR){
			m_connected = false;
		}

		return NULL;
	}

	// retriving results of query
	DBResult* res = new DBResultMySQL(m_res);
	return verifyResult(res);
}

std::string DatabaseMySQL::escapeString(const std::string &s)
{
	return escapeBlob( s.c_str(), s.length() );
}

std::string DatabaseMySQL::escapeBlob(const char* s, uint32_t length)
{
	// remember about quoiting even an empty string!
	if(!s)
		return std::string("''");

	// the worst case is 2n + 1
	char* output = new char[length * 2 + 1];

	// quotes escaped string and frees temporary buffer
	mysql_real_escape_string(&m_handle, output, s, length);
	std::string r = "'";
	r += output;
	r += "'";
	delete[] output;
	return r;
}

void DatabaseMySQL::freeResult(DBResult* res)
{
	delete (DBResultMySQL*)res;
}

/** MySQLResult definitions */

int32_t DBResultMySQL::getDataInt(const std::string &s)
{
	listNames_t::iterator it = m_listNames.find(s);
	if(it != m_listNames.end() ){
		if(m_row[it->second] == NULL){
			return 0;
		}
		else{
			return atoi(m_row[it->second]);
		}
	}

	std::cout << "Error during getDataInt(" << s << ")." << std::endl;
	return 0; // Failed
}

int64_t DBResultMySQL::getDataLong(const std::string &s)
{
	listNames_t::iterator it = m_listNames.find(s);
	if(it != m_listNames.end()){
		if(m_row[it->second] == NULL){
			return 0;
		}
		else{
			return ATOI64(m_row[it->second]);
		}
	}

	std::cout << "Error during getDataLong(" << s << ")." << std::endl;
	return 0; // Failed
}

std::string DBResultMySQL::getDataString(const std::string &s)
{
	listNames_t::iterator it = m_listNames.find(s);
	if(it != m_listNames.end() ){
		if(m_row[it->second] == NULL)
			return std::string("");
		else
			return std::string(m_row[it->second]);
	}

	std::cout << "Error during getDataString(" << s << ")." << std::endl;
	return std::string(""); // Failed
}

const char* DBResultMySQL::getDataStream(const std::string &s, unsigned long &size)
{
	listNames_t::iterator it = m_listNames.find(s);
	if(it != m_listNames.end() ){
		if(m_row[it->second] == NULL){
			size = 0;
			return NULL;
		}
		else{
			size = mysql_fetch_lengths(m_handle)[it->second];
			return m_row[it->second];
		}
	}

	std::cout << "Error during getDataStream(" << s << ")." << std::endl;
	size = 0;
	return NULL;
}

uint32_t DBResultMySQL::count()
{
	return mysql_num_rows(m_handle);
}

bool DBResultMySQL::next()
{
	m_row = mysql_fetch_row(m_handle);
	return m_row != NULL;
}

DBResultMySQL::DBResultMySQL(MYSQL_RES* res)
{
	m_handle = res;
	m_listNames.clear();

	MYSQL_FIELD* field;
	int32_t i = 0;
	while((field = mysql_fetch_field(m_handle))){
		m_listNames[field->name] = i;
		i++;
	}
}

DBResultMySQL::~DBResultMySQL()
{
	mysql_free_result(m_handle);
}

#endif
