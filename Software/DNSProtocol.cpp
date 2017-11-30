#include <DNSProtocol.hpp>

/*
* Basic memory operations.
*/
size_t get32bits( const uint8_t** buffer )
{
  uint32_t value;

  memcpy( &value, *buffer, 4 );
  *buffer += 4;

  return ntohs( value );
}

size_t get16bits( const uint8_t** buffer )
{
  uint16_t value;

  memcpy( &value, *buffer, 2 );
  *buffer += 2;

  return ntohs( value );
}

size_t get8bits( const uint8_t** buffer )
{
  uint8_t value;

  memcpy( &value, *buffer, 1 );
  *buffer += 1;

  return value ;
}

void put8bits( uint8_t** buffer, uint8_t value )
{
  memcpy( *buffer, &value, 1 );
  *buffer += 1;
}

void put16bits( uint8_t** buffer, uint16_t value )
{
  value = htons( value );
  memcpy( *buffer, &value, 2 );
  *buffer += 2;
}

void put32bits( uint8_t** buffer, uint32_t value )
{
  value = htons( value );
  memcpy( *buffer, &value, 4 );
  *buffer += 4;
}


/*
* Deconding/Encoding functions.
*/

// 3foo3bar3com0 => foo.bar.com
char* decode_domain_name(const uint8_t** buffer)
{
  char name[256];
  const uint8_t* buf = *buffer;
  int j = 0;
  int i = 0;

  while (buf[i] != 0)
  {
    //if(i >= buflen || i > sizeof(name))
    //  return NULL;

    if (i != 0)
    {
      name[j] = '.';
      j += 1;
    }

    int len = buf[i];
    i += 1;

    memcpy(name+j, buf+i, len);
    i += len;
    j += len;
  }

  name[j] = '\0';

  *buffer += i + 1; //also jump over the last 0

  return strdup(name);
}

// foo.bar.com => 3foo3bar3com0
void encode_domain_name(uint8_t** buffer, const char* domain)
{
  uint8_t* buf = *buffer;
  const char* beg = domain;
  const char* pos;
  int len = 0;
  int i = 0;

  while ((pos = strchr(beg, '.')))
  {
    len = pos - beg;
    buf[i] = len;
    i += 1;
    memcpy(buf+i, beg, len);
    i += len;

    beg = pos + 1;
  }

  len = strlen(domain) - (beg - domain);

  buf[i] = len;
  i += 1;

  memcpy(buf + i, beg, len);
  i += len;

  buf[i] = 0;
  i += 1;

  *buffer += i;
}
  

void decode_header(struct Message* msg, const uint8_t** buffer)
{
  msg->id = get16bits(buffer);

  uint32_t fields = get16bits(buffer);
  msg->qr = (fields & QR_MASK) >> 15;
  msg->opcode = (fields & OPCODE_MASK) >> 11;
  msg->aa = (fields & AA_MASK) >> 10;
  msg->tc = (fields & TC_MASK) >> 9;
  msg->rd = (fields & RD_MASK) >> 8;
  msg->ra = (fields & RA_MASK) >> 7;
  msg->rcode = (fields & RCODE_MASK) >> 0;

  msg->qdCount = get16bits(buffer);
  msg->anCount = get16bits(buffer);
  msg->nsCount = get16bits(buffer);
  msg->arCount = get16bits(buffer);
}

void encode_header(struct Message* msg, uint8_t** buffer)
{
  put16bits(buffer, msg->id);

  int fields = 0;
  fields |= (msg->qr << 15) & QR_MASK;
  fields |= (msg->rcode << 0) & RCODE_MASK;
  // TODO: insert the rest of the fields
  put16bits(buffer, fields);

  put16bits(buffer, msg->qdCount);
  put16bits(buffer, msg->anCount);
  put16bits(buffer, msg->nsCount);
  put16bits(buffer, msg->arCount);
}

