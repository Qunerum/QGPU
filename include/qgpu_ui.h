#ifndef QGPU_UI_H
#define QGPU_UI_H

#include "qgpu.h"

int drawButton(float posX, float posY, float width, float height, QColor clr, QColor hoverClr, QColor pressClr);
int drawSlider(float* value, float min, float max, float posX, float posY, float width, float height, float handleW, float handleH, QColor backgroundClr, QColor fillClr, QColor handleClr);

#endif
