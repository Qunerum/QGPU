#include "../include/qgpu_core.h"
#include "../include/qgpu.h"

void qgpu_create(int width, int height, const char* title, void (*updateFunc)()) { if (qgpu_init(width, height, title)) { qgpu_run(updateFunc); } qgpu_cleanup(); }
void drawGeometry(float posX, float posY, QGPU_Vertex* vertices, uint32_t vCount, uint32_t* indices, uint32_t iCount) { qgpu_draw_geo(vertices, vCount, indices, iCount, posX, -posY); }

void drawRect(float posX, float posY, float sizeX, float sizeY, float r, float g, float b) {
    float x = sizeX / 2, y = sizeY / 2;
    QGPU_Vertex v[] = {
        {{ -x , y}, {r, g, b}},
        {{ x , y}, {r, g, b}},
        {{ x , -y}, {r, g, b}},
        {{ -x , -y}, {r, g, b}}
    };
    uint32_t i[] = {0, 1, 2,  0, 2, 3};
    drawGeometry(posX, posY, v, 4, i, 6);
}
