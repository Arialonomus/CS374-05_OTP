CC = gcc
CFLAGS = -std=c99 -Iinclude
DEBUGFLAGS = -g
RELEASEFLAGS = -O2
LDFLAGS =

# Define the source directory, output binary directory, and build directory
SRC_DIR = source
BIN_DIR = bin
BUILD_DIR = build

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

dec_server: release_dec_server debug_dec_server

dec_client: release_dec_client debug_dec_client

enc_server: release_enc_server debug_enc_server

enc_client: release_enc_client debug_enc_client

# Release builds
release: release_keygen release_dec_server release_dec_client release_enc_server release_enc_client

release_keygen:
	$(CC) $(CFLAGS) $(RELEASEFLAGS) $(SRC_DIR)/keygen/keygen.c -o $(BIN_DIR)/release/keygen

release_dec_server:
	$(CC) $(CFLAGS) $(RELEASEFLAGS) $(SRC_DIR)/dec_server/*.c -o $(BIN_DIR)/release/dec_server

release_dec_client:
	$(CC) $(CFLAGS) $(RELEASEFLAGS) $(SRC_DIR)/dec_client/*.c -o $(BIN_DIR)/release/dec_client

release_enc_server:
	$(CC) $(CFLAGS) $(RELEASEFLAGS) $(SRC_DIR)/enc_server/*.c -o $(BIN_DIR)/release/enc_server

release_enc_client:
	$(CC) $(CFLAGS) $(RELEASEFLAGS) $(SRC_DIR)/enc_client/*.c -o $(BIN_DIR)/release/enc_client

# Debug builds
debug: debug_keygen debug_dec_server debug_dec_client debug_enc_server debug_enc_client

debug_keygen:
	$(CC) $(CFLAGS) $(DEBUGFLAGS) $(SRC_DIR)/keygen/*.c -o $(BIN_DIR)/debug/keygen

debug_dec_server:
	$(CC) $(CFLAGS) $(DEBUGFLAGS) $(SRC_DIR)/dec_server/*.r.c -o $(BIN_DIR)/debug/dec_server

debug_dec_client:
	$(CC) $(CFLAGS) $(DEBUGFLAGS) $(SRC_DIR)/dec_client/*.c -o $(BIN_DIR)/debug/dec_client

debug_enc_server:
	$(CC) $(CFLAGS) $(DEBUGFLAGS) $(SRC_DIR)/enc_server/*.c -o $(BIN_DIR)/debug/enc_server

debug_enc_client:
	$(CC) $(CFLAGS) $(DEBUGFLAGS) $(SRC_DIR)/enc_client/*.c -o $(BIN_DIR)/debug/enc_client

# Clean up build and bin directories
clean:
	rm -rf $(BUILD_DIR)/release/* $(BUILD_DIR)/debug/*
	rm -rf $(BIN_DIR)/release/* $(BIN_DIR)/debug/*

.PHONY: all clean release debug release_keygen release_dec_server release_dec_client release_enc_server release_enc_client debug_keygen debug_dec_server debug_dec_client debug_enc_server debug_enc_client
