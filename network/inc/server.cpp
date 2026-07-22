#include "server.hpp"



bool Start(int port);

void Run();


void HandleConnect(ENetEvent&);

void HandleDisconnect(ENetEvent&);

void HandlePacket(ENetEvent&);

void SendBoard();

void BroadcastGameOver();
