#ifndef QGPU_H
#define QGPU_H

#include "qgpu_core.h"

void qgpu_create(int width, int height, const char* title, void (*updateFunc)());
void drawGeometry(float posX, float posY, QGPU_Vertex* vertices, uint32_t vCount, uint32_t* indices, uint32_t iCount);

void drawRect(float posX, float posY, float sizeX, float sizeY, float r, float g, float b);
void drawTriangle(float posX, float posY, float p1X, float p1Y, float p2X, float p2Y, float p3X, float p3Y, float r, float g, float b);
void drawCircle(float posX, float posY, float radius, int segments, float r, float g, float b);

void drawLine(float x1, float y1, float x2, float y2, float thickness, float r, float g, float b);
void drawWireRect(float posX, float posY, float sizeX, float sizeY, float thickness, float r, float g, float b);
void drawWireTriangle(float posX, float posY, float p1X, float p1Y, float p2X, float p2Y, float p3X, float p3Y, float thickness, float r, float g, float b);
void drawWireCircle(float posX, float posY, float radius, int segments, float thickness, float r, float g, float b);

#endif
