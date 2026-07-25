#include "../lib/qgpu.h"

void Init() {
    qgSetRenderType(QGPU_RENDER_TYPE_NO_LIGHT);
}
void Update() {
    /*
    qgAddIndex(qgAddVertex(-100, -86.602, 0,  1, 0, 0, 1));
    qgAddIndex(qgAddVertex(0, 86.602,     0,  0, 1, 0, 1));
    qgAddIndex(qgAddVertex(100, -86.602,  0,  0, 0, 1, 1));
    */

    qgSetFontData(2.5f, QGPU_FONT_STYLE_REGULAR, 1, 1, 1, 1);

    qgAddText(-300, 50, 0, "\x80 \x81 \x82 \x83 \x84 \x85\n`1234567890-= []\\ ;' ,./\n~!@#$%^&*()_+ {}| :\" <>?\nABCDEFGHIJKLMNOPQRSTUVWXYZ\nabcdefghijklmnopqrstuvwxyz");
}

int main() {
    qgpuCreate(1280, 720, "QGPU Template Project", Init, Update);
    return 0;
}
