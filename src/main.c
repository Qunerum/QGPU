#include "../include/qgpu.h"

    QGPU_Vertex triangles[] = {
        {{ 0, 50}, {1.0f, 0.0f, 0.0f}},
        {{ 50,  -50}, {0.0f, 1.0f, 0.0f}},
        {{-50,  -50}, {0.0f, 0.0f, 1.0f}}
    };
    uint32_t indices[] = {0, 1, 2};
void Update() {

    drawGeometry(100, 0, triangles, 3, indices, 3);
    drawGeometry(-100, 0, triangles, 3, indices, 3);
    drawGeometry(100, 100, triangles, 3, indices, 3);
    drawGeometry(-100, 100, triangles, 3, indices, 3);
}

int main() {
    qgpu_create(600, 400, "QGPU Window", Update);
    return 0;
}
