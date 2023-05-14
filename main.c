#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "block.h"

#define MAGIC0 'G'
#define MAGIC1 'B'
#define MAGIC2 'O'
#define MAGIC3 '1'

#define SUBMAGIC0 'H'
#define SUBMAGIC1 'P'
#define SUBMAGIC2 'J'
#define SUBMAGIC3 'M'
#define SUBMAGIC4 'T'
#define SUBMAGIC5 'L'

typedef struct file {
    int8_t magic[4];
    char* block_start;
} file_t;

typedef struct buffer {
    uint32_t len;
    uint8_t *data;
} buffer_t;

void OpenFile(const char *path, buffer_t *buffer)
{
    FILE *fp = fopen(path, "r");
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    uint8_t *buf = malloc(size);
    fread(buf, size, 1, fp);
    fclose(fp);
    printf("Leo %ld bytes, los primeros son %c%c%c%c\n", size, buf[0], buf[1], buf[2], buf[3]);

    buffer->len = size;
    buffer->data = buf;
}

void PrintSection(block_header_t *block) {
    printf("\nMagic number (must be HPJMTL): ");
    for (int n = 0; n < 6; n++)
        printf("%c", block->magic[n]);
    printf("\n");

    printf("Type ID: %x\n", block->type);
    printf("Object ID: %x\n", block->object);
    printf("Master ID: %x\n", block->master);
    printf("CRC (must be 0): %0x\n", block->crc);
    printf("Size: %d bytes (0x%x)\n", block->size, block->size);
}

void DumpProducerSection(block_header_t *block)
{
    block_producer_t *producer = (block_producer_t *) block;
    PrintSection(block);
    printf("Name: %s\n", producer->name);
    printf("Version: %s\n", producer->version);
    printf("Info: %s\n", producer->info);
    printf("\n");
}

void DumpMapSection(block_header_t *block)
{
    block_map_t *map = (block_map_t *) block;
    PrintSection(block);
    printf("Name: %s\n", map->name);
    printf("Width: %d\n", map->width);
    printf("Height: %d\n", map->height);
    printf("Prop count: %d\n", map->prop_count);
    printf("Tile File: %s\n", map->file);
    printf("Tile count: %d\n", map->tile_count);
    printf("Prop Color Count: %d\n", map->prop_color_count);
    printf("\n");
}

void DumpData(buffer_t *buffer) {
    uint64_t pointer = 4;

    while (pointer < buffer->len) {
        block_header_t *block = (block_header_t *) (buffer->data + pointer);
        switch (block->type) {
            case BLOCK_PRODUCER_ID:
                DumpProducerSection(block);
                break;
            case BLOCK_MAP_ID:
                DumpMapSection(block);
                break;
            default:
                printf("Unknown section type %x\n", block->type);
                PrintSection(block);
                printf("\n");
        }
        pointer += sizeof(block_header_t) + block->size;
    }
}

void DisposeBuffer(buffer_t *buffer)
{
    free(buffer->data);
}

int main() {
    buffer_t QueBonitoElHamsterDeAfor;
    OpenFile("data.gbm", &QueBonitoElHamsterDeAfor);
    DumpData(&QueBonitoElHamsterDeAfor);
    DisposeBuffer(&QueBonitoElHamsterDeAfor);
    return 0;
}