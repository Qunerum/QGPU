#include "../include/qgpu.h"
#include "../include/qgpu_ui.h"

float x = 0.5f;
void Update() {
    QColor normal = {0.4f, 0.4f, 0.4f, 1.0f};
    QColor hover  = {0.5f, 0.5f, 0.5f, 1.0f};
    QColor press  = {0.3f, 0.3f, 0.3f, 1.0f};
    if (drawButton(0, 0, 200, 60, normal, hover, press) == 1) { print("Click!"); }

    drawSlider(&x, 0, 1, 0, 100, 200, 40, 10, 50, press, normal, hover);
    print("%f", x);
}

int main() {
    qgpuCreate(600, 400, "QGPU Window", Update);
    return 0;
}
