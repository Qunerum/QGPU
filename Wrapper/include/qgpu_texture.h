#ifndef QGPU_TEXTURE_H
#define QGPU_TEXTURE_H

typedef struct {
    unsigned char* pixels;
    int pixelCount;
    int width;
    int height;
} RawTexture;

#define TEXTURES 16
extern RawTexture txts[TEXTURES];

void load_texture(const char* filename, int slot);
void cleanup_textures();
void drawTextureScaling(int slot, float scale, float posX, float posY);

#endif
