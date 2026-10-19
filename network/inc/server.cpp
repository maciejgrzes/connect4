#include "server.hpp"
#include "packets.hpp"
#include <cstdint>
#include <cstring>
#include <enet/enet.h>
#include <iostream>
#include <ostream>

#define VLOG(msg) do { if (verbose) { std::cout << msg << std::endl; } } while (0)

bool Server::Start(int port, int maxClients) {
    VLOG("[Start] enet_initialize...");
    if (enet_initialize () != 0) {
        std::cerr << "[Start] An error occurred while initializing ENet." << std::endl;
        return false;
    }

    ENetAddress address{};
    address.host = ENET_HOST_ANY;
    address.port = port;

    VLOG("[Start] enet_host_create on port " << port << " maxClients=" << maxClients);
    host = enet_host_create(&address, maxClients, 2, 0, 0);

    if (host == nullptr) {
        std::cerr << "[Start] An error occurred while trying to create an ENet server host." << std::endl;
        return false;
    }

    VLOG("[Start] Started a server...");
    return true;
}

void Server::HandleConnect(ENetEvent& event) {
    VLOG("[HandleConnect] enter, current players.size()=" << players.size());

    if (players.size() >= 2) {
        std::cerr << "[HandleConnect] No more space for new players" << std::endl;
        return;
    }
    Player pl;
    pl.connected = true;
    pl.peer = event.peer;
    pl.id = players.empty() ? 1 : 2;

    VLOG("[HandleConnect] assigning id=" << (int)pl.id << ", pushing to list...");
    players.push_back(pl);
    event.peer->data = &players.back();
    VLOG("[HandleConnect] Player " << (int)pl.id << " connected, peer->data set to " << event.peer->data);

    PlayerJoinedPacket data {PLAYER_JOINED, (uint8_t)pl.id};
    VLOG("[HandleConnect] creating PLAYER_JOINED packet, sizeof=" << sizeof(data));
    ENetPacket* packet = enet_packet_create(&data, sizeof(data), ENET_PACKET_FLAG_RELIABLE);
    VLOG("[HandleConnect] sending PLAYER_JOINED to peer...");
    enet_peer_send(event.peer, 0, packet);
    VLOG("[HandleConnect] exit");
}

void Server::HandleDisconnect(ENetEvent& event) {
    VLOG("[HandleDisconnect] enter, peer->data=" << event.peer->data);
    Player* player = static_cast<Player*>(event.peer->data);
    if (player) {
        VLOG("[HandleDisconnect] Player " << player->id << " disconnected, removing from list");
        players.remove_if([player](const Player& p) { return &p == player; });
    } else {
        VLOG("[HandleDisconnect] peer->data was null, nothing to remove");
    }
    event.peer->data = nullptr;
    VLOG("[HandleDisconnect] exit, players.size()=" << players.size());
}

void Server::SendBoard() {
    VLOG("[SendBoard] enter, turn=" << turn);
    BoardPacket data{};
    data.type = PacketType::BOARD_UPDATE;

    VLOG("[SendBoard] memcpy board, sizeof(board)=" << sizeof(board)
         << " sizeof(data.board)=" << sizeof(data.board));
    std::memcpy(data.board, board, sizeof(board));

    data.turn = turn;

    VLOG("[SendBoard] creating packet, sizeof(data)=" << sizeof(data));
    ENetPacket* packet = enet_packet_create(
        &data,
        sizeof(data),
        ENET_PACKET_FLAG_RELIABLE
    );

    VLOG("[SendBoard] broadcasting...");
    enet_host_broadcast(host, 0, packet);
    VLOG("[SendBoard] exit");
}

void Server::BroadcastGameOver(uint8_t winner) {
    VLOG("[BroadcastGameOver] enter, winner=" << (int)winner);
    GameOverPacket data{
        GAME_OVER,
        winner
    };
    ENetPacket* packet = enet_packet_create(
        &data,
        sizeof(data),
        ENET_PACKET_FLAG_RELIABLE
    );
    VLOG("[BroadcastGameOver] broadcasting...");
    enet_host_broadcast(host, 0, packet);
    VLOG("[BroadcastGameOver] exit");
}

