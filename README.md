# P2PN-DNS
Peer to Peer Network Domain Name System
### Required 
make 
syslog
git 
cmake
g++
gcc

### Complile 

	make INSTALL_DEPENDS

	make all 

To build test programs 

	make TEST

NOTES:
Compiled on Ubuntu 16.04 LTS
Compiled against:
OpenDHT git commit f9d48cd15a72c34455158a970f84537b867de38a Thu Nov 30 00:17:29 2017 +0100

SimpleDNS git commit 7c60d66d84505185861e988216713dcc95b5fa3a Mon Nov 6 14:26:27 2017 +0100

### Run

Node 1 
-D is DNS port 

-p is DHT port

	./P2PN-DNS.exe -D USEDPORT_FOR_DNS -P ANOTHERUNUSEDPORT_FOR_DHT

example:

	./P2PN-DNS.exe -D 10076 -p 50666

Startup message will print: 

OpenDNSdht node 15665d9ce9341267bfe9cb395102a33d51cd609f running on port 50666

NOTE: port number.

Node 2 to N

In another terminal 

if on same computer

	./P2PN-DNS.exe -D UNUSEDPORT -P ANOTHERUNUSEDPORT -b IPADDRESS:PORT

example:

	./P2PN-DNS.exe -D 10075 -p 50667 -b 127.0.0.1:50666

else if on another computer change 127.0.0.1 to the ip address of the computer. 

	./P2PN-DNS.exe -b IPADDRESS:PORT

Send a DNS update mesage 

	./DNS-UPDATE.exe 

	Usage : S DomainName IPAddress DNSserverNodeIP DnsServerPort
example:

	S google.com 192.178.78.5 127.0.0.1 10076

DnsServerPort must match the port specified in -D for one of the reachable nodes

Send a DNS query for a domain name. Use dig

	dig @Dnsserver_IP -p DNSport(-D option from P2PN-DNS) Domainname 

example:  

	dig @127.0.0.1 -p 10075 google.com

### DEMO 
Terminal 1

	./P2PN-DNS.exe -D 10077 -p 50666 &
Terminal 2

	./P2PN-DNS.exe -D 10078 -p 50667 -b 127.0.0.1:50666 &

Terminal 3

	./DNS-UDATE.exe 
		S google.com 8.8.8.8 127.0.0.1 10077

Terminal 4

	dig @127.0.0.1 -p 10078 google.com

###Documentation

Documentation including report and slides in the project root under Documentation

[https://github.com/P2PN-DNS/P2PN-DNS](https://github.com/P2PN-DNS/P2PN-DNS)

### References - Sources
* ["Domain Name System”. En.wikipedia.org. N.p., 2017. Web. 2 November 2017.](https://en.wikipedia.org/wiki/Domain_Name_System)
* ["Distributed Denial of Service Attacks on Root Nameservers ”. En.wikipedia.org. N.p., 2017. Web. 2 November 2017](https://en.wikipedia.org/wiki/
* ["I Fought My ISPS Bad Behavior and Won”. Eric Helgeson. Web. 2 November 2017](https://erichelgeson.github.io/blog/2013/12/31/i-fought-my-isps-bad-behavior-and-won/)
* [Eckersley, Technical Analysis by Peter. “Widespread Hijacking of Search Traffic in the United States.” Electronic Frontier Foundation, 14 Oct. 2011. Web. 2 November 2017](https://www.eff.org/
* [Transaction Log”. En.wikipedia.org. N.p., 2017. Web. 2 November 2017.](https://en.wikipedia.org/wiki/Transaction_log)
* [Merkle Tree. En.wikipedia.org. N.p., 2017. Web. 2 November 2017.](https://en.wikipedia.org/wiki/Merkle_tree)
* [Kangasharju, Jussi. Chapter 4: Distributed Systems: Replication and Consistency. N.p., 2017. Web. 7 November 2017.](https://www.cs.helsinki.fi/webfm_send/1256)
* [Blockchain. En.wikipedia.org. N.p., 2017. Web. 7 November 2017.](https://en.wikipedia.org/wiki/Blockchain)
* [Jacquin, Ludovic, et al. “The Trust Problem in Modern Network Infrastructures.” SpringerLink, Springer, Cham, 28 Apr. 2015. N.p., 2017. Web. 7 November 2017. ](https://link.springer.com/
* [“ACID”. En.wikipedia.org. N.p., 2017. Web. 8 November 2017.](https://en.wikipedia.org/wiki/Acid)
*[DataStax Academy Follow. “A Deep Dive Into Understanding Apache Cassandra.”LinkedIn SlideShare, 25 Sept. 2013. N.p., 2017. Web. 2 December 2017.](https://www.slideshare.net/planetcassandra/a-deep-dive-into-understanding-apache-cassandra)
* [“Peer-to-Peer (P2P) Systems.” SlidePlayer. N.p., 2017. Web. 2 December 2017](http://slideplayer.com/slide/4168557/)
* [“Non-Transitive Connectivity and DHTs “](https://www.usenix.org/legacy/events/worlds05/tech/full_papers/freedman/freedman_html/index.html)
* [“Amazon Route 53”. En.wikipedia.org. N.p., 2017. Web. 6 December 2017.](https://en.wikipedia.org/wiki/Amazon_Route_53)
* [Distributed hash table En.wikipedia.org. N.p., 2017. Web. 11 November 2017.](https://en.wikipedia.org/wiki/Distributed_hash_table);
* [FIFO (computing and electronics)” .En.wikipedia.org. N.p., 2017. Web. 6 December 2017.](https://en.wikipedia.org/wiki/FIFO_(computing_and_electronics)    
* [“RFC 1034  DOMAIN NAMES - CONCEPTS AND FACILITIES”, N.p., 2017. Web. 2 November 2017.](https://www.ietf.org/rfc/rfc1034.txt)
* [“RFC 1035 DOMAIN NAMES - IMPLEMENTATION AND SPECIFICATION” , N.p., 2017. Web. 2 November 2017.](https://www.ietf.org/rfc/rfc1035.txt)
* [“Embedded DNS server in user-defined networks” N.p., 2017. Web. 2 November 2017.](https://docs.docker.com/engine/userguide/networking/configure-dns/)
* [“OpenDHT”   Web. 2 November 2017.](https://github.com/savoirfairelinux/opendht)
* [“SimpleDNS: Web. 2 November 2017.](https://github.com/mwarning/SimpleDNS)
* [“Dynamic Updates in the Domain Name System (DNS UPDATE)” Web. 2 November 2017.](https://tools.ietf.org/html/rfc2136)
* [“ You can’t Peer to Peer the DNS”](https://nohats.ca/wordpress/blog/2012/04/09/you-cant-p2p-the-dns-and-have-it-too/)
* [DSN Primer Notes](http://www.cs.duke.edu/courses/fall16/compsci356/DNS/DNS-primer.pdf)

### References - Other software components and tutorials used in this application
* [C++ Standard lib]()
* [OpenDht](https://github.com/savoirfairelinux/opendht)
* [SimpleDNS](https://github.com/mwarning/SimpleDNS)

### References - Build enviroment 
* [Cmake tutorial](http://derekmolloy.ie/hello-world-introductions-to-cmake/)