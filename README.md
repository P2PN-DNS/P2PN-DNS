# P2PN-DNS
Peer to Peer Network Domain Name System


### Complile 
sh ./Install_depends.sh 

sh ./compile.sh

### Run

Node 1 
./P2PN-DNS.exe 

Startup message will print: 
OpenDNSdht node 15665d9ce9341267bfe9cb395102a33d51cd609f running on port 45725

NOTE: port number.

Node 2 to N
In another terminal 

if on same computer

./P2PN-DNS.exe -b IPADDRESS:PORT
e.g
./P2PN-DNS.exe -b 0.0.0.0:45725

else if on another computer cahne 0.0.0.0 to the ip address of the computer. 

./P2PN-DNS.exe -b IPADDRESS:PORT

### References - Sources
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

### References - Other software components and tutorials used in this application
* [C++ Standard lib]()
* [OpenDht](https://github.com/savoirfairelinux/opendht)
* [LevelDB Key Value Store](https://github.com/google/leveldb)

### References - Build enviroment 
* [Cmake tutorial](http://derekmolloy.ie/hello-world-introductions-to-cmake/)