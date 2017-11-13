#Sequence Diagrams generated at http://sequencediagram.org/

#Sequence Diagram code for P2PN-DNS to DNS Client Interaction 

title P2PN-DNS to DNS Client Interaction 

Client -> P2PN-DNS Node:Request address record for domain name\ne.g. www.example.com
P2PN-DNS Node ->P2PN-DNS Node: Get address from key-value store.
P2PN-DNS Node --> Client: Return address for domain name.\n e.g. 186.123.16.2

#Sequence Diagram for P2PN-DNS Node to P2PN-DNS Node Locating Peers


#Sequence Diagram for P2PN-DNS Node to P2PN-DNS Node Update Interaction

title P2PN-DNS Node to P2PN-DNS Node Update Interaction
note right of P2PN-DNS Node 1:Node has been running and contains key value store entries
note right of P2PN-DNS Node 2:Node has been running and contains key value store entries
par Process incoming messages
P2PN-DNS Node 2 ->P2PN-DNS Node 2:Server thread listening for incoming messages
P2PN-DNS Node 1 -> P2PN-DNS Node 2: DNS update request.\n Change Domain Name address to Y.\n Contains domain name, new address, timestamp, hash of previous record.
P2PN-DNS Node 2 -> P2PN-DNS Node 2: Write DNS update request to end of internal commit log.
end
par Thread consuming 
P2PN-DNS Node 2 -> P2PN-DNS Node 2: Read the entry from the beggining of the commit log\n
P2PN-DNS Node 2 -> P2PN-DNS Node 2: Compare the hash of previous records in the \ncommit log to the hash entry in the key-value store.\n
alt Hashes match
P2PN-DNS Node 2 -> P2PN-DNS Node 2: If the hashes match, the update record \n has come in the correct order. 
P2PN-DNS Node 2 -> P2PN-DNS Node 2: Calculate the new hash from the timestamp, \n previous hash, and domain name.
P2PN-DNS Node 2 -> P2PN-DNS Node 2: Update key value store with new domain name
P2PN-DNS Node 2 -> P2PN-DNS Node 2: Remove the update record from the \n front of the commit log
else Hashes do not match
P2PN-DNS Node 2 -> P2PN-DNS Node 2: If the hashes do not match, the update record \n has NOT come in the correct order. 
P2PN-DNS Node 2 -> P2PN-DNS Node 2: Do NOT Update key value store with new domain name
P2PN-DNS Node 2 -> P2PN-DNS Node 2: Remove the update record from the \n front of the commit log. Scheme to handle error will need to be implmented
end
end
