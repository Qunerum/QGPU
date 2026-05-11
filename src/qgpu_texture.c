#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

#include "../include/qgpu_texture.h"
#include "../include/qgpu_core.h"
#include "../include/qgpu.h"

int count_files_with_ext(const char *path, const char *ext) {
    int count = 0;
    struct dirent *entry;
    struct stat statbuf;
    DIR *dir = opendir(path);

    if (!dir) return 0;
    while ((entry = readdir(dir)) != NULL) {
        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);
        if (stat(full_path, &statbuf) == -1) continue;
        if (S_ISDIR(statbuf.st_mode)) {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;
            count += count_files_with_ext(full_path, ext);
        } else {
            char *dot = strrchr(entry->d_name, '.');
            if (dot && strcmp(dot, ext) == 0) { count++; }
        }
    }
    closedir(dir);
    return count;
}
RawTexture txts[TEXTURES];

void load_texture(const char* filename, int slot) {
    if (slot < 0 || slot >= TEXTURES) return;
    if (txts[slot].pixels != NULL) {
        free(txts[slot].pixels);
        txts[slot].pixels = NULL;
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
    txts[slot].pixels = pixelData;
    txts[slot].pixelCount = pixelCount;
    txts[slot].width = width;
    txts[slot].height = height;
    fclose(file);
    printf("Loaded texture '%s' to slot '%d' (%dx%d)\n", filename, slot, width, height);
}
void cleanup_textures() { for (int i = 0; i < TEXTURES; i++) { if (txts[i].pixels != NULL) { free(txts[i].pixels); txts[i].pixels = NULL; } } }

void drawTextureScaling(int slot, float scale, float posX, float posY) {
    if (slot < 0 || slot >= 16 || txts[slot].pixels == NULL) return;
    
    RawTexture* tex = &txts[slot];
    int pixelCount = tex->width * tex->height;
    float halfW = (tex->width * scale) / 2.0f;
    float halfH = (tex->height * scale) / 2.0f;
    int vc = pixelCount * 4;
    int ic = pixelCount * 6;
    QGPU_Vertex* v = malloc(vc * sizeof(QGPU_Vertex));
    uint32_t* i_ptr = malloc(ic * sizeof(uint32_t));
    
    int vIdx = 0;
    int iIdx = 0;
    
    for (int y = 0; y < tex->height; y++) {
        for (int x = 0; x < tex->width; x++) {
            int p = (y * tex->width + x) * 4;
            float r = tex->pixels[p] / 255.0f;
            float g = tex->pixels[p + 1] / 255.0f;
            float b = tex->pixels[p + 2] / 255.0f;
            float a = tex->pixels[p + 3] / 255.0f;
            
            float x0 = (x * scale) - halfW;
            float x1 = ((x + 1) * scale) - halfW;
            
            float y0 = halfH - ((y + 1) * scale); 
            float y1 = halfH - (y * scale);
            v[vIdx + 0] = (QGPU_Vertex){{x0, y1}, {r, g, b, a}};
            v[vIdx + 1] = (QGPU_Vertex){{x1, y1}, {r, g, b, a}};
            v[vIdx + 2] = (QGPU_Vertex){{x1, y0}, {r, g, b, a}};
            v[vIdx + 3] = (QGPU_Vertex){{x0, y0}, {r, g, b, a}};
            
            uint32_t offset = vIdx;
            i_ptr[iIdx + 0] = offset + 0;
            i_ptr[iIdx + 1] = offset + 1;
            i_ptr[iIdx + 2] = offset + 2;
            i_ptr[iIdx + 3] = offset + 0;
            i_ptr[iIdx + 4] = offset + 2;
            i_ptr[iIdx + 5] = offset + 3;
    
            vIdx += 4;
            iIdx += 6;
        }
    }
    drawGeometry(posX, posY, v, vc, i_ptr, ic);
    free(v);
    free(i_ptr);
}
