
#include <DNS_COMMS.hpp>
#include <iostream> 


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

void sendupdate()
{

	std::vector<uint8_t> IpADDR;
	IpADDR.push_back(192);
	IpADDR.push_back(168);
	IpADDR.push_back(1);
	IpADDR.push_back(3);

	SendDNSUpdate("google.com", IpADDR, "127.0.0.1", 10666 );

}


int main()
{	

	std::cout << "Starting Test_DNSProtocol" << std::endl;
	

	sendupdate();



	std::cout << "Stopping Test_DNSProtocol" << std::endl;
	return 0; 
}

