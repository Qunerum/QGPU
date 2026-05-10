#ifndef QGPU_TEXTURE_H
#define QGPU_TEXTURE_H

typedef struct {
    unsigned char* pixels;
    int width;
    int height;
} RawTexture;

#define TEXTURES 16
extern RawTexture g_texture_list[TEXTURES];

void load_texture(const char* filename, int slot);
void cleanup_textures();

#endif
