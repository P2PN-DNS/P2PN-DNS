
#include <DNS_COMMS.hpp>
#include <iostream>
#include <string>
#include <getopt.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sstream>

static const constexpr char* PROMPT = ">> ";

std::string readLine(const char* prefix = PROMPT);

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
	
	while (true)
    {
        // using the GNU Readline API
        std::string line = readLine();
        if (!line.empty() && line[0] == '\0')
            break;

        std::istringstream iss(line);
        std::string op, idstr, value, index, keystr;
        iss >> op;

        if (op == "x" || op == "exit" || op == "quit")
        {
            break;
        }
        else if (op == "p") 
        {
        	std::cout << "DNS server node address" << std::endl;
            std::string v;
            iss >> v;

	        std::vector<uint8_t> IpADDR;
			IpADDR.push_back(192);
			IpADDR.push_back(168);
			IpADDR.push_back(1);
			IpADDR.push_back(3);


	        SendDNSUpdate("google.com", IpADDR, v, 10666 );

        }

    }

	std::cout << "Stopping Test_DNSProtocol" << std::endl;
	return 0; 
}


std::string readLine(const char* prefix)
{
#ifndef WIN32_NATIVE
    const char* line_read = readline(prefix);
    if (line_read && *line_read)
        add_history(line_read);

#else
    char line_read[512];
    std::cout << PROMPT;
    fgets(line_read, 512 , stdin);
#endif
    return line_read ? std::string(line_read) : std::string("\0", 1);
}
