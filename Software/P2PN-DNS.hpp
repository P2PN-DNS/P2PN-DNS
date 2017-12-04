
//include this file only once!
#pragma once

#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <unistd.h>
#include <opendht.h>
#include <vector>
#include <functional>
#include <thread>
#include <condition_variable>
//#include <opendht/log.h>
//#include <opendht/log_enable.h>

//include/opendht/log.h
//    include/opendht/log_enable.h

void print_usage();
void print_node_info(const  dht::DhtRunner& DNSdht);
void print_routing_table(const dht::DhtRunner& DNSdht);
void print_IpAddress(std::vector<uint8_t> Addr);
bool PutDomainName(dht::DhtRunner& DNSdht, const std::string& DomainName, const std::vector<uint8_t>& IPAddress);
const std::vector<uint8_t> GetDomainName(dht::DhtRunner& DNSdht, const std::string& DomainName);


