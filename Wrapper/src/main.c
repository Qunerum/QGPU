#include "../lib/qgpu.h"

float angle = 0.0f, spd = 0.1f;
int a = 1;
float an = 90;
void Init() {
    setCameraOrtographic(0);
    addLight(0, 5, 25, 300, 2);
}
void Update() {
    angle += a ? spd : -spd;
    an += spd*3;
    if (angle > 20) a = 0; else if (angle < -20) a = 1;
    drawBox(0, 5, 25, 0, 0, 0, 1, 1, 1, (QColor){1, 1, 0, 1});

    drawPlane(-20, -10, 100, angle + 20, 20, 0, 25, 25, DARK_RED);
    drawBox(20, -10, 200, 20, an, 0, 20, 5, 20, DARK_GREEN);
    drawBox(20, -10, 100, 20, an, 0, 20, 5, 20, DARK_GREEN);
    drawDisk(0, -10, 100, 30, angle*2, 0, 5, 8, DARK_BLUE);

}

int main() {
    qgpuCreate(1280, 720, "QGPU Template Project", Init, Update);
    return 0;
}
