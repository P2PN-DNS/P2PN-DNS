
//include this file only once!
#pragma once

#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <unistd.h>
#include <opendht.h>
//#include <getopt.h>
//#include <tuple>
//#include <readline/readline.h>
//#include <readline/history.h>


//#include <vector>

void print_usage();
void print_node_info(const  dht::DhtRunner& DNSdht);
void print_routing_table(const dht::DhtRunner& DNSdht);
bool PutDomainName(dht::DhtRunner& DNSdht, const std::string& DomainName, const std::string& IPAddress);
const std::string GetDomainName(dht::DhtRunner& DNSdht, const std::string& DomainName);


