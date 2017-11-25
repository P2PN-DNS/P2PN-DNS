
//include this file only once!
#pragma once

#include <DNSProtocol.hpp>
#include <exception>
#include <vector>
#include <string> 


int CreateUDPSocketHandle(int port);
int Run(int port, bool& shutdown, std::function<bool(const std::string&, const std::vector<uint8_t>& )> UpdateRecordCallback, std::function<const std::vector<uint8_t>(const std::string& )> GetRecordCallback);
