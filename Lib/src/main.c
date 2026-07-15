#include "../lib/qgpu.h"

int main() {
    qgpuShowLogs(0);
    return qgpuInit("QGPU Project", 720, 480);
}
