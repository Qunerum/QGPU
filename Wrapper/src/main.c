#include "../lib/qgpu.h"

#define R {0.8f, 0.2f, 0.2f, 1.0f} // X
#define G {0.2f, 0.8f, 0.2f, 1.0f} // Y
#define B {0.2f, 0.2f, 0.8f, 1.0f} // Z

float angle = 0.0f;
void Init() {
    addLight(0, -20, 50, 750, 5);
}
void Update() {
    angle += 0.5f;

    setCameraOrtographic(0);
    drawBox(0, -20, 50, 45, 45, 0, 10, 10, 10, WHITE);

    drawBox(-100, 0, 500, 30, angle, 0, 100, 30, 40, DARK_RED);
    setCameraOrtographic(1);
    drawBox(100, 0, 500, 30, angle, 0, 100, 30, 40, DARK_GREEN);
    drawText(-350, 120, "Perspective              Ortographic", 2, WHITE);
}

int main() {
    qgpuCreate(1280, 720, "QGPU Template Project", Init, Update);
    return 0;
}
