#pragma once

#include <stdint.h>

typedef struct block_header {
    int8_t magic[6];
    uint16_t type;
    uint16_t object;
    uint16_t master;
    uint32_t crc;
    uint32_t size;
} block_header_t;

#define BLOCK_PRODUCER_ID 1

typedef struct block_producer {
    block_header_t header;
    char name[128];
    char version[10];
    char info[128];
} block_producer_t;

#define BLOCK_MAP_ID 2

typedef struct block_map {
    block_header_t header;
    char name[128];
    uint32_t width;
    uint32_t height;
    uint32_t prop_count;
    char file[256];
    uint32_t tile_count;
    uint32_t prop_color_count;
} block_map_t;