//John Grun
#include <DNS_COMMS.hpp>
#include <iostream>
#include <string>
#include <getopt.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sstream>

static const constexpr char* PROMPT = ">> ";

std::string readLine(const char* prefix = PROMPT);
/// test callback 
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
/// test callback 
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

// the main function for the DNS update 
int main(int argc, char *argv[])
{	

	std::cout << "Starting Test_DNSProtocol" << std::endl;
	
	while (true)
    {
    	std::cout << "Usage : S DomainName IPAddress DNSserverNodeIP DnsServerPort" <<std::endl;
        // using the GNU Readline API
        std::string line = readLine();
        if (!line.empty() && line[0] == '\0')
            break;

        std::istringstream iss(line);
        std::string op, idstr, value, index, keystr;
        iss >> op;

        std::cout << "Your line was : " << line << std::endl;
        std::cout << " op was : " << op << std::endl;

        if (op == "x" || op == "exit" || op == "quit")
        {
            break;
        }
        else if (op == "S") 
        {
        	// get the parameters from the user. No validation at all!
            std::string DOMAINNAME, IPADDRESS, DNSSERVERIP, PORT;
            iss >> DOMAINNAME;
            iss >> IPADDRESS;
            iss >> DNSSERVERIP;
            iss >> PORT;

			std::istringstream ss(IPADDRESS);
			std::string token; 

			
			std::vector<uint8_t> IpADDR;
			// convert to ip octaves from string 
			while(std::getline(ss, token, '.'))
			{ // use getline to split by .
				std::stringstream IPOCTAVE(token);
				int IPBYTE =0;
				IPOCTAVE >> IPBYTE;
			    IpADDR.push_back(IPBYTE);
			}

			if( IpADDR.size() != 4)
			{ // we have an issue!
				std::cout << "Invalid Ip address format!" << std::endl;
				break;
			}
			// convert to a number without any form of validation or checking!
            std::stringstream PORTSTREAM(PORT);
            int PortInt = 0;
            PORTSTREAM >> PortInt;
            
	        SendDNSUpdate(DOMAINNAME, IpADDR, DNSSERVERIP, PortInt );
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
