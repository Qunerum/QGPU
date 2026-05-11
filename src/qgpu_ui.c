// #include "../include/qgpu_core.h"
#include "../include/qgpu.h"
// #include "../include/qgpu_ui.h"

int drawButton(float posX, float posY, float width, float height, QColor clr, QColor hoverClr, QColor pressClr) {
    double mx, my; getMousePos(&mx, &my);
    int hovered = AABB((float)mx, (float)my, posX, posY, width, height), o = 0;
    if (hovered) { if (onMouse(LMB)) { o = 1; } else if (getMouse(LMB)) { o = 2; } }
    drawRect(posX, posY, width, height, hovered ? o == 0 ? hoverClr : pressClr : clr);
    return o;
}
int drawSlider(float* value, float min, float max, float posX, float posY, float width, float height, float handleW, float handleH, QColor backgroundClr, QColor fillClr, QColor handleClr) {
    double mx, my; getMousePos(&mx, &my);
    int hovered = AABB((float)mx, (float)my, posX, posY, width, height);
    int changed = 0;
    if (getMouse(LMB)) {
        if (hovered) {
            float t = ((float)mx - (posX - width / 2.0f)) / width;
            if (t < 0.0f) t = 0.0f;
            if (t > 1.0f) t = 1.0f;
            *value = min + t * (max - min);
            changed = 1;
        }
    }
    float t = (*value - min) / (max - min);
    drawRect(posX, posY, width, height, backgroundClr);
    drawRect(posX - width/2 + width * t / 2, posY, width * t, height, fillClr);

    float handleX = posX - width / 2.0f + t * width;
    drawRect(handleX, posY, 10, height + 10, handleClr);

    return changed;
}
