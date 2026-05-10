#include "../include/qgpu.h"

void Update() {
    drawTextureScale(400, 0, 0, 20);
    
    float t = 100, p = 70, i = 0.5;

    drawLine(-50 + p, -50 + p, -50 + p, 50 + p, t, 0, i, 0, 1);
    drawLine(-50 + p, 50 + p, 50 + p, 50 + p, t, i, 0, 0, 1);
    drawLine(50 + p, 50 + p, 50 + p, -50 + p, t, 0, i, 0, 1);
    drawLine(50 + p, -50 + p, -50 + p, -50 + p, t, i, 0, 0, 1);

    drawLine(-50, -50, -50, 50, t, 0, i, 0, 1);
    drawLine(-50, 50, 50, 50, t, i, 0, 0, 1);
    drawLine(50, 50, 50, -50, t, 0, i, 0, 1);
    drawLine(50, -50, -50, -50, t, i, 0, 0, 1);

    drawLine(-50, -50, -50 + p, -50 + p, t, 0, 0, i, 1);
    drawLine(-50, 50, -50 + p, 50 + p, t, 0, 0, i, 1);
    drawLine(50, 50, 50 + p, 50 + p, t, 0, 0, i, 1);
    drawLine(50, -50, 50 + p, -50 + p, t, 0, 0, i, 1);

    /*
    double x = 0, y = 0;
    if (getMouse(LMB)) {
        getMousePos(&x, &y);
        print("x: %.2f y: %.2f", x, y);
    }
    
    drawCircle(x, y, 10, 16, 0.5f, 0.5f, 0, 1);
    */
}

int main() {
    qgpu_create(600, 400, "QGPU Window", Update);
    return 0;
}
