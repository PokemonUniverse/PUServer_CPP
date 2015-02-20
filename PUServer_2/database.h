#ifndef __DATABASE_H_
#define __DATABASE_H_

#include <boost/thread.hpp>
#include <string>
#include <sstream>

#include "definitions.h"

class DBResult;

enum DBParam_t {
	DBPARAM_MULTIINSERT = 1
};

class Database
{
public:
	//************************************
	// Method:    executeQuery
	// FullName:  Database::executeQuery
	// Access:    virtual public 
	// Returns:   bool
	// Qualifier: Executes query which doesn't generates results (eg. INSERT, UPDATE, DELETE...).
	// Parameter: const std::string & query
	//************************************
	virtual bool executeQuery(const std::string &query) { return false; };

	//************************************
	// Method:    storeQuery
	// FullName:  Database::storeQuery
	// Access:    virtual public 
	// Returns:   DBResult*
	// Qualifier: Executes query which generates results (mostly SELECT).
	// Parameter: const std::string & query
	//************************************
	virtual DBResult* storeQuery(const std::string &query) { return 0; };

	//************************************
	// Method:    escapeString
	// FullName:  Database::escapeString
	// Access:    virtual public 
	// Returns:   std::string
	// Qualifier: Prepares string to fit SQL queries including quoting it.
	// Parameter: const std::string & s
	//************************************
	virtual std::string escapeString(const std::string &s) { return "''"; };

	//************************************
	// Method:    escapeBlob
	// FullName:  Database::escapeBlob
	// Access:    virtual public 
	// Returns:   std::string
	// Qualifier: Prepares binary stream to fit SQL queries.
	// Parameter: const char * s
	// Parameter: uint32_t length
	//************************************
	virtual std::string escapeBlob(const char* s, uint32_t length) { return "''"; };

	//************************************
	// Method:    freeResult
	// FullName:  Database::freeResult
	// Access:    virtual public 
	// Returns:   void
	// Qualifier: Resource freeing.
	// Parameter: DBResult * res
	//************************************
	virtual void freeResult(DBResult *res) {};

	
	//************************************
	// Method:    getParam
	// FullName:  Database::getParam
	// Access:    virtual public 
	// Returns:   bool
	// Qualifier: Returns currently used database attribute.
	// Parameter: DBParam_t param
	//************************************
	virtual bool getParam(DBParam_t param) { return false; }

protected:
	//************************************
	// Transaction related methods. 
	//
	// Qualifier: Methods for starting, commiting and rolling back transaction. Each of the returns boolean value.
	// Returns: true on success, false on error
	//
	// Note: If your database system doesn't support transactions you should return true - it's not feature test, 
	//		 code should work without transaction, just will lack integrity.
	//************************************
	friend class DBTransaction;
	virtual bool beginTransaction() { return 0; }
	virtual bool rollback() { return 0; }
	virtual bool commit() { return 0; }
	
public:
	//************************************
	// Method:    getFactory
	// FullName:  Database::getFactory
	// Access:    public static 
	// Returns:   Database*
	// Qualifier: Retruns instance of database handler. Don't create database (or drivers) instances in your code - instead of 
	//			  it use Database::getFactory(). This method stores static instance of connection class internaly to make 
	//			  sure exacly one instance of connection is created for entire system.
	//************************************
	static Database* getFactory();

	//************************************
	// Method:    isConnected
	// FullName:  Database::isConnected
	// Access:    public 
	// Returns:   bool
	// Qualifier: Returns whether or not the database is connected.
	//************************************
	bool isConnected() { return m_connected; }

protected:
	Database() : m_connected(false) {};
	virtual ~Database() {};

	DBResult* verifyResult(DBResult* result);

	bool m_connected;

private:
	static Database* _instance;
};

class DBResult
{
public:
	//************************************
	// Method:    getDataInt
	// FullName:  DBResult::getDataInt
	// Access:    virtual public 
	// Returns:   int32_t
	// Qualifier: Get the Integer value of a field in database
	// Parameter: const std::string & s (The name of the field)
	//************************************
	virtual int32_t getDataInt(const std::string &s) { return 0; };

	//************************************
	// Method:    getDataLong
	// FullName:  DBResult::getDataLong
	// Access:    virtual public 
	// Returns:   int64_t
	// Qualifier: Get the Long value of a field in database
	// Parameter: const std::string & s
	//************************************
	virtual int64_t getDataLong(const std::string &s) { return 0; };

	//************************************
	// Method:    getDataString
	// FullName:  DBResult::getDataString
	// Access:    virtual public 
	// Returns:   std::string
	// Qualifier: Get the String of a field in database
	// Parameter: const std::string & s
	//************************************
	virtual std::string getDataString(const std::string &s) { return "''"; };

	//************************************
	// Method:    getDataStream
	// FullName:  DBResult::getDataStream
	// Access:    virtual public 
	// Returns:   const char*
	// Qualifier: Get the blob of a field in database
	// Parameter: const std::string & s
	// Parameter: unsigned long & size
	//************************************
	virtual const char* getDataStream(const std::string &s, unsigned long &size) { return 0; };

	//************************************
	// Method:    next
	// FullName:  DBResult::next
	// Access:    virtual public 
	// Returns:   true if moved, false if there are no more results.
	// Qualifier: Moves to next result in set.
	//************************************
	virtual bool next() { return false; };

	//************************************
	// Method:    count
	// FullName:  DBResult::count
	// Access:    virtual public 
	// Returns:   uint32_t
	// Qualifier: Returns the number of rowns in the result
	//************************************
	virtual uint32_t count() { return 0; };

protected:
	DBResult() {};
	virtual ~DBResult() {};
};

class DBQuery : public std::stringstream
{
	friend class Database;

public:
	DBQuery();
	~DBQuery();

protected:
	static boost::recursive_mutex database_lock;
};

/**
 * INSERT statement.
 *
 * Gives possibility to optimize multiple INSERTs on databases that support multiline INSERTs.
 */
class DBInsert
{
public:
	/**
	* Associates with given database handler.
	*
	* @param Database* database wrapper
	*/
	DBInsert(Database* db);
	~DBInsert() {};

	/**
	* Sets query prototype.
	*
	* @param std::string& INSERT query
	*/
	void setQuery(const std::string& query);

	/**
	* Adds new row to INSERT statement.
	*
	* On databases that doesn't support multiline INSERTs it simply execute INSERT for each row.
	*
	* @param std::string& row data
	*/
	bool addRow(const std::string& row);
	/**
	* Allows to use addRow() with stringstream as parameter.
	*/
	bool addRow(std::stringstream& row);

	/**
	* Executes current buffer.
	*/
	bool execute();

protected:
	Database* m_db;
	bool m_multiLine;
	uint32_t m_rows;
	std::string m_query;
	std::string m_buf;
};

class DBTransaction
{
public:
	DBTransaction(Database* database)
	{
		m_database = database;
		m_state = STATE_NO_START;
	}

	~DBTransaction()
	{
		if(m_state == STATE_START) {
			m_database->rollback();
		}
	}

	bool begin()
	{
		m_state = STATE_START;
		return m_database->beginTransaction();
	}

	bool commit()
	{
		if(m_state == STATE_START) {
			m_state = STATE_COMMIT;
			return m_database->commit();
		}
		else {
			return false;
		}
	}

private:
	enum TransactionStates_t {
		STATE_NO_START,
		STATE_START,
		STATE_COMMIT
	};
	TransactionStates_t m_state;
	Database* m_database;
};

#include "databasemysql.h"

#endif