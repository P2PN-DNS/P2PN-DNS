
#include <SimpleDNS.h>
#include <std::exception>

//this may change on different systems 
constexpr socklen_t addr_len = sizeof(struct sockaddr_in);

//int port = 9000;

int CreateUDPSocketHandle(int port = 10666)
{
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
		throw std::runtime_error("Could not bind socket to port: " + port + " with Error: " + strerror(errno));
	}

	return sockhandle;
}

int Run(int port, bool& shutdown)
{
	
	int sock = CreateUDPSocketHandle(port);
	struct sockaddr_in client_addr;
	struct sockaddr_in addr;
	int nbytes;

	struct Message msg;
	memset(&msg, 0, sizeof(struct Message));
	uint8_t buffer[BUF_SIZE];

	int status = 0;

	while (shutdown == false )
	{
		// FIXME need to look at how this is freed 
		free_questions(msg.questions);
		free_resource_records(msg.answers);
		free_resource_records(msg.authorities);
		free_resource_records(msg.additionals);
		memset(&msg, 0, sizeof(struct Message));

		nbytes = recvfrom(sock, buffer, sizeof(buffer), 0, (struct sockaddr *) &client_addr, &addr_len);

		if (decode_msg(&msg, buffer, nbytes) != 0) {
			continue;
		}

		/* Print query */
		print_query(&msg);

		// need to change this to use the GetDomainName
		resolver_process(&msg);

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
		free_questions(msg.questions);
		free_resource_records(msg.answers);
		free_resource_records(msg.authorities);
		free_resource_records(msg.additionals);

	return status;
} // end Run