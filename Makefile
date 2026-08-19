CXX = g++
CXXFLAGS = -Wall -Wextra

SRC = src/main.cpp \
      src/inc/funcs.cpp \
      src/inc/structs.cpp \
	  src/inc/button.cpp

SRC_SERVER = network/ntmain.cpp \
			 network/inc/server.cpp \
			 src/inc/structs.cpp

TARGET = connect4
TARGET_SERVER = serv

.PHONY: all clean run run_server

all: $(TARGET) $(TARGET_SERVER)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) -lraylib

clean:
	rm $(TARGET) $(TARGET_SERVER)

run: $(TARGET)
	./$(TARGET)

$(TARGET_SERVER): $(SRC_SERVER)
	$(CXX) $(CXXFLAGS) $(SRC_SERVER) -o $(TARGET_SERVER) -lenet

run_server: $(TARGET_SERVER)
	./$(TARGET_SERVER)
