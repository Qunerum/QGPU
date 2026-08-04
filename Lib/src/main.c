#include "../lib/qgpu.h"

void Init() {
    qgSetRenderType(QGPU_RENDER_TYPE_NO_LIGHT);
    qgSetFontData(3, QGPU_FONT_STYLE_REGULAR, 1, 1, 1, 1);

    qgLoadFont("fonts/polish.qf");
    qgLoadFont("fonts/symbols.qf");
}
void Update() {

    qgAddText(-qgGetWidth() / 2.0f + 10, qgGetHeight() / 2.0f - 10, 0, "\
`1234567890-= []\\ ;' ,./\n\
~!@#$%^&*()_+ {}| :\" <>?\n\
ABCDEFGHIJKLMNOPQRSTUVWXYZ\n\
abcdefghijklmnopqrstuvwxyz\n\n\
q;0080; q;0081; q;0082; q;0083; q;0084; q;0085; q;0086; q;0087; q;0088; q;0089; q;008A; q;008B; q;008C;\n\
q;008D; q;008E; q;008F; q;0090; q;0091; q;0092; q;0093; q;0094; q;0095; q;0096; q;0097; q;0098; q;0099;\n\n\
q;00B3; q;00B4; q;00B5; q;00B6; q;00B7; q;00B8; q;00B9; q;00BA; q;00BB;\n\
q;00BC; q;00BD; q;00BE; q;00BF; q;00C0; q;00C1; q;00C2; q;00C3; q;00C4;");
}

int main() {
    qgpuCreate(1280, 720, "QGPU Template Project", Init, Update);
    return 0;
}