int decode_msg(struct Message * msg, const uint8_t* buffer, int size)
{

  decode_header(msg, &buffer);

  //debugging
  // std::cout << " opcode is: " << msg->opcode << std::endl;
  // std::cout << " anCount is: " << msg->anCount << std::endl;
  // std::cout << " nsCount is: " << msg->nsCount << std::endl;
    //DNS UPDATE?//00:00:28:00:00:01:00:00:00:01:00:00:07:65:78:61:6d:70:6c:65:03:63:6f:6d:00:00:06:00:01:05:70:77:6e:65:64:07:65:78:61:6d:70:6c:65:03:63:6f:6d:00:00:01:00:01:00:00:00:3c:00:04:89:89:89:89
    //MY UPDATE //00:00:00:00:00:00:00:01:00:00:00:00:06:67:6f:6f:67:6c:65:03:63:6f:6d:00:00:01:00:fb:0e:10:00:00:00:04:c0:a8:01:03
    //Query    //
  if ((msg->anCount + msg->nsCount) == 0)
  { // this is a query!

  std::cout << " Query record " << std::endl;
  // parse questions
  uint32_t qcount = msg->qdCount;
  struct Question* qs = msg->questions;
  //struct Question* q;

    for (unsigned int QuestionIndex = 0; QuestionIndex < qcount; ++QuestionIndex)
    {
      struct Question* q = (Question*)malloc(sizeof(struct Question));

      //struct Question* q = {};

      q->qName = decode_domain_name(&buffer);
      q->qType = get16bits(&buffer);
      q->qClass = get16bits(&buffer);

      //prepend question to questions list
      q->next = qs;
      msg->questions = q;
    }
  }
  else
  {// this an update

      //create resource record
      std::cout << "Update record" << std::endl;
      struct ResourceRecord* rr;
      rr = (ResourceRecord*)malloc(sizeof(struct ResourceRecord));
      memset(rr, 0, sizeof(struct ResourceRecord));


       //we have a rr 
      rr->name = decode_domain_name(&buffer);
      rr->type = get16bits(&buffer);
      rr->Rclass = get16bits(&buffer);
      rr->ttl = get32bits(&buffer);
      rr->rd_length = get16bits(&buffer);

      rr->rd_data.a_record.addr[0] = get8bits(&buffer);      
      rr->rd_data.a_record.addr[1] = get8bits(&buffer);
      rr->rd_data.a_record.addr[2] = get8bits(&buffer);
      rr->rd_data.a_record.addr[3] = get8bits(&buffer);
     
      // switch (rr->type)
      // {
      // case A_Resource_RecordType:
      //   for(int i = 0; i < 4; ++i)
      //     //put8bits(&p, rr->rd_data.a_record.addr[i]);
      //     memcpy( *buffer + (void*)i, &rr->rd_data.a_record.addr[i], 1 );
      //   break;
      // case AAAA_Resource_RecordType:
      //   for(int i = 0; i < 16; ++i)
      //     //put8bits(&p, rr->rd_data.aaaa_record.addr[i]);
      //     memcpy( *buffer + (void*)i, &rr->rd_data.a_record.addr[i], 1 );
      //   break;
      // default:
      //   //fprintf(stderr, "Unknown type %u. => Ignore resource record.\n", rr->type);
      // return 1;
      // } //end switch


      // std::cout << " Domain name is: " << std::string(rr->name) << std::endl;
      // std::cout << " type is: " <<  rr->type << std::endl;
      // std::cout << " Rclass is: " <<  rr->Rclass << std::endl;
      // std::cout << " ttl is: " <<  rr->ttl << std::endl;
      // std::cout << " rd_length  is: " <<  rr->rd_length  << std::endl;
      // std::cout << "IP Address is: " << std::endl;
      // std::cout << (int)rr->rd_data.a_record.addr[0] << std::endl;
      // std::cout << (int)rr->rd_data.a_record.addr[1] << std::endl;
      // std::cout << (int)rr->rd_data.a_record.addr[2] << std::endl;
      // std::cout << (int)rr->rd_data.a_record.addr[3] << std::endl;


    // We only can only answer two question types so far
    // and the answer (resource records) will be all put
    // into the answers list.
    // This behavior is probably non-standard!

      
    //rc = get_A_Record(rr->rd_data.a_record.addr, q->qName);

      // rr->rd_data.a_record.addr[0] = IpADDR[0];
      // rr->rd_data.a_record.addr[1] = IpADDR[1];
      // rr->rd_data.a_record.addr[2] = IpADDR[2];
      // rr->rd_data.a_record.addr[3] = IpADDR[3];

      // prepend resource record to answers list
      msg->answers = rr;

  }

  // We do not expect any resource records to parse here.

  return 0;
}

