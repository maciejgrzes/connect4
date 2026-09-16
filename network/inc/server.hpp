#pragma once

#include <enet/enet.h>
#include <list>
#include <cstdint>

#include "../../src/inc/game.hpp"
#include "player.hpp"

class Server {
public:

    bool Start(int port, int maxClients);

    void Run();

    bool verbose = false;
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
