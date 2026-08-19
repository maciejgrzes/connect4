#pragma once

#include <cstdint>
enum PacketType {
    MOVE,
    BOARD_UPDATE,
    GAME_OVER,
    PLAYER_JOINED,
    READY
};

struct MovePacket {
    PacketType type;
    uint8_t column;
    uint8_t player;
};

struct BoardPacket {
    PacketType type;
    uint32_t board[6][7];
    uint8_t turn;
};

struct GameOverPacket {
    PacketType type;
    uint8_t winner;
};

struct ReadyPacket {
    PacketType type;
};

struct PlayerJoinedPacket {
    PacketType type;
    uint8_t assignedId;
};
