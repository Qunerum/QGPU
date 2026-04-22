#include "../include/qgpu_core.h"

void qgpu_create(int width, int height, const char* title, void (*updateFunc)()) { if (qgpu_init(width, height, title)) { qgpu_run(updateFunc); } qgpu_cleanup(); }
void drawGeometry(float posX, float posY, QGPU_Vertex* vertices, uint32_t vCount, uint32_t* indices, uint32_t iCount) { qgpu_draw_geo(vertices, vCount, indices, iCount, posX, -posY); }
