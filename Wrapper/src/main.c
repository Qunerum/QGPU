#include "../lib/qgpu.h"

float s = 3;
int firstFrame = 1;
void Update() {
    if (firstFrame) {
        loadTexture(FILES "test.qgt", 0);
        firstFrame = 0;
    }

    if (getKey(QKEY_UP)) s++;
    if (getKey(QKEY_DOWN) && s > 0) s--;

    drawTextureScale(0, 0, 0, s);
}

int main() {
    qgpuCreate(1280, 720, "QGPU Template Project", Update);
    return 0;
}
