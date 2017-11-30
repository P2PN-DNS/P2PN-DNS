
#include <DNS_COMMS.hpp>


//this may change on different systems 
socklen_t addr_len = sizeof(struct sockaddr_in);

int CreateUDPSocketHandle(int port = 53)
{ //
	// create a socket 

  int nbytes;
  int rc;
  int sockhandle;
  struct sockaddr_in addr;


	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(port);

	sockhandle = socket(AF_INET, SOCK_DGRAM, 0);

	
	if (bind(sockhandle, (struct sockaddr*) &addr, addr_len) != 0)
	{
		throw std::runtime_error("Could not bind socket to port: " + port); // + " with Error: " + std::string(strerror(errno)));
	}

	return sockhandle;
}


int Run(int port, bool& shutdown, UpdateRecordCallback_t UpdateRecordCallback, GetRecordCallback_t GetRecordCallback)
{
	/// called as a thread to run the DNS server in the background. pass in the callbacks so this class does not need to know the anything about other classes using it. 
	

	// network structures
	struct sockaddr_in client_addr = {};
	//struct sockaddr_in addr = {};
	int nbytes = {};

	int sock = -1; // declare sock here outside of the try catch black.
	sock = CreateUDPSocketHandle(port);
	
	int status = 0;

	while (shutdown == false )
	{
		// init with a blank initializer list
		struct Message msg = {};

		// message buffer
		uint8_t buffer[BUF_SIZE];

		// FIXME need to look at how this is freed 
		//free_questions(msg.questions);
		//free_resource_records(msg.answers);
		//free_resource_records(msg.authorities);
		//free_resource_records(msg.additionals);
		//memset(&msg, 0, sizeof(struct Message));

		// get the incoming packet on this port. record the client address so we can respond back
		nbytes = recvfrom(sock, buffer, sizeof(buffer), 0, (struct sockaddr *) &client_addr, &addr_len);

		std::cout << "Got Message!" << std::endl;
		print_hex(buffer, nbytes);
		std::cout << std::endl;

		if (decode_msg(&msg, buffer, nbytes) != 0) 
		{
			continue;
		}

		/* Print query */
		std::cout << std::endl;
		print_query(&msg);
		std::cout << std::endl;

		// need to change this to use the GetDomainName
		//resolver_process(&msg);
		if ((msg.anCount + msg.nsCount) == 0)
		{
			resolver_process(&msg, GetRecordCallback);
		}
		else
		{	
			updater_process(&msg, UpdateRecordCallback);
		}

		/* Print response */
		std::cout << std::endl;
		print_query(&msg);
		std::cout << std::endl;

		uint8_t *p = buffer;
		if (encode_msg(&msg, &p) != 0) 
		{
			continue;
		}


		int buflen = p - buffer;

		std::cout << "sending Message!" << std::endl;
		std::cout << std::endl;
		print_hex(buffer, buflen);

		sendto(sock, buffer, buflen, 0, (struct sockaddr*) &client_addr, addr_len);
	} // end while(1)

		//free dynamic memory 
		// free_questions(msg.questions);
		// free_resource_records(msg.answers);
		// free_resource_records(msg.authorities);
		// free_resource_records(msg.additionals);

	return status;
} // end Run

int SendDNSUpdate(const std::string& Domainname, const std::vector<uint8_t> & IpADDR, const std::string& DestinationIP, const int & port  )
{

	struct sockaddr_in client_addr = {};
	//struct sockaddr_in addr = {};
	int nbytes = {};

	int sock = -1; // declare sock here outside of the try catch black.
	
	// init with a blank initializer list
	struct Message msg = {};

	// message buffer
	uint8_t buffer[BUF_SIZE];


	// set the destination ip and port. Not checked!
	memset((char *) &client_addr, 0, sizeof(client_addr));
    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(port);

    if (inet_aton(DestinationIP.c_str(), &client_addr.sin_addr)==0) 
    {
       throw std::runtime_error("Invalid Ip or Port format! inet_aton() failed");
    }

    // create socket 0n random port 
    sock = CreateUDPSocketHandle(0);

    // set packet 
    msg.id = 0xAAAA;
	msg.opcode = UPDATE_OperationCode;
	msg.qr = 0; // this is not a response
	msg.aa = 1; // this server is authoritative
	msg.ra = 0; // no recursion available
	msg.rcode = Ok_ResponseType;

  // should already be 0
	msg.anCount = 0;
	msg.nsCount = 0;
	msg.arCount = 0;

	//msg.questions->qType = A_Resource_RecordType;

	//create resource record
	struct ResourceRecord* rr;
	rr = (ResourceRecord*)malloc(sizeof(struct ResourceRecord));
	memset(rr, 0, sizeof(struct ResourceRecord));

	// not checking domain name here either good luck!
	rr->name = strdup(Domainname.c_str());
	rr->type = A_Resource_RecordType;
	rr->Rclass = IXFR_QueryType;
	rr->ttl = 60*60; // in seconds; 0 means no caching

	//printf("Update for '%s'\n", Domainname.c_str());

	// We only can only answer two question types so far
	// and the answer (resource records) will be all put
	// into the answers list.
	// This behavior is probably non-standard!

	rr->rd_length = 4;
	//rc = get_A_Record(rr->rd_data.a_record.addr, q->qName);
	//if(IpADDR.size() > 3) // bad way to eval this
	//{
		rr->rd_data.a_record.addr[0] = IpADDR[0];
		rr->rd_data.a_record.addr[1] = IpADDR[1];
		rr->rd_data.a_record.addr[2] = IpADDR[2];
		rr->rd_data.a_record.addr[3] = IpADDR[3];
	//}

	printf("Update for '%s' with address %d .%d. %d. %d \n", Domainname.c_str(),rr->rd_data.a_record.addr[0], rr->rd_data.a_record.addr[1],rr->rd_data.a_record.addr[2],rr->rd_data.a_record.addr[3] );
	msg.anCount++;

    // prepend resource record to answers list
    msg.answers = rr;

    // encode packet 
	uint8_t *p = buffer;
	if (encode_msg(&msg, &p) != 0) 
	{
		std::cout << "Something Happened!" << std::endl;
	}

	/* Print query */
	std::cout << std::endl;
	print_query(&msg);
	std::cout << std::endl;

	int buflen = p - buffer;
	if(sendto(sock, buffer, buflen, 0, (struct sockaddr*) &client_addr, addr_len) < 0)
	{
		throw std::runtime_error("Send failed!!");
	}

	return 0;
}
