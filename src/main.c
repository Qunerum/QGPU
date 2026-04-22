#include "../include/qgpu.h"
void Update() {
    drawRect(-120, 50, 200, 100, 0, 0.5f, 0);
    drawRect(0, 0, 200, 100, 1, 0.5f, 0);
    drawRect(120, 50, 200, 100, 0.5f, 0.5f, 0);
}

int main() {
    qgpu_create(600, 400, "QGPU Window", Update);
    return 0;
}
