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
    static float x, y, z, s = 1;
    if (qgGetKey(QKEY_SPACE)) { x += s; y += s; z += s; if (x > 360) { x = 0; y = 0; z = 0; } }

    if (qgGetKey(QKEY_W)) x += s;
    if (qgGetKey(QKEY_S)) x -= s;
    if (qgGetKey(QKEY_A)) y += s;
    if (qgGetKey(QKEY_D)) y -= s;
    if (qgGetKey(QKEY_Q)) z += s;
    if (qgGetKey(QKEY_E)) z -= s;
    qgSetRotation(x, y, z);
    qgPrint("Rot: (%f, %f, %f)\n", x, y, z);

    // qgAddBox(0, 0, 0, 50, 25, 100, 0.5f, 0.5f, 0, 1);
    qgAddSphere(0, 0, 0, 25, 5, 9, .5f, .5f, 0, 1);

    // qgSetRotationPivot(-100, 0, -1);
    // qgAddRect(-100, 0, -1, 100, 100, .5f, 0, 0, 1);
    // qgSetRotationPivot(100, 0, -1);
    // qgAddCircle(100, 0, -1, 10, 100, 0.5f, 0, 0, 1);
}

int main() {
    qgpuCreate(1280, 720, "QGPU Template Project", Init, Update);
    return 0;
}
