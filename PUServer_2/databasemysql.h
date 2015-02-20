#ifdef __USE_MYSQL__

#ifndef __DATABASE_MYSQL_H_
#define __DATABASE_MYSQL_H_

#ifndef __DATABASE_H_
#error "database.h should be included first."
#endif

#include <iostream>
#include <map>
#include <string>
#include <boost/asio.hpp>
#include <mysql.h>
#include <errmsg.h>

using namespace std;

class DatabaseMySQL : Database
{
	friend class Database;

public:
	DatabaseMySQL();
	virtual ~DatabaseMySQL();

	virtual bool beginTransaction();
	virtual bool rollback();
	virtual bool commit();

	virtual bool executeQuery(const std::string &query);
	virtual DBResult* storeQuery(const std::string &query);

	virtual std::string escapeString(const std::string &s);
	virtual std::string escapeBlob(const char* s, uint32_t length);

	virtual bool getParam(DBParam_t param);

	virtual void freeResult(DBResult *res);	

private:
	MYSQL m_handle;
};

class DBResultMySQL : DBResult
{
	friend class DatabaseMySQL;

public:
	int32_t getDataInt(const std::string &s);
	int64_t getDataLong(const std::string &s);
	std::string getDataString(const std::string &s);
	const char* getDataStream(const std::string &s, unsigned long &size);

	uint32_t count();

	bool next();

protected:
	DBResultMySQL(MYSQL_RES* res);
	~DBResultMySQL();

	typedef std::map<const std::string, uint32_t> listNames_t;
	listNames_t m_listNames;

	MYSQL_RES* m_handle;
	MYSQL_ROW m_row;
};

#endif

#endif // __USE_MYSQL__