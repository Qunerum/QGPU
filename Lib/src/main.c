#include "../lib/qgpu.h"

void init() {
    qgSetBackground(0.05f, 0.05f, 0.08f);
}

void update() {
    static Vector3 pos = {2, 4, 2};
    const float spd = .1f;
    if (qgGetKey(QKEY_W)) pos.z += spd;
    if (qgGetKey(QKEY_S)) pos.z -= spd;
    if (qgGetKey(QKEY_A)) pos.x += spd;
    if (qgGetKey(QKEY_D)) pos.x -= spd;
    if (qgGetKey(QKEY_Q)) pos.y -= spd;
    if (qgGetKey(QKEY_E)) pos.y += spd;

    qgSetCamera((Vector3){0, 4, -10}, (Vector3){0, 0, 0}, 60.0f);

    qgPrint("(%.2f, %.2f, %.2f)\n", pos.x, pos.y, pos.z);

    qgAddLight(pos, 100.0f, 2.0f);

    qgAddTriangle((Vector3){0,0,0}, (Vector3){1,0,0}, (Vector3){0.5f,1.5f,0}, 1,0,0,1);

    qgAddTriangle((Vector3){-5,0,-5}, (Vector3){5,0,-5}, (Vector3){5,0,5}, 0.6f,0.6f,0.6f,1.0f);
    qgAddTriangle((Vector3){-5,0,-5}, (Vector3){5,0,5}, (Vector3){-5,0,5}, 0.6f,0.6f,0.6f,1.0f);
}

int main() {
    qgpuCreate(1280, 720, "QGPU 2.0.0", init, update);
}
