#include "vmspch.h"
#include "sqlConnection.h"

sqlConnection::sqlConnection(std::string server, std::string username, std::string pass, std::string dbName) :
    mainServer(server), mainDbUser(username), mainDbPass(pass), mainDbName(dbName)
{
    connect = mysql_init(NULL);
    if (!connect)
        std::cout << "Couldn't initiate connector\n";
    if (mysql_real_connect(connect, mainServer.c_str(), mainDbUser.c_str(), mainDbPass.c_str(), mainDbName.c_str(), 0, NULL, 0))
    {
        //std::cout << "Connection done\n";
        //MYSQL_RES* res = Perform_Query(connect, "select * from enroll");
        ////std::cout << "Tables in database test" << std::endl;
        //MYSQL_ROW row;
        //while ((row = mysql_fetch_row(res)) != NULL)
        //{
        //    std::cout << row[0]<<"     "<<row[1] << std::endl;
        //}
        //mysql_free_result(res);
    }
    else
    {
        std::cout << mysql_error(connect) << std::endl;
        exit(1);
    }
    std::cout << mainServer << std::endl;
}


sqlConnection::~sqlConnection()
{
    mysql_close(connect);
}

MYSQL_RES* sqlConnection::Perform_Query(MYSQL* connection, const char* query)
{
    // send the query to the database
    if (mysql_query(connection, query))
    {
        std::cout << "MySQL query error : %s\n" << mysql_error(connection) << std::endl;
        error = mysql_error(connection);
        //exit(1);
    }
    else {
        error = "";
    }
    return mysql_use_result(connection);
}
