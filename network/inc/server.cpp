#include "server.hpp"
#include "packets.hpp"
#include <cstdint>
#include <cstring>
#include <enet/enet.h>
#include <iostream>
#include <ostream>

bool Server::Start(int port, int maxClients) {
    std::cout << "[Start] enet_initialize..." << std::endl;
    if (enet_initialize () != 0) {
        std::cerr << "[Start] An error occurred while initializing ENet." << std::endl;
        return false;
    }

    ENetAddress address{};
    address.host = ENET_HOST_ANY;
    address.port = port;

    std::cout << "[Start] enet_host_create on port " << port << " maxClients=" << maxClients << std::endl;
    host = enet_host_create(&address, maxClients, 2, 0, 0);

    if (host == nullptr) {
        std::cerr << "[Start] An error occurred while trying to create an ENet server host." << std::endl;
        return false;
    }

    std::cout << "[Start] Started a server..." << std::endl;
    return true;
}

void Server::HandleConnect(ENetEvent& event) {
    std::cout << "[HandleConnect] enter, current players.size()=" << players.size() << std::endl;

    if (players.size() >= 2) {
        std::cerr << "[HandleConnect] No more space for new players" << std::endl;
        return;
    }
    Player pl;
    pl.connected = true;
    pl.peer = event.peer;
    pl.id = players.empty() ? 1 : 2;

    std::cout << "[HandleConnect] assigning id=" << (int)pl.id << ", pushing to list..." << std::endl;
    players.push_back(pl);
    event.peer->data = &players.back();
    std::cout << "[HandleConnect] Player " << (int)pl.id << " connected, peer->data set to " << event.peer->data << std::endl;

    PlayerJoinedPacket data {PLAYER_JOINED, (uint8_t)pl.id};
    std::cout << "[HandleConnect] creating PLAYER_JOINED packet, sizeof=" << sizeof(data) << std::endl;
    ENetPacket* packet = enet_packet_create(&data, sizeof(data), ENET_PACKET_FLAG_RELIABLE);
    std::cout << "[HandleConnect] sending PLAYER_JOINED to peer..." << std::endl;
    enet_peer_send(event.peer, 0, packet);
    std::cout << "[HandleConnect] exit" << std::endl;
}

void Server::HandleDisconnect(ENetEvent& event) {
    std::cout << "[HandleDisconnect] enter, peer->data=" << event.peer->data << std::endl;
    Player* player = static_cast<Player*>(event.peer->data);
    if (player) {
        std::cout << "[HandleDisconnect] Player " << player->id << " disconnected, removing from list" << std::endl;
        players.remove_if([player](const Player& p) { return &p == player; });
    } else {
        std::cout << "[HandleDisconnect] peer->data was null, nothing to remove" << std::endl;
    }
    event.peer->data = nullptr;
    std::cout << "[HandleDisconnect] exit, players.size()=" << players.size() << std::endl;
}

void Server::SendBoard() {
    std::cout << "[SendBoard] enter, game.turn=" << game.turn << std::endl;
    BoardPacket data{};
    data.type = PacketType::BOARD_UPDATE;

    std::cout << "[SendBoard] memcpy board, sizeof(game.board)=" << sizeof(game.board)
               << " sizeof(data.board)=" << sizeof(data.board) << std::endl;
    std::memcpy(data.board, game.board, sizeof(game.board));

    data.turn = game.turn;

    std::cout << "[SendBoard] creating packet, sizeof(data)=" << sizeof(data) << std::endl;
    ENetPacket* packet = enet_packet_create(
        &data,
        sizeof(data),
        ENET_PACKET_FLAG_RELIABLE
    );

    std::cout << "[SendBoard] broadcasting..." << std::endl;
    enet_host_broadcast(host, 0, packet);
    std::cout << "[SendBoard] exit" << std::endl;
}

void Server::BroadcastGameOver(uint8_t winner) {
    std::cout << "[BroadcastGameOver] enter, winner=" << (int)winner << std::endl;
    GameOverPacket data{
        GAME_OVER,
        winner
    };
    ENetPacket* packet = enet_packet_create(
        &data,
        sizeof(data),
        ENET_PACKET_FLAG_RELIABLE
    );
    std::cout << "[BroadcastGameOver] broadcasting..." << std::endl;
    enet_host_broadcast(host, 0, packet);
    std::cout << "[BroadcastGameOver] exit" << std::endl;
}

