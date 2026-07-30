#include "../lib/qgpu.h"

void Init() {
    qgSetRenderType(QGPU_RENDER_TYPE_NO_LIGHT);
    qgSetFontData(4, QGPU_FONT_STYLE_REGULAR, 1, 1, 1, 1);
}
void Update() {
    /*
    qgAddIndex(qgAddVertex(-100, -86.602, 0,  1, 0, 0, 1));
    qgAddIndex(qgAddVertex(0, 86.602,     0,  0, 1, 0, 1));
    qgAddIndex(qgAddVertex(100, -86.602,  0,  0, 0, 1, 1));
    */

    qgAddText(-400, 200, 0, "\
\x80 \x81 \x82 \x83 \x84 \x85\n \
`1234567890-= []\\ ;' ,./\n\
~!@#$%^&*()_+ {}| :\" <>?\n\
ABCDEFGHIJKLMNOPQRSTUVWXYZ\n\
abcdefghijklmnopqrstuvwxyz\n\
\x86 \x87 \x88 \x89 \x8A \x8B \x8C \x8D \x8E\n\
\x8F \x90 \x91 \x92 \x93 \x94 \x95 \x96 \x97\n\
\x98 \x99 \xA0 \xA1 \xA2 \xA3 \xA4");
}

int main() {
    qgpuCreate(1280, 720, "QGPU Template Project", Init, Update);
    return 0;
}
