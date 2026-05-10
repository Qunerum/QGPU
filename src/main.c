#include "../include/qgpu.h"

void Update() {
    drawTextureScale(100, 0, 0, 50);
    
    drawRect(-100, 0, 100, 100, 1, 0, 0, 0.5f);
    drawRect(-150, 30, 100, 100, 0, 1, 0, 0.5f);
    
    double x = 0, y = 0;
    if (getMouse(LMB)) {
        getMousePos(&x, &y);
        print("x: %.2f y: %.2f", x, y);
    }
    
    drawCircle(x, y, 10, 16, 0.5f, 0.5f, 0, 1);
}

int main() {
    qgpu_create(600, 400, "QGPU Window", Update);
    return 0;
}
