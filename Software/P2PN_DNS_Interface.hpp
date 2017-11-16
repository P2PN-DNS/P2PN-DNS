#include <iostream>
#include <string>
#include <getopt.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <opendht.h>

/// used for argc and argV
struct dht_params {
    bool help {false}; // print help and exit
    bool log {false};
    std::string logfile {};
    bool syslog {false};
    in_port_t port {0};
    //long network {0};
    bool generate_identity {false};
    bool daemonize {false};
    bool service {false};
    std::pair<std::string, std::string> bootstrap {};
    in_port_t proxyserver {0};
};

static const constexpr in_port_t DHT_DEFAULT_PORT = 4222;

static const constexpr char* PROMPT = ">> ";

static const constexpr struct option long_options[] = {
   {"help",       no_argument      , nullptr, 'h'},
   {"port",       required_argument, nullptr, 'p'},
   {"net",        required_argument, nullptr, 'n'},
   {"bootstrap",  required_argument, nullptr, 'b'},
   {"identity",   no_argument      , nullptr, 'i'},
   {"verbose",    no_argument      , nullptr, 'v'},
   {"daemonize",  no_argument      , nullptr, 'd'},
   {"service",    no_argument      , nullptr, 's'},
   {"logfile",    required_argument, nullptr, 'l'},
   {"syslog",     no_argument      , nullptr, 'L'},
   {"proxyserver",required_argument, nullptr, 'S'},
   {nullptr,      0                , nullptr,  0}
};

std::string readLine(const char* prefix = PROMPT);
std::pair<std::string, std::string> splitPort(const std::string& s);
dht_params parseArgs(int argc, char **argv);