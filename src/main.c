#include "../include/qgpu.h"
void Update() {
    drawRect(-120, 50, 200, 100, 0, 0.5f, 0);
    drawWireRect(-120, 50, 200, 100, 3, 0, 0.75f, 0);

    drawCircle(100, 0, 50, 12, 0, 0, 0.2f);
    drawWireCircle(100, 0, 50, 12, 3, 0, 0, 0.45f);

    drawTriangle(50, 100, -50, -25, 0, 25,   50, -50, 0.3f, 0, 0);
    drawWireTriangle(50, 100, -50, -25, 0, 25,   50, -50, 3, 0.55f, 0, 0);
}

int main() {
    qgpu_create(600, 400, "QGPU Window", Update);
    return 0;
}