void Server::HandlePacket(ENetEvent& event) {
    std::cout << "[HandlePacket] enter, dataLength=" << event.packet->dataLength << std::endl;

    if (event.packet->dataLength < sizeof(PacketType)) {
        std::cout << "[HandlePacket] packet too small to even contain a PacketType, dropping" << std::endl;
        enet_packet_destroy(event.packet);
        return;
    }

    PacketType type = *reinterpret_cast<PacketType*>(event.packet->data);
    std::cout << "[HandlePacket] parsed type=" << (int)type << std::endl;

    Player* sender = static_cast<Player*>(event.peer->data);
    std::cout << "[HandlePacket] sender ptr=" << sender
               << (sender ? (" sender->id=" + std::to_string(sender->id)) : " (null!)") << std::endl;

    if (type == MOVE) {
        std::cout << "[HandlePacket] type==MOVE, dataLength=" << event.packet->dataLength
                   << " sizeof(MovePacket)=" << sizeof(MovePacket) << std::endl;

        if (event.packet->dataLength >= sizeof(MovePacket)) {
            if (sender && !game.game_over) {
                MovePacket* packet = (MovePacket*)event.packet->data;
                std::cout << "[HandlePacket] MovePacket column=" << packet->column
                           << " packet->player(client-claimed, IGNORED)=" << packet->player << std::endl;

                std::cout << "[HandlePacket] calling game.MakeMove(sender->id=" << sender->id
                           << ", column=" << packet->column << ")..." << std::endl;
                bool moved = game.MakeMove(sender->id, packet->column);
                std::cout << "[HandlePacket] MakeMove returned " << moved << std::endl;

                if (moved) {
                    std::cout << "[HandlePacket] calling SendBoard()..." << std::endl;
                    SendBoard();
                    std::cout << "[HandlePacket] SendBoard() returned, calling CheckWin(" << sender->id << ")..." << std::endl;
                    bool won = game.CheckWin(sender->id);
                    std::cout << "[HandlePacket] CheckWin returned " << won << std::endl;
                    if (won) {
                        game.game_over = true;
                        std::cout << "[HandlePacket] calling BroadcastGameOver(" << sender->id << ")..." << std::endl;
                        BroadcastGameOver(sender->id);
                        std::cout << "[HandlePacket] BroadcastGameOver returned" << std::endl;
                    }
                }
            } else {
                std::cout << "[HandlePacket] MOVE ignored: sender=" << sender
                           << " game.game_over=" << game.game_over << std::endl;
            }
        } else {
            std::cout << "[HandlePacket] MOVE packet too small, ignoring" << std::endl;
        }
    }
    else if (type == READY) {
        std::cout << "[HandlePacket] type==READY, dataLength=" << event.packet->dataLength
                   << " sizeof(ReadyPacket)=" << sizeof(ReadyPacket) << std::endl;

        if (event.packet->dataLength >= sizeof(ReadyPacket)) {
            if (sender && game.game_over) {
                sender->ready = true;
                std::cout << "[HandlePacket] Player " << sender->id << " is ready" << std::endl;

                bool allReady = players.size() == 2;
                for (auto& p : players) {
                    std::cout << "[HandlePacket] checking player " << p.id << " ready=" << p.ready << std::endl;
                    if (!p.ready) allReady = false;
                }
                std::cout << "[HandlePacket] allReady=" << allReady << std::endl;

                if (allReady) {
                    std::cout << "[HandlePacket] calling game.EndGame()..." << std::endl;
                    game.EndGame();
                    std::cout << "[HandlePacket] EndGame() returned, game.turn=" << game.turn
                               << " game.game_over=" << game.game_over << std::endl;
                    for (auto& p : players) p.ready = false;
                    std::cout << "[HandlePacket] calling SendBoard() after reset..." << std::endl;
                    SendBoard();
                    std::cout << "[HandlePacket] SendBoard() after reset returned" << std::endl;
                }
            } else {
                std::cout << "[HandlePacket] READY ignored: sender=" << sender
                           << " game.game_over=" << game.game_over << std::endl;
            }
        }
    }
    else {
        std::cout << "[HandlePacket] unrecognized type, ignoring" << std::endl;
    }

    std::cout << "[HandlePacket] destroying packet..." << std::endl;
    enet_packet_destroy(event.packet);
    std::cout << "[HandlePacket] exit" << std::endl;
}

void Server::Run() {
    std::cout << "[Run] entering main loop" << std::endl;
    while (true)
    {
        ENetEvent event;

        while (enet_host_service(host, &event, 1000) > 0)
        {
            std::cout << "[Run] event received, type=" << event.type << std::endl;
            switch(event.type)
            {
                case ENET_EVENT_TYPE_CONNECT:
                    std::cout << "[Run] -> HandleConnect" << std::endl;
                    HandleConnect(event);
                    break;

                case ENET_EVENT_TYPE_RECEIVE:
                    std::cout << "[Run] -> HandlePacket" << std::endl;
                    HandlePacket(event);
                    break;

                case ENET_EVENT_TYPE_DISCONNECT:
                    std::cout << "[Run] -> HandleDisconnect" << std::endl;
                    HandleDisconnect(event);
                    break;

                case ENET_EVENT_TYPE_NONE:
                    break;
            }
            std::cout << "[Run] event handled" << std::endl;
        }
    }
}
