#include "../lib/qgpu.h"

void Init() {
}
void Update() {
    /*
    uint32_t vs[] = {
        qgAddVertex(-100, -86.602, 0,  1, 0, 0, 1),
        qgAddVertex(0, 86.602,     0,  0, 1, 0, 1),
        qgAddVertex(100, -86.602,  0,  0, 0, 1, 1),
    };
    for (int i = 0; i < (int)(sizeof(vs)/sizeof(uint32_t)); i++) qgAddIndex(vs[i]);
    */
    static float x;
    if (qgGetKey(QKEY_SPACE)) x += 1;
    qgSetRotation(0, 0, x);
    qgPrint("Rot: %f\n", x);
    qgSetRotationPivot(100, 0, 1);
    qgAddRect(100, 0, 1, 100, 100, .5f, 0, 0, 1);
    qgSetRotationPivot(-100, 0, 1);
    qgAddCircle(-100, 0, 1, 10, 100, 0.5f, 0, 0, 1);
}

int main() {
    qgpuCreate(1280, 720, "QGPU Template Project", Init, Update);
    return 0;
}
