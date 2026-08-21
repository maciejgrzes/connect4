# Connect Four

A classic Connect Four game built with [raylib](https://www.raylib.com/) for rendering and [ENet](http://enet.bespin.org/) for networking. Play locally against a friend on the same screen, or host/join a game over LAN or the internet.

## Features

- **Single Player** — local hot-seat mode, two players share one screen
- **Multiplayer** — authoritative dedicated server, connect to a friend by IP
- **Cross-platform** — builds natively on Linux and Windows, with an optional (almost) fully static Windows executable

## Getting Started

You can just download the compiled game from the releases page (NOTE THE GAME HAS STILL MANY MANY BUGS)

Or you can build it by yourself.

### Requirements

- A C++ compiler (`gcc` recommended)
- [raylib](https://www.raylib.com/)
- [ENet](http://enet.bespin.org/)
- `make`

### Build

```bash
make            # builds both the game client and the server
make client     # builds (you guessed it) just the client
make server     # you won't believe it...
```

This produces:
| Binary | Platform | Description |
|---|---|---|
| `connect4` (or `connect4.exe`) | Linux / Windows | The game client |
| `serv` (or `serv.exe`) | Linux / Windows | The dedicated multiplayer server |

Run `make` from a native Linux shell for Linux binaries, or from an MSYS2/mingw64 shell on Windows for a statically-linked Windows build — the same `Makefile` detects the platform automatically.

### Cross-compiling a Windows `.exe` from Linux

If you'd rather not leave Linux, and have `mingw-w64` installed along with Windows-target builds of raylib and ENet:

## Running the Game

### Single Player

```bash
./connect4
```
Choose **Single Player** from the main menu. Two players take turns on the same keyboard/mouse.

### Multiplayer

**1. Start a server** (on whichever machine will host):
```bash
./serv
```

**2. Launch the client, choose Multiplayer:**
- **Host** — connects to `localhost`, assuming the server above is running on your own machine
- **Join** — enter the host's IP address to connect over LAN, a VPN (e.g. Tailscale), or the open internet

The server listens on port `7777` by default.

## Controls

| Action | Input |
|---|---|
| Drop a piece | Left-click a column |
| Confirm ready for next round | `Space` |
| Return to menu | `Escape` also `Caps` for some reason :3|

## How It Works

The server is fully authoritative — it owns the real game state (board, whose turn it is, win detection) and is the only source of truth. Clients send move/ready requests and render whatever the server confirms back; a client can never place a piece the server doesn't agree to. This keeps both players' boards in sync and prevents a modified or malicious client from cheating.

```
Client A ──MOVE──▶  Server  ──BOARD_UPDATE──▶  both clients
                       │                        
Client B ──READY──▶  Server  ──GAME_OVER───▶   both clients
```

## Project Structure

```
.
├── src/
│   ├── main.cpp        # client entry point, menu & game states
│   └── inc/
│       ├── game.hpp/.cpp    # core Connect Four rules & board logic
│       ├── button.hpp/.cpp  # simple UI button widget
│       ├── inputbox.hpp     # IP address text input
│       ├── funcs.hpp/.cpp   # rendering helpers
│       └── vars.hpp         # global variables 
├── network/
│   ├── ntmain.cpp       # server entry point
│   └── inc/
│       ├── server.hpp/.cpp  # server-side networking & game orchestration
│       ├── packets.hpp      # definition of packet structs
│       ├── player.hpp       # definition of player structs
│       ├── client.hpp/.cpp  # client-side networking wrapper
│       └── packets.hpp      # shared network packet definitions
└── Makefile
```

## Roadmap

- [ ] Fix massive bugs :3

- [ ] Sound effects
- [ ] Reconnect handling for dropped connections

