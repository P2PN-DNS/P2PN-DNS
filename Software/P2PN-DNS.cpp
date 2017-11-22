
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

void print_IpAddress(std::vector<uint8_t> Addr)
{

	std::cout << "print_IpAddress was called. IP address is print_IpAddress: "<< std::string{Addr.begin(),Addr.end()} << std::endl;
}

//bool PutDomainName(dht::DhtRunner& DNSdht, const std::string& DomainName, const std::string& IPAddress)
bool PutDomainName(dht::DhtRunner& DNSdht, const std::string& DomainName, const std::vector<uint8_t>& IPAddress)
{
	
	bool OperationStatus = false;
	//domain name needs to be all lower case and validated. 
	//ValidateDomainName();
	//ValidateIPAddress();

	//DNSdht.put(dht::InfoHash::get(DomainName), dht::Value {std::vector<uint8_t> {IPAddress.begin(), IPAddress.end()}}, [&OperationStatus](bool ok) {OperationStatus = ok;});

	DNSdht.put(dht::InfoHash::get(DomainName), dht::Value {IPAddress}, [&OperationStatus](bool ok) {OperationStatus = ok;});

	return OperationStatus;
}

const std::vector<uint8_t> GetDomainName(dht::DhtRunner& DNSdht, const std::string& DomainName)
{

	
	// mutex lock 
	std::mutex cv_m;
	std::unique_lock<std::mutex> lock(cv_m);

	// condtional variable to make this dumb function wait on a return value from the dht.get call
	std::condition_variable cv;
	//function handle to a lambda 
	// pass in the condtional variable to the "done" callback
	std::function<void(bool)> Callbackhandle = [&cv](bool done) { cv.notify_all(); };

	std::vector<uint8_t> IpAddress;

	DNSdht.get(dht::InfoHash::get(DomainName),[&IpAddress](const std::vector<std::shared_ptr<dht::Value>>& values)
	{
        //for (const auto& v : values)
        //{
			// get the last value in the vector of shared pointers 
			// get the underlying value object. 
			// data is the name of the field 
			// Blob is a fucking std::vector<uint8_t>   fucking write docs 
			IpAddress = values.at(values.size()-1).get()->data;
        	//IpAddress = std::string(values.at(values.size()-1).get()->data.begin(), values.at(values.size()-1).get()->data.end());
            //std::cout << "Got value: " << IpAddress << std::endl;
       // }
        return true; // keep looking for values
    }, Callbackhandle);

	// force the function to wait for the conditional notify_all() that is called in the done call back. This way we know that IPAddress has a value
	cv.wait(lock);

    return IpAddress;

}
