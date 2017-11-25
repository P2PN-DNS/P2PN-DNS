
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
	struct sockaddr_in addr = {};
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

		if (decode_msg(&msg, buffer, nbytes) != 0) 
		{
			continue;
		}

		/* Print query */
		//print_query(&msg);

		// need to change this to use the GetDomainName
		//resolver_process(&msg);
		resolver_process(&msg, GetRecordCallback);

		/* Print response */
		//print_query(&msg);

		uint8_t *p = buffer;
		if (encode_msg(&msg, &p) != 0) 
		{
			continue;
		}

		int buflen = p - buffer;
		sendto(sock, buffer, buflen, 0, (struct sockaddr*) &client_addr, addr_len);
	} // end while(1)

		//free dynamic memory 
		// free_questions(msg.questions);
		// free_resource_records(msg.answers);
		// free_resource_records(msg.authorities);
		// free_resource_records(msg.additionals);

	return status;
} // end Run

