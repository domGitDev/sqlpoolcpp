gcc -std=c++11 -g \
-I$(pwd) \
-I$(pwd)/../ \
-I/usr/include/mysql \
-L/usr/lib64/mysql \
main.cpp -lstdc++ -lpthread -lmysqlclient -o multiConn
