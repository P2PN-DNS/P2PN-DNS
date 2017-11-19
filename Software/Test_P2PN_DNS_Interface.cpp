//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// John W Grun 11/13/2017
// This is the command line interface for the P2PN-DNS. This is only used for 

#include <P2PN-DNS.hpp>
#include <P2PN_DNS_Interface.hpp>


int main (int argc, char *argv[])
{

	std::cout << "Starting up P2PN-DNS" << std::endl;

	auto params = parseArgs(argc,argv);
	if (params.help)
	{
        print_usage();
        return 0;
    }
	// create a distributed hash table
	dht::DhtRunner DNSdht;

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

    
    //Get a domain name ipddress!
    // warning this is a future 
    

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
            std::string v;
            iss >> v;

            bool putstatus = PutDomainName(DNSdht,"www.example.com", std::vector<uint8_t> {v.begin(),v.end()});
            std::cout << "Put status is: " << putstatus << std::endl;
        }
         else if (op == "g") {
            std::string rem;
            std::getline(iss, rem);
            std::string domainlookup = "www.example.com";

            // calls a future in the callback, thus function can return before callback finishes. This is way the string can be empty. 
            
            auto Addressreturned = GetDomainName(DNSdht,domainlookup);
            std::cout << std::string(Addressreturned.begin(), Addressreturned.end()) << std::endl;

            //std::vector<uint8_t> v;
            //uint8_t* a = &v[0];
        }

    }// end while
    //shut the down down properly 
    DNSdht.join();

    std::cout << std::endl <<  "Stopping node..." << std::endl;
    return 0;
}