
#include <P2PN-DNS.hpp>

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

bool PutDomainName(dht::DhtRunner& DNSdht, const std::string& DomainName, const std::string& IPAddress)
{
	
	bool OperationStatus = false;
	//domain name needs to be all lower case and validated. 
	//ValidateDomainName();
	//ValidateIPAddress();

	DNSdht.put(dht::InfoHash::get(DomainName), dht::Value {std::vector<uint8_t> {IPAddress.begin(), IPAddress.end()}}, [&OperationStatus](bool ok) {OperationStatus = ok;});

	return OperationStatus;
}

const std::string GetDomainName(dht::DhtRunner& DNSdht, const std::string& DomainName)
{

	std::string IpAddress;

	DNSdht.get(dht::InfoHash::get(DomainName),[&IpAddress](const std::vector<std::shared_ptr<dht::Value>>& values)
	{


        //for (const auto& v : values)
        //{

        	

			// get the last value in the vector of shared pointers 
			// get the underlying value object. 
			// data is the name of the field 
			// Blob is a fucking std::vector<uint8_t>   fucking write docs 

        	IpAddress = std::string(values.at(values.size()-1).get()->data.begin(), values.at(values.size()-1).get()->data.end());
            std::cout << "Got value: " << IpAddress << std::endl;
       // }

        return true; // keep looking for values
    },
    [](bool success) {
        std::cout << "Get finished with " << (success ? "success" : "failure") << std::endl;
    }
);

	// aync as fuck!!!!!!!!!!!
	std::cout << "Value outside of callback is: " << IpAddress << std::endl;

    return IpAddress;

}