void Server::HandlePacket(ENetEvent& event) {
    VLOG("[HandlePacket] enter, dataLength=" << event.packet->dataLength);

    if (event.packet->dataLength < sizeof(PacketType)) {
        VLOG("[HandlePacket] packet too small to even contain a PacketType, dropping");
        enet_packet_destroy(event.packet);
        return;
    }

    PacketType type = *reinterpret_cast<PacketType*>(event.packet->data);
    VLOG("[HandlePacket] parsed type=" << (int)type);

    Player* sender = static_cast<Player*>(event.peer->data);
    VLOG("[HandlePacket] sender ptr=" << sender
         << (sender ? (" sender->id=" + std::to_string(sender->id)) : " (null!)"));

    if (type == MOVE) {
        VLOG("[HandlePacket] type==MOVE, dataLength=" << event.packet->dataLength
             << " sizeof(MovePacket)=" << sizeof(MovePacket));

        if (event.packet->dataLength >= sizeof(MovePacket)) {
            if (sender && !game_over) {
                MovePacket* packet = (MovePacket*)event.packet->data;
                VLOG("[HandlePacket] MovePacket column=" << packet->column
                     << " packet->player(client-claimed, IGNORED)=" << packet->player);

                VLOG("[HandlePacket] calling MakeMove(sender->id=" << sender->id
                     << ", column=" << packet->column << ")...");
                bool moved = MakeMove(sender->id, packet->column);
                VLOG("[HandlePacket] MakeMove returned " << moved);

                if (moved) {
                    VLOG("[HandlePacket] calling SendBoard()...");
                    SendBoard();

                    VLOG("[HandlePacket] SendBoard() returned, calling CheckWin(1)...");
                    bool p1Won = CheckWin(1);
                    VLOG("[HandlePacket] CheckWin(1) returned " << p1Won);

                    VLOG("[HandlePacket] calling CheckWin(2)...");
                    bool p2Won = CheckWin(2);
                    VLOG("[HandlePacket] CheckWin(2) returned " << p2Won);

                    if (p1Won || p2Won) {
                        uint8_t winner = p1Won ? 1 : 2;
                        game_over = true;
                        VLOG("[HandlePacket] calling BroadcastGameOver(" << (int)winner << ")...");
                        BroadcastGameOver(winner);
                        VLOG("[HandlePacket] BroadcastGameOver returned");
                    } else {
                        VLOG("[HandlePacket] no winner, calling FullBoard(game)...");
                        bool full = FullBoard();
                        FullBoard();
                        VLOG("[HandlePacket] FullBoard returned " << full);
                        if (full) {
                            game_over = true;
                            VLOG("[HandlePacket] board full, no winner -> draw, calling BroadcastGameOver(0)...");
                            BroadcastGameOver(0);
                            VLOG("[HandlePacket] BroadcastGameOver returned");
                        }
                    }
                }
            } else {
                VLOG("[HandlePacket] MOVE ignored: sender=" << sender
                     << " game_over=" << game_over);
            }
        } else {
            VLOG("[HandlePacket] MOVE packet too small, ignoring");
        }
    }
    else if (type == READY) {
        VLOG("[HandlePacket] type==READY, dataLength=" << event.packet->dataLength
             << " sizeof(ReadyPacket)=" << sizeof(ReadyPacket));

        if (event.packet->dataLength >= sizeof(ReadyPacket)) {
            if (sender && game_over) {
                sender->ready = true;
                VLOG("[HandlePacket] Player " << sender->id << " is ready");

                bool allReady = players.size() == 2;
                for (auto& p : players) {
                    VLOG("[HandlePacket] checking player " << p.id << " ready=" << p.ready);
                    if (!p.ready) allReady = false;
                }
                VLOG("[HandlePacket] allReady=" << allReady);

                if (allReady) {
                    VLOG("[HandlePacket] calling EndGame()...");
                    EndGame();
                    VLOG("[HandlePacket] EndGame() returned, turn=" << turn
                         << " game_over=" << game_over);
                    for (auto& p : players) p.ready = false;
                    VLOG("[HandlePacket] calling SendBoard() after reset...");
                    SendBoard();
                    VLOG("[HandlePacket] SendBoard() after reset returned");
                }
            } else {
                VLOG("[HandlePacket] READY ignored: sender=" << sender
                     << " game_over=" << game_over);
            }
        }
    }
    else {
        VLOG("[HandlePacket] unrecognized type, ignoring");
    }

    VLOG("[HandlePacket] destroying packet...");
    enet_packet_destroy(event.packet);
    VLOG("[HandlePacket] exit");
}

void Server::Run() {
    VLOG("[Run] entering main loop");
    while (true)
    {
        ENetEvent event;

        while (enet_host_service(host, &event, 1000) > 0)
        {
            VLOG("[Run] event received, type=" << event.type);
            switch(event.type)
            {
                case ENET_EVENT_TYPE_CONNECT:
                    VLOG("[Run] -> HandleConnect");
                    HandleConnect(event);
                    break;

                case ENET_EVENT_TYPE_RECEIVE:
                    VLOG("[Run] -> HandlePacket");
                    HandlePacket(event);
                    break;

                case ENET_EVENT_TYPE_DISCONNECT:
                    VLOG("[Run] -> HandleDisconnect");
                    HandleDisconnect(event);
                    break;

                case ENET_EVENT_TYPE_NONE:
                    break;
            }
            VLOG("[Run] event handled");
        }
    }
}

#undef VLOG
