#John Grun 11/24/2017
ALL: P2PN-DNS DNS-UPDATE

TEST: TEST_P2PN-DNS TEST_DNSPROTOCOL

PURGE:
	rm -fR opendht
	rm -fR msgpack-2.1.1

INSTALL_DEPENDS:
	sh ./Install_depends.sh

CLEAN:
	rm *.exe
	rm *.o

# Test program to only test the DHT key value store 
TEST_P2PN-DNS:
	g++  Test_P2PN_DNS_Interface.cpp P2PN_DNS_Interface.cpp P2PN-DNS.cpp -std=c++11 -I./ -lopendht -lgnutls -lreadline -o TEST_P2PN-DNS.exe
# Test program to only test the DNS communications lib 
TEST_DNSPROTOCOL:
	g++ -g Test_DNSProtocol.cpp DNS_COMMS.cpp DNSProtocol.cpp -std=c++14 -I./ -lreadline -lgnutls -o Test_DNSProtocol.exe	
# Peer to peer Network Domain Name System
P2PN-DNS:
	g++ -g P2PN-DNS-MAIN.cpp P2PN_DNS_Interface.cpp P2PN-DNS.cpp DNS_COMMS.cpp DNSProtocol.cpp -std=c++14 -I./ -lopendht -lgnutls -lreadline -o P2PN-DNS.exe
#Tool created to send unoffical DNS update packets to P2PN-DNS
DNS-UPDATE:
	g++ -g DNS-UPDATE.cpp DNS_COMMS.cpp DNSProtocol.cpp -std=c++14 -I./ -lreadline -lgnutls -o DNS-UPDATE.exe