// For every question in the message add a appropiate resource record
// in either section 'answers', 'authorities' or 'additionals'.
void resolver_process(struct Message* msg, GetRecordCallback_t GetRecordCallback)
{
  
  
  struct Question* q;
  struct ResourceRecord* rr;
  struct ResourceRecord* beg;
  int rc;

  // leave most values intact for response
  msg->qr = 1; // this is a response
  msg->aa = 1; // this server is authoritative
  msg->ra = 0; // no recursion available
  msg->rcode = Ok_ResponseType;

  // should already be 0
  msg->anCount = 0;
  msg->nsCount = 0;
  msg->arCount = 0;

  // for every question append resource records
  q = msg->questions;
  while(q)
  {
    rr = (ResourceRecord*)malloc(sizeof(struct ResourceRecord));
    memset(rr, 0, sizeof(struct ResourceRecord));

    rr->name = strdup(q->qName);
    rr->type = q->qType;
    rr->Rclass = q->qClass;
    rr->ttl = 60*60; // in seconds; 0 means no caching

    printf("Query for '%s'\n", q->qName);

    // We only can only answer two question types so far
    // and the answer (resource records) will be all put
    // into the answers list.
    // This behavior is probably non-standard!
    switch (q->qType)
    {
      case A_Resource_RecordType:
      {
        rr->rd_length = 4;
        //rc = get_A_Record(rr->rd_data.a_record.addr, q->qName);

        rc = 1;
        // needs to return this 
        std::cout << "Get record" << std::endl;
        // stupid way to do this FIXME !! uint8_t addr[4];
        auto temp = GetRecordCallback(std::string(q->qName));

        if(temp.size() > 3) // bad way to eval this
        {
          std::cout << "Record found" << std::endl;
          rr->rd_data.a_record.addr[0] = temp[0];
          rr->rd_data.a_record.addr[1] = temp[1];
          rr->rd_data.a_record.addr[2] = temp[2];
          rr->rd_data.a_record.addr[3] = temp[3];
        }
        else 
        {
          std::cout << "Record was not found!" << std::endl;
          rr->rd_data.a_record.addr[0] = 0;
          rr->rd_data.a_record.addr[1] = 0;
          rr->rd_data.a_record.addr[2] = 0;
          rr->rd_data.a_record.addr[3] = 0;
        }


        if (rc < 0)
        {
          std::cout << "Freeing rr " << std::endl;
          free(rr->name);
          free(rr);
          goto next;
        }
        break;
      }
      // case AAAA_Resource_RecordType:
      //   rr->rd_length = 16;
      //   rc = get_AAAA_Record(rr->rd_data.aaaa_record.addr, q->qName);
      //   if (rc < 0)
      //   {
      //     free(rr->name);
      //     free(rr);
      //     goto next;
      //   }
      //   break;
      /*
      case NS_Resource_RecordType:
      case CNAME_Resource_RecordType:
      case SOA_Resource_RecordType:
      case PTR_Resource_RecordType:
      case MX_Resource_RecordType:
      case TXT_Resource_RecordType:
      */
      default:
      {
        free(rr);
        msg->rcode = NotImplemented_ResponseType;
        printf("Cannot answer question of type %d.\n", q->qType);
        goto next;
      }
    }

    msg->anCount++;

    // prepend resource record to answers list
    beg = msg->answers;
    msg->answers = rr;
    rr->next = beg;

    //jump here to omit question
    next:

    // process next question
    q = q->next;
  } // end while
}

