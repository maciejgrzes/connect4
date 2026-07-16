#include "../inc/enet.h"

#include "../inc/nttypes.hpp"

#include <iostream>
#include <string>

using namespace std;

int main() {
    if (enet_initialize() != 0) {
        cout << "Failed to initialize ENet" << endl;
        return 1;
    }

    ENetHost* client = {};
    client = enet_host_create(nullptr, 1, 2, 0, 0);

    if (!client) {
        cout << "Failed to create client!" << endl;
        return 1;
    }

    ENetAddress addr = {};
    ENetEvent event = {};
    ENetPeer* peer = {};

    string ipv4 = "123.0.0.1";

    enet_address_set_host(&addr, ipv4.c_str());
    addr.port = 7777;

    peer = enet_host_connect(client, &addr, 2, 0);
    
    if (!peer) {
        cout << "No available peers!" << endl;
        return 1;
    }

    if (enet_host_service(client, &event, 5000) > 0 &&
        event.type == ENET_EVENT_TYPE_CONNECT) {
        cout << "Connection to  succeeded." << endl;
    } else {
    /* Either the 5 seconds are up or a disconnect event was */
    /* received. Reset the peer in the event the 5 seconds   */
    /* had run out without any significant event.            */
        enet_peer_reset(peer);
        puts("Connection to some.server.net:1234 failed.");
    }

    // TODO: finish this shite

    enet_host_destroy(client);
    enet_deinitialize();
    return 0;
}
