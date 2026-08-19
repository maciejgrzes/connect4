#include "server.hpp"
#include "packets.hpp"
#include <cstdint>
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
    if (players.size() >= 2) {
        std::cerr << "No more space for new players" << std::endl;
        return;
    }
    Player pl;
    pl.connected = true;
    pl.peer = event.peer;
    pl.id = players.empty() ? 1 : 2;

    players.push_back(pl);
    event.peer->data = &players.back();
    std::cout << "Player " << pl.id << " connected" << std::endl;

    PlayerJoinedPacket data {PLAYER_JOINED, (uint8_t)pl.id};
    ENetPacket* packet = enet_packet_create(&data, sizeof(data), ENET_PACKET_FLAG_RELIABLE);
    enet_peer_send(event.peer, 0, packet);
}

void Server::HandleDisconnect(ENetEvent& event) {
    Player* player = static_cast<Player*>(event.peer->data);
    if (player) {
        std::cout << "Player " << player->id << " disconnected" << std::endl;
        players.remove_if([player](const Player& p) { return &p == player; });
    }
    event.peer->data = nullptr;
}

void Server::SendBoard() {
    BoardPacket data {
        PacketType::BOARD_UPDATE
    };

    std::memcpy(data.board, game.board, sizeof(game.board));

    data.turn = game.turn;

    ENetPacket* packet = enet_packet_create(
        &data,
        sizeof(data),
        ENET_PACKET_FLAG_RELIABLE
    );

    enet_host_broadcast(host, 0, packet);
}

void Server::BroadcastGameOver(uint8_t winner) {
    GameOverPacket data{
        GAME_OVER,
        winner
    };
    ENetPacket* packet = enet_packet_create(
        &data,
        sizeof(data),
        ENET_PACKET_FLAG_RELIABLE
    );
    enet_host_broadcast(host, 0, packet);
}

void Server::HandlePacket(ENetEvent& event) {
    if (event.packet->dataLength < sizeof(PacketType)) {
        enet_packet_destroy(event.packet);
        return;
    }
    PacketType type = *reinterpret_cast<PacketType*>(event.packet->data);
    Player* sender = static_cast<Player*>(event.peer->data);

    if (type == MOVE && event.packet->dataLength >= sizeof(MovePacket)) {
        if (sender && !game.game_over) {
            MovePacket* packet = (MovePacket*)event.packet->data;
            if (game.MakeMove(sender->id, packet->column)) {
                SendBoard();
                if (game.CheckWin(sender->id)) {
                    game.game_over = true;
                    BroadcastGameOver(sender->id);
                }
            }
        }
    }
    else if (type == READY && event.packet->dataLength >= sizeof(ReadyPacket)) {
        if (sender && game.game_over) {
            sender->ready = true;
            std::cout << "Player " << sender->id << " is ready" << std::endl;

            bool allReady = players.size() == 2;
            for (auto& p : players)
                if (!p.ready) allReady = false;

            if (allReady) {
                game.EndGame();
                for (auto& p : players) p.ready = false;
                SendBoard();
            }
        }
    }

    enet_packet_destroy(event.packet);
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
