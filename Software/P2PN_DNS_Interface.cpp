#include <P2PN_DNS_Interface.hpp>


std::pair<std::string, std::string> splitPort(const std::string& s)
 {
    if (s.empty())
        return {};
    if (s[0] == '[') {
        std::size_t closure = s.find_first_of(']');
        std::size_t found = s.find_last_of(':');
        if (closure == std::string::npos)
            return {s, ""};
        if (found == std::string::npos or found < closure)
            return {s.substr(1,closure-1), ""};
        return {s.substr(1,closure-1), s.substr(found+1)};
    }
    std::size_t found = s.find_last_of(':');
    std::size_t first = s.find_first_of(':');
    if (found == std::string::npos or found != first)
        return {s, ""};
    return {s.substr(0,found), s.substr(found+1)};
}

std::string readLine(const char* prefix)
{
#ifndef WIN32_NATIVE
    const char* line_read = readline(prefix);
    if (line_read && *line_read)
        add_history(line_read);

#else
    char line_read[512];
    std::cout << PROMPT;
    fgets(line_read, 512 , stdin);
#endif
    return line_read ? std::string(line_read) : std::string("\0", 1);
}

dht_params parseArgs(int argc, char **argv) 
{
    dht_params params;
    int opt;
    while ((opt = getopt_long(argc, argv, "hidsvp:n:b:l:", long_options, nullptr)) != -1) 
    {
        switch (opt) {
        case 'p': {
                int port_arg = atoi(optarg);
                if (port_arg >= 0 && port_arg < 0x10000)
                    params.port = port_arg;
                else
                    std::cout << "Invalid port: " << port_arg << std::endl;
            }
            break;
        case 'S': {
                int port_arg = atoi(optarg);
                if (port_arg >= 0 && port_arg < 0x10000)
                    params.proxyserver = port_arg;
                else
                    std::cout << "Invalid port: " << port_arg << std::endl;
            }
            break;
        case 'n':
            //params.network = strtoul(optarg, nullptr, 0);
            break;
        case 'b':
            params.bootstrap = splitPort((optarg[0] == '=') ? optarg+1 : optarg);
            if (not params.bootstrap.first.empty() and params.bootstrap.second.empty()) {
                params.bootstrap.second = std::to_string(DHT_DEFAULT_PORT);
            }
            break;
        case 'h':
            params.help = true;
            break;
        case 'l':
            params.logfile = optarg;
            break;
        case 'L':
            params.log = true;
            params.syslog = true;
            break;
        case 'v':
            params.log = true;
            break;
        case 'i':
            params.generate_identity = true;
            break;
        case 'd':
            params.daemonize = true;
            break;
        case 's':
            params.service = true;
            break;
        default:
            break;
        }
    }
    return params;
}