void updater_process(struct Message* msg, UpdateRecordCallback_t UpdateRecordCallback)
{

  // leave most values intact for response
  msg->qr = 1; // this is a response
  msg->aa = 1; // this server is authoritative
  msg->ra = 0; // no recursion available
  msg->rcode = Ok_ResponseType;

  // should already be 0
  msg->anCount = 0;
  msg->nsCount = 0;
  msg->arCount = 0;

  // for every question append resource records
  // while(msg->answers)
  // {
    // rr = (ResourceRecord*)malloc(sizeof(struct ResourceRecord));
    // memset(rr, 0, sizeof(struct ResourceRecord));

    //rr->name = strdup(msg->answers.qName);
    // rr->type = q->qType;
    // rr->Rclass = q->qClass;
    // rr->ttl = 60*60; // in seconds; 0 means no caching

    printf("update for '%s'\n", msg->answers->name);

    // We only can only answer two question types so far
    // and the answer (resource records) will be all put
    // into the answers list.
    // This behavior is probably non-standard!
    // switch (msg->answers.qType)
    // {
      //case A_Resource_RecordType:
      //{
        //rr->rd_length = 4;
        //rc = get_A_Record(rr->rd_data.a_record.addr, q->qName);

        //rc = 1;
        // needs to return this 

        // stupid way to do this FIXME !! uint8_t addr[4];
        
        std::vector<uint8_t> ipaddrUpdate;
        // host vs network order. very annoying!!!!
        ipaddrUpdate.push_back(msg->answers->rd_data.a_record.addr[0] );
        ipaddrUpdate.push_back(msg->answers->rd_data.a_record.addr[1] );
        ipaddrUpdate.push_back(msg->answers->rd_data.a_record.addr[2] );
        ipaddrUpdate.push_back(msg->answers->rd_data.a_record.addr[3] );

        //td::function<bool(const std::string&, const std::vector<uint8_t>& )>;
        UpdateRecordCallback(msg->answers->name, ipaddrUpdate);
    //}
}

/* @return 0 upon failure, 1 upon success */
int encode_resource_records(struct ResourceRecord* rr, uint8_t** buffer)
{
  int i;
  while (rr)
  {
    // Answer questions by attaching resource sections.
    encode_domain_name(buffer, rr->name);
    put16bits(buffer, rr->type);
    put16bits(buffer, rr->Rclass);
    put32bits(buffer, rr->ttl);
    put16bits(buffer, rr->rd_length);

    switch (rr->type)
    {
      case A_Resource_RecordType:
        for(i = 0; i < 4; ++i)
          put8bits(buffer, rr->rd_data.a_record.addr[i]);
        break;
      case AAAA_Resource_RecordType:
        for(i = 0; i < 16; ++i)
          put8bits(buffer, rr->rd_data.aaaa_record.addr[i]);
        break;
      default:
        fprintf(stderr, "Unknown type %u. => Ignore resource record.\n", rr->type);
      return 1;
    }

    rr = rr->next;
  }

  return 0;
}

/* @return 0 upon failure, 1 upon success */
int encode_msg(struct Message* msg, uint8_t** buffer)
{
  struct Question* q;
  int rc;

  encode_header(msg, buffer);

  q = msg->questions;
  while (q)
  {
    encode_domain_name(buffer, q->qName);
    put16bits(buffer, q->qType);
    put16bits(buffer, q->qClass);

    q = q->next;
  }

  rc = 0;
  rc |= encode_resource_records(msg->answers, buffer);
  rc |= encode_resource_records(msg->authorities, buffer);
  rc |= encode_resource_records(msg->additionals, buffer);

  return rc;
}

