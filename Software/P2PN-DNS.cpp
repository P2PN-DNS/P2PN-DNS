
#include <P2PN-DNS.hpp>


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
    //dht.bootstrap(params.bootstrap.first.c_str(), params.bootstrap.second.c_str());

    print_node_info(DNSdht);
    print_routing_table(DNSdht);


    // start listening on port 53 DNS for DNS updates 

    //update dns record
    PutDomainName(DNSdht,"www.example.com", "192.167.111.56");

    //Get a domain name ipddress!
    // warning this is a future 
    GetDomainName(DNSdht,"www.example.com");


    sleep(2);
    //shut the down down properly 
    DNSdht.join();

    std::cout << std::endl <<  "Stopping node..." << std::endl;
    return 0;
}


///////////////////////////////////////////////////////////// Begin Functions /////////////////////////////////////////////////////////////////

void print_usage() 
{
	std::cout << "Usage: P2PN-DNS [-n network_id] [-p local_port] [-b bootstrap_host[:port]]" << std::endl;
}

void print_node_info(const dht::DhtRunner& DNSdht)
{
    std::cout << "OpenDNSdht node " << DNSdht.getNodeId() << " running on port " <<  DNSdht.getBoundPort() << std::endl;
    std::cout << "Public key ID " << DNSdht.getId() << std::endl;
    std::cout << "IPv4 stats:" << std::endl;
    std::cout << DNSdht.getNodesStats(AF_INET).toString() << std::endl;
    std::cout << "IPv6 stats:" << std::endl;
    std::cout << DNSdht.getNodesStats(AF_INET6).toString() << std::endl;
}

void print_routing_table(const dht::DhtRunner& DNSdht)
{
	std::cout << "IPv4 routing table:" << std::endl;
	std::cout << DNSdht.getRoutingTablesLog(AF_INET) << std::endl;
	std::cout << "IPv6 routing table:" << std::endl;
	std::cout << DNSdht.getRoutingTablesLog(AF_INET6) << std::endl;
}


dht_params parseArgs(int argc, char **argv) 
{
    dht_params params;
    int opt;
    // while ((opt = getopt_long(argc, argv, "hidsvp:n:b:l:", long_options, nullptr)) != -1) 
    // {
    //     switch (opt) {
    //     case 'p': {
    //             int port_arg = atoi(optarg);
    //             if (port_arg >= 0 && port_arg < 0x10000)
    //                 params.port = port_arg;
    //             else
    //                 std::cout << "Invalid port: " << port_arg << std::endl;
    //         }
    //         break;
    //     case 'S': {
    //             int port_arg = atoi(optarg);
    //             if (port_arg >= 0 && port_arg < 0x10000)
    //                 params.proxyserver = port_arg;
    //             else
    //                 std::cout << "Invalid port: " << port_arg << std::endl;
    //         }
    //         break;
    //     case 'n':
    //         //params.network = strtoul(optarg, nullptr, 0);
    //         break;
    //     case 'b':
    //         params.bootstrap = splitPort((optarg[0] == '=') ? optarg+1 : optarg);
    //         if (not params.bootstrap.first.empty() and params.bootstrap.second.empty()) {
    //             params.bootstrap.second = std::to_string(DHT_DEFAULT_PORT);
    //         }
    //         break;
    //     case 'h':
    //         params.help = true;
    //         break;
    //     case 'l':
    //         params.logfile = optarg;
    //         break;
    //     case 'L':
    //         params.log = true;
    //         params.syslog = true;
    //         break;
    //     case 'v':
    //         params.log = true;
    //         break;
    //     case 'i':
    //         params.generate_identity = true;
    //         break;
    //     case 'd':
    //         params.daemonize = true;
    //         break;
    //     case 's':
    //         params.service = true;
    //         break;
    //     default:
    //         break;
    //     }
    // }
    return params;
}


bool PutDomainName(dht::DhtRunner& DNSdht, const std::string& DomainName, const std::string& IPAddress)
{
	
	bool OperationStatus = false;
	//domain name needs to be all lower case and validated. 
	//ValidateDomainName();
	//ValidateIPAddress();

	DNSdht.put(dht::InfoHash::get(DomainName), dht::Value {std::vector<uint8_t> {IPAddress.begin(), IPAddress.end()}}, [&OperationStatus](bool ok) {OperationStatus = ok;});

				//dht::Value::Value(const std::vector<uint8_t>&) or C-style with dht::Value::Value(const uint8_t* ptr, size_t len).
	return OperationStatus;
}

const std::string GetDomainName(dht::DhtRunner& DNSdht, const std::string& DomainName)
{

	DNSdht.get(dht::InfoHash::get(DomainName),[](const std::vector<std::shared_ptr<dht::Value>>& values) {
        for (const auto& v : values)
            std::cout << "Got value: " << *v << std::endl;
        return true; // keep looking for values
    },
    [](bool success) {
        std::cout << "Get finished with " << (success ? "success" : "failure") << std::endl;
    }
);

    return std::string("NULL");

}