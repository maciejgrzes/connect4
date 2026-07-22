#include "inc/server.hpp"

int main() {
    Server server;

    if (!server.Start(7777)) {
        return 1;
    }

    server.Run();
    return 0;
}