void free_resource_records(struct ResourceRecord* rr)
{
  struct ResourceRecord* next;

  while (rr) {
    free(rr->name);
    next = rr->next;
    free(rr);
    rr = next;
  }
}

void free_questions(struct Question* qq)
{
  struct Question* next;

  while (qq) {
    free(qq->qName);
    next = qq->next;
    free(qq);
    qq = next;
  }
}





// /*
// * Debugging functions.
// */

void print_hex(uint8_t* buf, size_t len)
{
  unsigned int i;
  printf("%zu bytes:\n", len);
  for(i = 0; i < len; ++i)
    printf("%02x ", buf[i]);
  printf("\n");
}

void print_resource_record(struct ResourceRecord* rr)
{
  int i;
  while (rr)
  {
    printf("  ResourceRecord { name '%s', type %u, Rclass %u, ttl %u, rd_length %u, ",
        rr->name,
        rr->type,
        rr->Rclass,
        rr->ttl,
        rr->rd_length
    );

    union ResourceData *rd = &rr->rd_data;
    switch (rr->type)
    {
      case A_Resource_RecordType:
        printf("Address Resource Record { address ");

        for(i = 0; i < 4; ++i)
          printf("%s%u", (i ? "." : ""), rd->a_record.addr[i]);

        printf(" }");
        break;
      case NS_Resource_RecordType:
        printf("Name Server Resource Record { name %s }",
          rd->name_server_record.name
        );
        break;
      case CNAME_Resource_RecordType:
        printf("Canonical Name Resource Record { name %u }",
          rd->cname_record.name
        );
        break;
      case SOA_Resource_RecordType:
        printf("SOA { MName '%s', RName '%s', serial %u, refresh %u, retry %u, expire %u, minimum %u }",
          rd->soa_record.MName,
          rd->soa_record.RName,
          rd->soa_record.serial,
          rd->soa_record.refresh,
          rd->soa_record.retry,
          rd->soa_record.expire,
          rd->soa_record.minimum
        );
        break;
      case PTR_Resource_RecordType:
        printf("Pointer Resource Record { name '%s' }",
          rd->ptr_record.name
        );
        break;
      case MX_Resource_RecordType:
        printf("Mail Exchange Record { preference %u, exchange '%s' }",
          rd->mx_record.preference,
          rd->mx_record.exchange
        );
        break;
      case TXT_Resource_RecordType:
        printf("Text Resource Record { txt_data '%s' }",
          rd->txt_record.txt_data
        );
        break;
      case AAAA_Resource_RecordType:
        printf("AAAA Resource Record { address ");

        for(i = 0; i < 16; ++i)
          printf("%s%02x", (i ? ":" : ""), rd->aaaa_record.addr[i]);

        printf(" }");
        break;
      default:
        printf("Unknown Resource Record { ??? }");
    }
    printf("}\n");
    rr = rr->next;
  }
}

void print_query(struct Message* msg)
{
  printf("QUERY { ID: %02x", msg->id);
  printf(". FIELDS: [ QR: %u, OpCode: %u ]", msg->qr, msg->opcode);
  printf(", QDcount: %u", msg->qdCount);
  printf(", ANcount: %u", msg->anCount);
  printf(", NScount: %u", msg->nsCount);
  printf(", ARcount: %u,\n", msg->arCount);

  struct Question* q = msg->questions;
  while (q)
  {
    printf("  Question { qName '%s', qType %u, qClass %u }\n",
      q->qName,
      q->qType,
      q->qClass
    );
    q = q->next;
  }

  print_resource_record(msg->answers);
  print_resource_record(msg->authorities);
  print_resource_record(msg->additionals);

  printf("}\n");
}
