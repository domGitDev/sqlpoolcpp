#include <locale>
#include <ctime>
#include <chrono>
#include <mutex>
#include <map>
#include <csignal>
#include <sstream>
#include <fstream>
#include <thread>
#include <iostream>
#include "./sqlconn/ConnectionPool.h"


bool running = true;
std::shared_ptr<ConnectionPool> connPool;


std::string GetProgramDir(char** argv)
{
    std::string programPath(argv[0]);
    size_t pos = programPath.rfind('/');
    if (pos == programPath.npos)
    {
        std::cerr << "Could get program directory." << std::endl;
        exit(EXIT_FAILURE);
    }

    return programPath.substr(0, pos);
}


std::map<std::string, std::string> ReadConfigFile(std::string filename)
{
    std::map<std::string, std::string> params;

    std::ifstream stream(filename);
    if (stream.is_open())
    {
        std::string line;
        while (std::getline(stream, line))
        {
            if(line.length() == 0 || line.find('#') != line.npos)
                continue;
                
            std::stringstream ss(line);
            std::string name;
            std::string value;

            ss >> name;
            ss >> value;
            params[name] = value;
        }
        stream.close();
    }
    else
    {
        std::cerr << filename << " does not exists." << std::endl;
    }
    
    return params;
}


int main(int argc, char** argv)
{
	std::stringstream ssdbfile;

	// signal hadler for Ctrl + C to stop program
	std::signal(SIGINT, [](int signal){
		running = false;
		std::cout << "Program Interrupted." << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(2));
        exit(EXIT_SUCCESS);
	}); 

	std::cout << "Starting...\n";
	
	std::string programDir = GetProgramDir(argv);

    ssdbfile << programDir << "/config.txt";
    auto dbconfigs = ReadConfigFile(ssdbfile.str());

	auto hit = dbconfigs.find("dbhost");
	auto pit = dbconfigs.find("port");
	auto uit = dbconfigs.find("user");
	auto pwdit = dbconfigs.find("password");
	auto dit = dbconfigs.find("database");
    auto tbit = dbconfigs.find("table");

	if(pit == dbconfigs.end() || hit == dbconfigs.end() || uit == dbconfigs.end() || 
        pwdit == dbconfigs.end() || dit == dbconfigs.end() || tbit == dbconfigs.end())
	{
		std::cerr << "Could not find server/port in config!";
		exit(EXIT_FAILURE);
	}

    int port = std::stoi(dbconfigs.at("port"));
    std::string database = dbconfigs.at("database");
    std::string table = dbconfigs.at("table");

    size_t NUM_CONNS = 3;
    connPool.reset(new ConnectionPool(
                    dbconfigs.at("dbhost"), port, dbconfigs.at("user"), 
                    dbconfigs.at("password"), database, NUM_CONNS));

    if(!connPool->HasActiveConnections())
    {
        std::cerr << "Error Initializing connection pool!" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::cout << "Connection Initialized!" << std::endl;

    size_t conn_id = 0;

    while(running) // Crtl + C to stop
    {
        auto sqlPtr = connPool->GetConnecion();
        std::thread([sqlPtr, table]()
        {
            std::string error;
            std::stringstream ssquery;
            ssquery << "select * from " << sqlPtr->getDatabase() <<".`" << table << "`";
            
            auto results = sqlPtr->selectQuery(ssquery.str(), error);
            if(error.length() > 0)
                std::cout << error << std::endl;
            else
                std::cout << "Results Count " << results.size() << std::endl;

            connPool->ReleaseConnecion(sqlPtr);
        }).detach();
    }

    return 0;
}

