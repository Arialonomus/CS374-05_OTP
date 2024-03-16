CC = gcc
CFLAGS = -std=c99 -Iheaders
DEBUG_FLAGS = -g
RELEASE_FLAGS = -O2
LDFLAGS =
ENC_FLAG = -DENC

# Define the source directory, output binary directory, and build directory
SRC_DIR = source
BIN_DIR = bin
BUILD_DIR = build

#Sources for each program
SHARED_SRC = $(wildcard $(SRC_DIR)/shared/*.c)
KEYGEN_SRC = $(wildcard $(SRC_DIR)/keygen/*.c)
SERVER_SRC = $(wildcard $(SRC_DIR)/server/*.c)
ENC_CLIENT_SRC = $(wildcard $(SRC_DIR)/enc_client/*.c)
DEC_CLIENT_SRC = $(wildcard $(SRC_DIR)/dec_client/*.c)

# Programs
PROGRAMS = keygen dec_server dec_client enc_server enc_client

# Ensure the release and debug directories are created
$(shell mkdir -p $(BIN_DIR)/release)
$(shell mkdir -p $(BIN_DIR)/debug)
$(shell mkdir -p $(BUILD_DIR)/release)
$(shell mkdir -p $(BUILD_DIR)/debug)

# Default rule for making release and debug
all: release debug

# Program specific build commands
keygen: release_keygen debug_keygen

enc_server: release_enc_server debug_enc_server

enc_client: release_enc_client debug_enc_client

dec_server: release_dec_server debug_dec_server

dec_client: release_dec_client debug_dec_client

# Release builds
release: release_keygen release_enc_server release_enc_client release_dec_server release_dec_client

release_keygen:
	$(CC) $(CFLAGS) $(RELEASE_FLAGS) $(KEYGEN_SRC) -o $(BIN_DIR)/release/keygen

release_enc_server:
	$(CC) $(CFLAGS) $(ENC_FLAG) $(RELEASE_FLAGS) $(SERVER_SRC) $(SHARED_SRC) -o $(BIN_DIR)/release/enc_server

release_enc_client:
	$(CC) $(CFLAGS) $(RELEASE_FLAGS) $(ENC_CLIENT_SRC) $(SHARED_SRC) -o $(BIN_DIR)/release/enc_client

release_dec_server:
	$(CC) $(CFLAGS) $(RELEASE_FLAGS) $(SERVER_SRC) $(SHARED_SRC) -o $(BIN_DIR)/release/dec_server

release_dec_client:
	$(CC) $(CFLAGS) $(RELEASE_FLAGS) $(DEC_CLIENT_SRC) $(SHARED_SRC) -o $(BIN_DIR)/release/dec_client

# Debug builds
debug: debug_keygen debug_enc_server debug_enc_client debug_dec_server debug_dec_client

debug_keygen:
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) $(KEYGEN_SRC) -o $(BIN_DIR)/debug/keygen

debug_enc_server:
	$(CC) $(CFLAGS) $(ENC_FLAG) $(DEBUG_FLAGS) $(SERVER_SRC) $(SHARED_SRC) -o $(BIN_DIR)/debug/enc_server

debug_enc_client:
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) $(ENC_CLIENT_SRC) $(SHARED_SRC) -o $(BIN_DIR)/debug/enc_client

debug_dec_server:
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) $(SERVER_SRC) $(SHARED_SRC) -o $(BIN_DIR)/debug/dec_server

debug_dec_client:
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) $(DEC_CLIENT_SRC) $(SHARED_SRC) -o $(BIN_DIR)/debug/dec_client

# Clean up build and bin directories
clean:
	rm -rf $(BUILD_DIR)/release/* $(BUILD_DIR)/debug/*
	rm -rf $(BIN_DIR)/release/* $(BIN_DIR)/debug/*

.PHONY: all clean release debug release_keygen release_dec_server release_dec_client release_enc_server release_enc_client debug_keygen debug_dec_server debug_dec_client debug_enc_server debug_enc_client