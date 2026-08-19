#pragma once

#include <enet/enet.h>
#include <list>

#include "../../src/inc/structs.hpp"
#include "player.hpp"

class Server {
public:

    bool Start(int port, int maxClients);

    void Run();

private:

    void HandleConnect(ENetEvent& event);

    void HandleDisconnect(ENetEvent& event);

    void HandlePacket(ENetEvent& event);

    void SendBoard();

    void BroadcastGameOver(uint8_t winner);

    ENetHost* host = nullptr;

    std::list<Player> players;

    Game game;
};
