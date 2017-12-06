# P2PN-DNS
Peer to Peer Network Domain Name System
### Required 
make 

syslog 

### Complile 

	make INSTALL_DEPENDS

	make all 

To build test programs 

	make TEST

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

### References - Sources
* [DNS update](https://tools.ietf.org/html/rfc2136)
* [DSN Primer Notes](http://www.cs.duke.edu/courses/fall16/compsci356/DNS/DNS-primer.pdf)
* ["Domain Name System”. En.wikipedia.org. N.p., 2017. Web. 2 November 2017.](https://en.wikipedia.org/wiki/Domain_Name_System)
* ["Distributed Denial of Service Attacks on Root Nameservers ”. En.wikipedia.org. N.p., 2017. Web. 2 November 2017](https://en.wikipedia.org/wiki/Distributed_denial-of-service_attacks_on_root_nameservers)
* ["I Fought My ISPS Bad Behavior and Won”. Eric Helgeson. Web. 2 November 2017](https://erichelgeson.github.io/blog/2013/12/31/i-fought-my-isps-bad-behavior-and-won/)
* [Eckersley, Technical Analysis by Peter. “Widespread Hijacking of Search Traffic in the United States.” Electronic Frontier Foundation, 14 Oct. 2011. Web. 2 November 2017](https://www.eff.org/deeplinks/2011/07/widespread-search-hijacking-in-the-us)
* [Transaction Log”. En.wikipedia.org. N.p., 2017. Web. 2 November 2017.](https://en.wikipedia.org/wiki/Transaction_log)
* [Merkle Tree. En.wikipedia.org. N.p., 2017. Web. 2 November 2017.](https://en.wikipedia.org/wiki/Merkle_tree)
* [Kangasharju, Jussi. Chapter 4: Distributed Systems: Replication and Consistency. N.p., 2017. Web. 7 November 2017.](https://www.cs.helsinki.fi/webfm_send/1256)
* [Blockchain. En.wikipedia.org. N.p., 2017. Web. 7 November 2017.](https://en.wikipedia.org/wiki/Blockchain)
* [Jacquin, Ludovic, et al. “The Trust Problem in Modern Network Infrastructures.” SpringerLink, Springer, Cham, 28 Apr. 2015. N.p., 2017. Web. 7 November 2017. ](https://link.springer.com/chapter/10.1007/978-3-319-25360-2_10)
* [“ACID”. En.wikipedia.org. N.p., 2017. Web. 8 November 2017.](https://en.wikipedia.org/wiki/Acid)
* [Distributed hash table En.wikipedia.org. N.p., 2017. Web. 11 November 2017.](https://en.wikipedia.org/wiki/Distributed_hash_table);
* [Non-Transitive Connectivity and DHTs](https://www.usenix.org/legacy/events/worlds05/tech/full_papers/freedman/freedman_html/index.html);

### References - Other software components and tutorials used in this application
* [C++ Standard lib]()
* [OpenDht](https://github.com/savoirfairelinux/opendht)
* [SimpleDNS](https://github.com/mwarning/SimpleDNS)

### References - Build enviroment 
* [Cmake tutorial](http://derekmolloy.ie/hello-world-introductions-to-cmake/)
