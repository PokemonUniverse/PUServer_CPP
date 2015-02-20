#include "database.h"

#ifdef __USE_MYSQL__
#include "databasemysql.h"
#endif

boost::recursive_mutex DBQuery::database_lock;

Database* Database::_instance = NULL;

Database* Database::getFactory()
{
	if(!_instance){
#ifdef __USE_MYSQL__
		_instance = new DatabaseMySQL;
#else
		_instance = new Database;
#endif
	}

	return _instance;
}

DBResult* Database::verifyResult(DBResult* result)
{
	if(!result->next()){
		_instance->freeResult(result);
		return NULL;
	}
	else{
		return result;
	}
}

DBQuery::DBQuery()
{
	database_lock.lock();
}

DBQuery::~DBQuery()
{
	database_lock.unlock();
}

DBInsert::DBInsert(Database* db)
{
	m_db = db;
	m_rows = 0;

	// checks if current database engine supports multiline INSERTs
	m_multiLine = m_db->getParam(DBPARAM_MULTIINSERT) != 0;
}

void DBInsert::setQuery(const std::string& query)
{
	m_query = query;
	m_buf = "";
	m_rows = 0;
}

bool DBInsert::addRow(const std::string& row)
{
	if(m_multiLine){
		m_rows++;
		int32_t size = m_buf.length();

		// adds new row to buffer
		if(size == 0){
			m_buf = "(" + row + ")";
		}
		else if(size > 8192){
			if(!execute())
				return false;

			m_buf = "(" + row + ")";
		}
		else{
			m_buf += ",(" + row + ")";
		}

		return true;
	}
	else{
		// executes INSERT for current row
		return m_db->executeQuery(m_query + "(" + row + ")" );
	}
}

bool DBInsert::addRow(std::stringstream& row)
{
	bool ret = addRow(row.str());
	row.str("");
	return ret;
}

bool DBInsert::execute()
{
	if(m_multiLine && m_buf.length() > 0){
		if(m_rows == 0){
			//no rows to execute
			return true;
		}

		m_rows = 0;

		// executes buffer
		bool res = m_db->executeQuery(m_query + m_buf);
		m_buf = "";
		return res;
	}
	else{
		// INSERTs were executed on-fly
		return true;
	}
}