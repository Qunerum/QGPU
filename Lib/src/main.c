#include "../lib/qgpu.h"

void Init() {
    qgpuSetShow(QGPU_SHOW_LOGS, 0);
}
void Update() {
    uint32_t vs[] = {
        addVertex(-100, -86.602, 0, 1, 0, 0, 1),
        addVertex(0, 86.602, 0, 0, 1, 0, 1),
        addVertex(100, -86.602, 0, 0, 0, 1, 1)
    };
    for (int i = 0; i < (int)(sizeof(vs)/sizeof(uint32_t)); i++) {
        addIndex(vs[i]);
    }

}

int main() {
    qgpuCreate(1280, 720, "QGPU Template Project", Init, Update);
    return 0;
}
