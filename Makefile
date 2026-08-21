CXX = g++
CXXFLAGS = -Wall -Wextra

SERVER_SRCS = network/ntmain.cpp network/inc/server.cpp src/inc/game.cpp
CLIENT_SRCS = src/main.cpp src/inc/funcs.cpp src/inc/game.cpp src/inc/button.cpp network/inc/client.cpp src/inc/inputbox.cpp

INCLUDES := -Iinclude -Inetwork/inc -Iclient/inc

ifeq ($(OS),Windows_NT)
    CXXFLAGS += -DWIN32_LEAN_AND_MEAN -DNOGDI -DNOUSER -DNOMINMAX
    EXE_EXT = .exe
	LIBS = -lenet -lraylib C:/msys64/mingw64/lib/libglfw3.dll.a -lopengl32 -lgdi32 -lwinmm -lws2_32
    STATIC_FLAGS = -static -static-libgcc -static-libstdc++
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        EXE_EXT :=
        LIBS := -lenet -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
        STATIC_FLAGS :=
    endif
endif

SERVER_OUT := serv$(EXE_EXT)
CLIENT_OUT := connect4$(EXE_EXT)

.PHONY: all server client clean windows-cross windows-cross-static

all: server client

server: $(SERVER_SRCS)
	$(CXX) $(CXXFLAGS) $(STATIC_FLAGS) $(SERVER_SRCS) -o $(SERVER_OUT) $(INCLUDES) $(LIBS)

client: $(CLIENT_SRCS)
	$(CXX) $(CXXFLAGS) $(STATIC_FLAGS) $(CLIENT_SRCS) -o $(CLIENT_OUT) $(INCLUDES) $(LIBS)

clean:
	rm $(SERVER_OUT) $(CLIENT_OUT) client-win.exe client-win-static.exe
