#include "../lib/qgpu.h"

#define R {0.8f, 0.2f, 0.2f, 1.0f} // X
#define G {0.2f, 0.8f, 0.2f, 1.0f} // Y
#define B {0.2f, 0.2f, 0.8f, 1.0f} // Z

float angle = 0.0f;
void Init() {}
void Update() {
    angle += 0.5f;
    float x = 50.0f, y = 15.0f, z = 20.0f;
    QGPU_Vertex3D v[] = {
        {{ -x,  y,  z }, B}, // 0
        {{  x,  y,  z }, B}, // 1
        {{  x, -y,  z }, B}, // 2
        {{ -x, -y,  z }, B}, // 3

        {{ -x,  y,  z }, G}, // 4
        {{ -x,  y, -z }, G}, // 5
        {{  x,  y, -z }, G}, // 6
        {{  x,  y,  z }, G}, // 7

        {{  x,  y,  z }, R}, // 8
        {{  x,  y, -z }, R}, // 9
        {{  x, -y, -z }, R}, // 10
        {{  x, -y,  z }, R}, // 11

        {{  x, -y,  z }, G}, // 12
        {{  x, -y, -z }, G}, // 13
        {{ -x, -y, -z }, G}, // 14
        {{ -x, -y,  z }, G}, // 15

        {{ -x,  y, -z }, R}, // 16
        {{ -x,  y,  z }, R}, // 17
        {{ -x, -y,  z }, R}, // 18
        {{ -x, -y, -z }, R}, // 19

        {{  x,  y, -z }, B}, // 20
        {{ -x,  y, -z }, B}, // 21
        {{ -x, -y, -z }, B}, // 22
        {{  x, -y, -z }, B}  // 23
    };
    uint32_t i[] = {
        0, 1, 2,   0, 2, 3,
        4, 5, 6,   4, 6, 7,
        8, 9, 10,  8, 10, 11,
        12, 13, 14, 12, 14, 15,
        16, 17, 18, 16, 18, 19,
        20, 21, 22, 20, 22, 23
    };
    setCameraOrtographic(0);
    drawMesh(-100.0f, 0.0f, 500.0f, 30.0f, angle, 0.0f, v, 24, i, 36);
    setCameraOrtographic(1);
    drawMesh(100.0f, 0.0f, 250.0f, 30.0f, angle, 0.0f, v, 24, i, 36);
    drawText(-350, 120, "Perspective              Ortographic", 2, WHITE);
}

int main() {
    qgpuCreate(1280, 720, "QGPU Template Project", Init, Update);
    return 0;
}
