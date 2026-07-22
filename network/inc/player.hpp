#pragma once

#include <enet/enet.h>

struct Player {
    ENetPeer* peer = nullptr;

    int id = -1;

    bool connected = false;
};
