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
    static float x, y, z, r, p, s = 1;
    if (qgGetKey(QKEY_S)) z += s;
    if (qgGetKey(QKEY_W)) z -= s;
    if (qgGetKey(QKEY_D)) x += s;
    if (qgGetKey(QKEY_A)) x -= s;
    if (qgGetKey(QKEY_E)) y += s;
    if (qgGetKey(QKEY_Q)) y -= s;
    if (qgGetKey(QKEY_UP)) r += s;
    if (qgGetKey(QKEY_DOWN)) r -= s;
    if (qgGetKey(QKEY_RIGHT)) p += s;
    if (qgGetKey(QKEY_LEFT)) p -= s;

    qgPrint("FPS: %.1f Light pos: (%03.1f, %03.1f, %03.1f) Range: %03.1f Power: %03.1f\n", qgGetFPS(), x, y, z, r, p);

    // qgAddBox(0, 0, 0, 50, 25, 100, 0.5f, 0.5f, 0, 1);
    qgAddLight(x, y, z, r, p);
    qgAddSphere(x, y, z, 10, 3, 3, .5f, .5f, .5f, 1);

    qgAddSphere(0, 0, 0, 60, 16, 32, .5f, .5f, 0, 1);

    // qgSetRotationPivot(-100, 0, -1);
    // qgAddRect(-100, 0, -1, 100, 100, .5f, 0, 0, 1);
    // qgSetRotationPivot(100, 0, -1);
    // qgAddCircle(100, 0, -1, 10, 100, 0.5f, 0, 0, 1);
}

int main() {
    qgpuCreate(1280, 720, "QGPU Template Project", Init, Update);
    return 0;
}
