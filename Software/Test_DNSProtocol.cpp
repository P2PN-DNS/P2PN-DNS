
#include <DNS_COMMS.hpp>
#include <iostream>
#include <string>
#include <getopt.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sstream>


const std::vector<uint8_t> TestGetrecordcallback(const std::string & testinput)
{
	
	std::cout << testinput << std::endl;

	std::vector <uint8_t> testoutput;
	testoutput.push_back(192);
	testoutput.push_back(168);
	testoutput.push_back(1);
	testoutput.push_back(1);

	return testoutput; 
}

bool TestUpdaterecordcallback(const std::string & testinput, const std::vector<uint8_t> & testinputVector )
{

	std::cout << testinput << std::endl;
	//std::cout << std::string(testinputVector) << std::endl;
	return true; 
}

void RunServer()
{
	bool shutdown = false; 
	Run(10666,shutdown, TestUpdaterecordcallback, TestGetrecordcallback);
}


int main(int argc, char *argv[])
{	

	std::cout << "Starting Test_DNSProtocol" << std::endl;
	
	RunServer();

	std::cout << "Stopping Test_DNSProtocol" << std::endl;
	return 0; 
}
