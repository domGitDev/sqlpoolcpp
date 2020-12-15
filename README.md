# ##########################################################

    MYSQL CONNECTION POOL FOR REALTIME DATA

    AUTHOR: DOM SILVA

    COPYRIGHT: ENCE20 Inc.

    WEBSITE: https://ence20.com

# ##########################################################


# Install Dependencies CentOS

- yum -y install mysql-community-libs mysql-devel

# INCLUDE IN PROJECT

# #########################################

#include "./sqlconn/ConnectionPool.h"

std::shared_ptr<ConnectionPool> connPool;

std::string host = "127.0.0.1";

std::string username = "root";

std::string password = "password";

std::string database = "mydb";

size_t NUM_CONNS = 3;

connPool.reset(new ConnectionPool(
                host, port, username, password, database, NUM_CONNS));

# #############################################

# RUN EXAMPLE

- update database credentials by editing dbcredentials.txt 

- on command run:

compile.sh

./multiConn

- press Ctrl+C to exit


