#pragma once

enum PacketType {
    MOVE,
    BOARD_UPDATE,
    GAME_OVER,
    PLAYER_JOINED
};

struct MovePacket {
    PacketType type;
    int column;
    int player;
};

struct BoardPacket {
    PacketType type;
    int board[6][7];
};
