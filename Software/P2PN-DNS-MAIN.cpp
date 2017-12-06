//John Grun 
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

// main function START HERE 
int main (int argc, char *argv[])
{	
	std::cout << "Starting up P2PN-DNS" << std::endl;

	//register signal handler 
	signal(SIGINT, SingnalHandler); 

	// parse inputs from command lie 
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
	// Not implemented yet 

	//enable looging to syslog
	char logname [16];			// why c style strings 
	sprintf(logname, "P2PN-DNS%d",params.port);
	dht::log::enableSyslog(DNSdht,logname);


	// this is the -b argument 
	//we need to manually bootstrap to another node 
	if (not params.bootstrap.first.empty())
	{
    	DNSdht.bootstrap(params.bootstrap.first.c_str(), params.bootstrap.second.c_str());
	}


	// print out information about the node. Mostly garbage as the lib maintainers haven't added anything useful
    print_node_info(DNSdht);
    print_routing_table(DNSdht);

    // start listening on port 53 DNS for DNS updates 

	std::cout << "Starting DNS server on port: " << params.dnsport << std::endl;

	// while loop 
	Run(params.dnsport,ShutdownFlag, UpdateRecord, GetRecord);

	std::cout << "Stopping P2PN-DNS" << std::endl;
	std::cout << std::endl <<  "Stopping node..." << std::endl;

	//join the threads and shutdown gracefully 
	DNSdht.join();

    
    return 0;
}


const std::vector<uint8_t> GetRecord(const std::string & DomainQuery)
{ //Get a domain name ipddress!
	//wrapper function so that I do not need to pass a reference to  an object in the callback. The receiving function is in C
	return GetDomainName(DNSdht,DomainQuery);
}


bool UpdateRecord(const std::string & DomainNameinput, const std::vector<uint8_t> & Ipaddress )
{
	//each octave of the ipaddress as an element in the vector. e.g Ipaddress[0] =192 Ipaddress[1] = 168 .....
	//wrapper function so that I do not need to pass a reference to  an object in the callback. The receiving function is in C
	PutDomainName(DNSdht,DomainNameinput, Ipaddress);
	return true; 
}

void SingnalHandler(int sig)
{ // can be called asynchronously
	std::cout << "Signal caught! Shutting down!" << std::endl;
  	ShutdownFlag = true; // set flag -- this doesn't work for some reason. probably a blocking call in the while loop
  	sleep(1);
  	exit(0);
}