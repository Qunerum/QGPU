#include "../lib/qgpu.h"

void Init() {
    qgSetRenderType(QGPU_RENDER_TYPE_NO_LIGHT);
    qgSetFontData(3, QGPU_FONT_STYLE_REGULAR, 1, 1, 1, 1);

    qgConvertFont("fontsReadable/symbols.qfr", "fonts/symbols.qf");
    qgLoadFont("fonts/polish.qf");
    qgLoadFont("fonts/symbols.qf");
}
void Update() {

    /*
    qgAddIndex(qgAddVertex(-100, -86.602, 0,  1, 0, 0, 1));
    qgAddIndex(qgAddVertex(0, 86.602,     0,  0, 1, 0, 1));
    qgAddIndex(qgAddVertex(100, -86.602,  0,  0, 0, 1, 1));
    */

    qgAddText(-qgGetWidth() / 2.0f + 10, qgGetHeight() / 2.0f - 10, 0, "\
`1234567890-= []\\ ;' ,./\n\
~!@#$%^&*()_+ {}| :\" <>?\n\
ABCDEFGHIJKLMNOPQRSTUVWXYZ\n\
abcdefghijklmnopqrstuvwxyz\n\
\n\
\x80 \x81 \x82 \x83 \x84 \x85 \x86 \x87 \x88 \x89 \x8A \x8B \x8C\n\
\x8D \x8E \x8F \x90 \x91 \x92 \x93 \x94 \x95 \x96 \x97 \x98 \x99\n\
\n\
\xB3 \xB4 \xB5 \xB6 \xB7 \xB8 \xB9 \xBA \xBB\n\
\xBC \xBD \xBE \xBF \xC0 \xC1 \xC2 \xC3 \xC4");
}

int main() {
    qgpuCreate(1280, 720, "QGPU Template Project", Init, Update);
    return 0;
}
