#include <DNS_COMMS.hpp>
#include <P2PN-DNS.hpp>
#include <P2PN_DNS_Interface.hpp>
#include <iostream>  // iostream
#include <signal.h> // handle signals 

// shutdown flag for the entire program 
bool ShutdownFlag = false; 

// create a distributed hash table
dht::DhtRunner DNSdht;

//foward all the declarations!
const std::vector<uint8_t> GetRecord(const std::string & DomainQuery);
bool UpdateRecord(const std::string & DomainNameinput, const std::vector<uint8_t> & Ipaddress );
void SingnalHandler(int sig);

int main (int argc, char *argv[])
{	
	std::cout << "Starting up P2PN-DNS" << std::endl;

	//register signal handler 
	signal(SIGINT, SingnalHandler); 

	// parse inputs
	auto params = parseArgs(argc,argv);
	if (params.help)
	{
        print_usage();
        return 0;
    }

	// start the DHT backend
	DNSdht.run(params.port, dht::crypto::generateIdentity("P2PN-DNS"), true);

	//locate other nodes 
	// steal from scanner code 

	// this is the -b argument 
	if (not params.bootstrap.first.empty())
	{
    	DNSdht.bootstrap(params.bootstrap.first.c_str(), params.bootstrap.second.c_str());
	}

    print_node_info(DNSdht);
    print_routing_table(DNSdht);

    // start listening on port 53 DNS for DNS updates 

    //update dns record

	std::cout << "Starting DNS server" << std::endl;

	std::vector<uint8_t> test;
	test.push_back(6);
	test.push_back(6);
	test.push_back(6);
	test.push_back(9);
	
	UpdateRecord("google.com",test);
	
	// while loop 
	Run(10666,ShutdownFlag, UpdateRecord, GetRecord);

	std::cout << "Stopping Test_DNSProtocol" << std::endl;
	std::cout << std::endl <<  "Stopping node..." << std::endl;
	DNSdht.join();

    
    return 0;
}


const std::vector<uint8_t> GetRecord(const std::string & DomainQuery)
{ //Get a domain name ipddress!
	return GetDomainName(DNSdht,DomainQuery);
}


bool UpdateRecord(const std::string & DomainNameinput, const std::vector<uint8_t> & Ipaddress )
{
	//each octave of the ipaddress as an element in the vector. e.g Ipaddress[0] =192 Ipaddress[1] = 168 .....

	PutDomainName(DNSdht,DomainNameinput, Ipaddress);
	return true; 
}

void SingnalHandler(int sig)
{ // can be called asynchronously
	std::cout << "Signal caught! Shutting down" << std::endl;
  	ShutdownFlag = true; // set flag
  	exit(0);
}