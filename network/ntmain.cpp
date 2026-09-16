#include "inc/server.hpp"
#include <iostream>
#include <string>

int main(int argc, char *argv[]) {
    Server server;
    if (argc > 2) { std::cerr << "Only one argument supported (-v or --verbose)" << std::endl;}

    std::string arg = argv[1];
    if (arg.compare("-v") || arg.compare("--verbose")) {
        server.verbose = true;
    }

    if (!server.Start(7777, 2)) {
        return 1;
    }

    server.Run();
    return 0;
}
