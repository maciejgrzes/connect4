#include "inc/enet.h"

#include "inc/servertypes.h"

#include <iostream>

using namespace std;

int main() {
    if (enet_initialize() != 0) {
        cout << "Failed to initialize ENet" << endl;
        return 1;
    }

    ENetAddress addr = {0};
    const int MAX_CLIENTS = 2;

    addr.host = ENET_HOST_ANY;
    addr.port = 7777;

    ENetHost* server = enet_host_create(&addr, MAX_CLIENTS, 2, 0, 0);

    if (!server) {
        cout << "Failed to create server!" << endl;
        return 1;
    }

    cout << "Started the server..." << endl;

    while (true) {

        ENetEvent event;
    
        while (enet_host_service(server, &event, 1000) > 0) {
            switch(event.type) {
                case ENET_EVENT_TYPE_CONNECT:
                    printf("A new client connected from %x:%u.\n",  event.peer->address.host, event.peer->address.port);
                    // maybe add a player into a vector or something or just innit it as p1 and p2 since there will be only 2 players
                    break;

                case ENET_EVENT_TYPE_RECEIVE:
                    printf("A packet of length %lu containing %s was received from %s on channel %u.\n",
                            event.packet->dataLength,
                            event.packet->data,
                            event.peer->data,
                            event.channelID);
                    // do sum with da packet
                    enet_packet_destroy(event.packet);
                    break;

                case ENET_EVENT_TYPE_DISCONNECT:
                    printf("%s disconnected.\n", event.peer->data);
                    // some how remove the player from players vector and await for a new player to join
                    // maybe even give an option to restart the game or wait for the player to return
                    event.peer->data = nullptr;
                    break;

                case ENET_EVENT_TYPE_DISCONNECT_TIMEOUT:
                    printf("%s disconnected due to timeout.\n", event.peer->data);
                    event.peer->data = nullptr;
                    // do da same as disconnect
                    break;

                case ENET_EVENT_TYPE_NONE:
                    break;
            }
        }
    
    }



    enet_host_destroy(server);
    enet_deinitialize();
    return 0;
}
