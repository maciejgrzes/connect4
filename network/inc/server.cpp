#include "server.hpp"
#include "packets.hpp"
#include <cstring>
#include <enet/enet.h>
#include <iostream>
#include <ostream>

bool Server::Start(int port, int maxClients) {
    if (enet_initialize () != 0) {
        std::cerr << "An error occurred while initializing ENet." << std::endl;
        return false;
    }

    ENetAddress address = {0};

    address.host = ENET_HOST_ANY;
    address.port = port;

    host = enet_host_create(&address, maxClients, 2, 0, 0);

    if (host == nullptr) {
        std::cerr << "An error occurred while trying to create an ENet server host." << std::endl;
        return false;
    }

    std::cout << "Started a server..." << std::endl;
    return true;
}


void Server::HandleConnect(ENetEvent& event) {
    Player pl;
    pl.connected = true;
    pl.peer = event.peer;
    event.peer->data = &pl;

    if (players.empty()) {
        pl.id = 1;
        players.push_back(pl);

        std::cout << "Player 1 connected" << std::endl;
        return;
    } else if (players.size() == 1) {
        pl.id = 2;
        players.push_back(pl);

        std::cout << "Player 2 connected" << std::endl;
        return;
    } else if (players.size() >= 2) {
        std::cerr << "No more space for new players" << std::endl;
        return;
    }
    return;
}

void Server::HandleDisconnect(ENetEvent& event) {
    Player* player = static_cast<Player*>(event.peer->data);

    if (player)
    {
        std::cout << "Player " << player->id << " disconnected" << std::endl;

        player->peer = nullptr;
        player->id = -1;
    }
}

void Server::SendBoard() {
    BoardPacket data{
        PacketType::BOARD_UPDATE
    };

    std::memcpy(data.board, game.board, sizeof(game.board));

    ENetPacket* packet = enet_packet_create(
        &data,
        sizeof(data),
        ENET_PACKET_FLAG_RELIABLE
    );

    enet_host_broadcast(host, 0, packet);
}

void Server::BroadcastGameOver() {

}

void Server::HandlePacket(ENetEvent& event) {
    MovePacket* packet = (MovePacket*)event.packet->data;

    game.MakeMove(packet->player, packet->column);
    
    SendBoard();

    if (game.CheckWin(packet->player))
        BroadcastGameOver();
}

void Server::Run() {
    while (true)
    {
        ENetEvent event;

        while (enet_host_service(host, &event, 1000) > 0)
        {
            switch(event.type)
            {
                case ENET_EVENT_TYPE_CONNECT:
                    HandleConnect(event);
                    break;

                case ENET_EVENT_TYPE_RECEIVE:
                    HandlePacket(event);
                    break;

                case ENET_EVENT_TYPE_DISCONNECT:
                    HandleDisconnect(event);
                    break;

                case ENET_EVENT_TYPE_NONE:
                    break;
            }
        }
    }
}
