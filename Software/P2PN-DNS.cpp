
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
	 dht::DhtRunner dht;

	// start the DHT backend
	dht.run(params.port, dht::crypto::generateIdentity("P2PN-DNS"), true);

	//locate other nodes 
	// steal from scanner code 
    //dht.bootstrap(params.bootstrap.first.c_str(), params.bootstrap.second.c_str());

    print_node_info(dht);



    std::cout << std::endl <<  "Stopping node..." << std::endl;
    dht.join();
    return 0;
}


///////////////////////////////////////////////////////////// Begin Functions /////////////////////////////////////////////////////////////////

void print_usage() 
{
	std::cout << "Usage: P2PN-DNS [-n network_id] [-p local_port] [-b bootstrap_host[:port]]" << std::endl;
}

void print_node_info(const dht::DhtRunner& dht)
{
    std::cout << "OpenDht node " << dht.getNodeId() << " running on port " <<  dht.getBoundPort() << std::endl;
    std::cout << "Public key ID " << dht.getId() << std::endl;
}


dht_params parseArgs(int argc, char **argv) {
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
