#include <iostream>

#include "mysql.h"
using namespace std;

MYSQL_RES* Perform_Query(MYSQL* connection, const char* query)
{
    // send the query to the database
    if (mysql_query(connection, query))
    {
        cout << "MySQL query error : %s\n" << mysql_error(connection) << endl;
        exit(1);
    }

    return mysql_use_result(connection);
}

int main()
{
    string mainServer = "127.0.0.1";
    string mainDbUser = "root";
    string mainDbPass = "";
    MYSQL* connect; //database connection variable

    connect = mysql_init(NULL);
    if (!connect)
        cout << "Couldn't initiate connector\n";

    if (mysql_real_connect(connect, mainServer.c_str(), mainDbUser.c_str(), mainDbPass.c_str(), "abc", 0, NULL, 0))
    {
        cout << "Connection done\n";
        MYSQL_RES* res = Perform_Query(connect, "show tables");
        cout << "Tables in database test" << endl;
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(res)) != NULL)
        {
            cout << row[0] << endl;
        }
        mysql_free_result(res);

    }
    else
    {
        cout << mysql_error(connect) << endl;
    }
    mysql_close(connect);
    return 0;
}
