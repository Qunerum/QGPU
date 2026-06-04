#version 450

layout(location = 0) in vec2 fragUV;
layout(location = 0) out vec4 outColor;

// Tutaj ląduje nasza tablica pikseli jako surowe liczby uint (RGBA spakowane do 32-bitów)
layout(std430, binding = 0) buffer TextureBuffer {
    uint width;
    uint height;
    uint pixels[];
};

void main() {
    // Obliczamy indeks piksela na podstawie UV
    uint x = uint(fragUV.x * float(width));
    uint y = uint(fragUV.y * float(height));

    // Zabezpieczenie przed wyjściem poza tablicę
    if (x >= width) x = width - 1;
    if (y >= height) y = height - 1;

    uint idx = y * width + x;
    uint rawColor = pixels[idx];

    // Rozpakowujemy uint (0xAABBGGRR lub 0xRRGGBBAA w zależności od zapisu) do vec4
    // Vulkan domyślnie unpackUnorm4x8 traktuje jako RGBA
    outColor = unpackUnorm4x8(rawColor);
}
