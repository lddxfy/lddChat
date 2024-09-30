#pragma once
#include "jdbc/cppconn/driver.h"
#include "jdbc/cppconn/resultset.h"
#include <jdbc/cppconn/statement.h>
#include "jdbc/cppconn/prepared_statement.h"
#include "jdbc/mysql_driver.h"
#include "jdbc/mysql_connection.h"
#include <jdbc/cppconn/exception.h>  

#include "const.h"
class SqlConnection {
public:


};

class MysqlConPool
{
public:
	MysqlConPool(size_t poolsize, std::string ip, std::string port, std::string user, std::string pass, std::string schema);
	~MysqlConPool();
	std::unique_ptr<sql::Connection> GetConnection();

	void returnConnection(std::unique_ptr<sql::Connection> con);

	void Close();

private:
	std::string host_;
	std::string pass_;
	std::string user_;
	std::string schema_;
	size_t poolsize_;
	std::mutex mutex_;
	std::condition_variable cond_;
	std::queue<std::unique_ptr<sql::Connection>> pool_;
	std::atomic<bool> b_stop_;

};

