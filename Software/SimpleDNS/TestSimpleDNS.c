#include <SimpleDNS.h>

int main()
{
  // buffer for input/output binary packet
  uint8_t buffer[BUF_SIZE];
  struct sockaddr_in client_addr;
  socklen_t addr_len = sizeof(struct sockaddr_in);
  struct sockaddr_in addr;
  int nbytes, rc;
  int sock;
  int port = 9000;

  struct Message msg;
  memset(&msg, 0, sizeof(struct Message));

  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port = htons(port);

  sock = socket(AF_INET, SOCK_DGRAM, 0);

  rc = bind(sock, (struct sockaddr*) &addr, addr_len);

  if (rc != 0)
  {
    printf("Could not bind: %s\n", strerror(errno));
    return 1;
  }

  printf("Listening on port %u.\n", port);

  while (1)
  {
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

    resolver_process(&msg);

    /* Print response */
    print_query(&msg);

    uint8_t *p = buffer;
    if (encode_msg(&msg, &p) != 0) {
      continue;
    }

    int buflen = p - buffer;
    sendto(sock, buffer, buflen, 0, (struct sockaddr*) &client_addr, addr_len);
  }
}