#pragma once

#include <enet/enet.h>

#include "../../src/inc/structs.hpp"

class Client {
public:
    bool Connect(const char* ip, int port, int timeoutMs = 5000);
    
    void Disconnect();

    void Poll();

    void SendMove(int column);

    void SendReady();


    bool connected = false;
    
    int myPlayerId = 0;
    
    Game game;

    int winner = 0;

private:
    ENetHost* host = nullptr;

    ENetPeer* peer = nullptr;
};
