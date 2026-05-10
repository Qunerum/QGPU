#include <stdio.h>
#include <stdlib.h>
#include "../include/qgpu_texture.h"

RawTexture g_texture_list[TEXTURES];

void load_texture(const char* filename, int slot) {
    if (slot < 0 || slot >= TEXTURES) return;
    if (g_texture_list[slot].pixels != NULL) {
        free(g_texture_list[slot].pixels);
        g_texture_list[slot].pixels = NULL;
    }
    
    FILE* file = fopen(filename, "r");
    if (!file) return;
    
    char line[16];
    int width = 0, height = 0;
    
    if (fgets(line, sizeof(line), file)) { sscanf(line, "%d %d", &width, &height); }
    
    int pixelCount = width * height;
    unsigned char* pixelData = (unsigned char*)malloc(pixelCount * 4);
    
    int currentByte = 0;
    while (fgets(line, sizeof(line), file) && currentByte < pixelCount * 4) {
        int r, g, b, a;
        if (sscanf(line, "%d %d %d %d", &r, &g, &b, &a) == 4) {
            pixelData[currentByte++] = (unsigned char)r;
            pixelData[currentByte++] = (unsigned char)g;
            pixelData[currentByte++] = (unsigned char)b;
            pixelData[currentByte++] = (unsigned char)a;
        }
    }
    g_texture_list[slot].pixels = pixelData;
    g_texture_list[slot].width = width;
    g_texture_list[slot].height = height;
    fclose(file);
    printf("Loaded texture '%s' to slot '%d' (%dx%d)\n", filename, slot, width, height);
}
void cleanup_textures() { for (int i = 0; i < TEXTURES; i++) { if (g_texture_list[i].pixels != NULL) { free(g_texture_list[i].pixels); g_texture_list[i].pixels = NULL; } } }
