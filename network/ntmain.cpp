#include "inc/server.hpp"

int main() {
    Server server;

    if (!server.Start(7777, 2)) {
        return 1;
    }

    server.Run();
    return 0;
}
