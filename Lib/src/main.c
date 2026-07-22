#include "../lib/qgpu.h"

void Init() {

}
void Update() {
    uint32_t vs[] = {
        qgAddVertex(-100, -86.602, 0,  1, 0, 0, 1),
        qgAddVertex(0, 86.602,     0,  0, 1, 0, 1),
        qgAddVertex(100, -86.602,  0,  0, 0, 1, 1),
    };
    for (int i = 0; i < (int)(sizeof(vs)/sizeof(uint32_t)); i++) qgAddIndex(vs[i]);

    qgAddCircle(100, 0, 1, 10, 100, 0.5f, 0, 0, 1);
}

int main() {
    qgpuCreate(1280, 720, "QGPU Template Project", Init, Update);
    return 0;
}
