#John Grun 11/24/2017
ALL: P2PN-DNS

TEST_P2PN-DNS:
	g++  Test_P2PN_DNS_Interface.cpp P2PN_DNS_Interface.cpp P2PN-DNS.cpp -std=c++11 -I./ -lopendht -lgnutls -lreadline -o TEST_P2PN-DNS.exe

TEST_DNS_COMMS:
	g++ -g Test_DNSProtocol.cpp DNS_COMMS.cpp DNSProtocol.cpp -std=c++14 -I./ -o Test_DNSProtocol.exe	

P2PN-DNS:
	g++ -g P2PN-DNS-MAIN.cpp P2PN_DNS_Interface.cpp P2PN-DNS.cpp DNS_COMMS.cpp DNSProtocol.cpp -std=c++14 -I./ -lopendht -lgnutls -lreadline -o P2PN-DNS.exe