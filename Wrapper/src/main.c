#include "../lib/qgpu.h"

#define R {0.8f, 0.2f, 0.2f, 1.0f} // X
#define G {0.2f, 0.8f, 0.2f, 1.0f} // Y
#define B {0.2f, 0.2f, 0.8f, 1.0f} // Z

float angle = 0.0f, spd = 0.1f;
int a = 1;
float an = 90;
void Init() {
    setCameraOrtographic(0);
    addLight(0, 2, 5, 150, 5);
}
void Update() {
    angle += a ? spd : -spd;
    an += spd*3;
    if (angle > 20) a = 0; else if (angle < -20) a = 1;
    drawPlane(-20, -10, 100, angle, 0, 0, 25, 25, DARK_RED);
    drawBox(20, 0, 100, 20, an, 0, 30, 10, 40, DARK_RED);
    drawDisk(0, 2, 10, -90, angle, 0, 1, 8, DARK_GREEN);
}

int main() {
    qgpuCreate(1280, 720, "QGPU Template Project", Init, Update);
    return 0;
}
