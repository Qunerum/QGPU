#include "../lib/qgpu.h"

int main() {
    qgpuSetShow(Q_SHOW_BANNER, 1);
    return qgpuInit("QGPU Project", 720, 480);
}
