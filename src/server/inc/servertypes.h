#pragma once

#include "enet.h"

struct Player {
    ENetPeer* peer;
    int id;
    bool connected;
};
