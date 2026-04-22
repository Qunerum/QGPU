#ifndef QGPU_H
#define QGPU_H

#include "qgpu_core.h"

void qgpu_create(int width, int height, const char* title, void (*updateFunc)());
void drawGeometry(float posX, float posY, QGPU_Vertex* vertices, uint32_t vCount, uint32_t* indices, uint32_t iCount);

#endif
