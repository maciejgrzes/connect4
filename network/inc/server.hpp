#pragma once

#include <enet/enet.h>

#include "../../inc/structs.hpp"
#include "player.hpp"

class Server {
public:

    bool Start(int port);

    void Run();

private:

    void HandleConnect(ENetEvent&);

    void HandleDisconnect(ENetEvent&);

    void HandlePacket(ENetEvent&);

    void SendBoard();

    void BroadcastGameOver();


    ENetHost* host = nullptr;

    Player players[2];

    Game game;
};
