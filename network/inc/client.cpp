#include "client.hpp"
#include "packets.hpp"
#include <cstring>
#include <iostream>

bool Client::Connect(const char* ip, int port, int timeoutMs) {
    if (enet_initialize() != 0) {
        std::cerr << "Failed to initialize ENet." << std::endl;
        return false;
    }

    host = enet_host_create(nullptr, 1, 2, 0, 0);   // client: 1 outgoing connection, 2 channels
    if (!host) {
        std::cerr << "Failed to create client host." << std::endl;
        return false;
    }

    ENetAddress address{};
    enet_address_set_host(&address, ip);
    address.port = port;

    peer = enet_host_connect(host, &address, 2, 0);
    if (!peer) {
        std::cerr << "No available peers for connection." << std::endl;
        return false;
    }

    ENetEvent event;
    if (enet_host_service(host, &event, timeoutMs) > 0 && event.type == ENET_EVENT_TYPE_CONNECT) {
        connected = true;
        return true;
    }

    enet_peer_reset(peer);
    peer = nullptr;
    std::cerr << "Connection to " << ip << ":" << port << " failed/timed out." << std::endl;
    return false;
}

void Client::Disconnect() {
    if (peer) {
        enet_peer_disconnect(peer, 0);
        // give the disconnect a brief moment to actually send, non-blocking-ish
        ENetEvent event;
        while (enet_host_service(host, &event, 100) > 0) {
            if (event.type == ENET_EVENT_TYPE_DISCONNECT) break;
            if (event.type == ENET_EVENT_TYPE_RECEIVE) enet_packet_destroy(event.packet);
        }
        peer = nullptr;
    }
    if (host) {
        enet_host_destroy(host);
        host = nullptr;
    }
    connected = false;
    myPlayerId = 0;
}

void Client::Poll() {
    if (!host) return;

    ENetEvent event;
    while (enet_host_service(host, &event, 0) > 0) {
        switch (event.type) {
            case ENET_EVENT_TYPE_RECEIVE: {
                if (event.packet->dataLength < sizeof(PacketType)) {
                    enet_packet_destroy(event.packet);
                    break;
                }
                PacketType type = *reinterpret_cast<PacketType*>(event.packet->data);

                if (type == BOARD_UPDATE && event.packet->dataLength >= sizeof(BoardPacket)) {
                    BoardPacket* p = (BoardPacket*)event.packet->data;
                    std::memcpy(board, p->board, sizeof(board));
                    turn = p->turn;

                    for (int i = 0; i < COLS; i++) {
                        int h = -1;
                        for (int r = ROWS - 1; r >= 0; r--) {
                            if (board[r][i] == 0) { h = r; break; }
                        }
                        heights[i] = h;
                    }

                    game_over = false;
                    winner = 0;
                    winningCells.clear();
                }
                else if (type == GAME_OVER && event.packet->dataLength >= sizeof(GameOverPacket)) {
                    GameOverPacket* p = (GameOverPacket*)event.packet->data;
                    game_over = true;
                    winner = p->winner;
                    FindWinningLine(winner);
                }
                else if (type == PLAYER_JOINED && event.packet->dataLength >= sizeof(PlayerJoinedPacket)) {
                    PlayerJoinedPacket* p = (PlayerJoinedPacket*)event.packet->data;
                    myPlayerId = p->assignedId;
                    std::cout << "Assigned player id: " << myPlayerId << std::endl;
                }

                enet_packet_destroy(event.packet);
                break;
            }
            case ENET_EVENT_TYPE_DISCONNECT:
                connected = false;
                std::cout << "Disconnected from server." << std::endl;
                break;
            default:
                break;
        }
    }
}

void Client::SendMove(int column) {
    if (!peer) return;
    MovePacket data{ MOVE, (uint8_t)column, (uint8_t)myPlayerId };
    ENetPacket* packet = enet_packet_create(&data, sizeof(data), ENET_PACKET_FLAG_RELIABLE);
    enet_peer_send(peer, 0, packet);
}

void Client::SendReady() {
    if (!peer) return;
    ReadyPacket data{ READY };
    ENetPacket* packet = enet_packet_create(&data, sizeof(data), ENET_PACKET_FLAG_RELIABLE);
    enet_peer_send(peer, 0, packet);
}
