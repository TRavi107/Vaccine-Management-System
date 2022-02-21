#pragma once
#include "mysql.h"
class sqlConnection
{
public:
	sqlConnection();
	~sqlConnection();

	std::string error = "";

	MYSQL_RES* Perform_Query(MYSQL* connection, const char* query);

	inline void SetdataBaseName(const char* name) { mainDbName = name; };
	inline const char* GetdataBaseName() { return mainDbName ; };
	inline MYSQL* GetConnectPtr() { return connect; };

private:
	std::string mainServer = "127.0.0.1";
	std::string mainDbUser = "root";
	std::string mainDbPass = "";
	const char* mainDbName = "abc";
	MYSQL* connect; //database connection variable
};

