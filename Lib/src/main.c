#include "../lib/qgpu.h"
uint8_t ao = 1, msaa = 8, shadows = 1;
void set() {
    qgSetGraphicsSetting(QGPU_SETTINGS_AMBIENT_OCCLUSION, ao);
    qgSetGraphicsSetting(QGPU_SETTINGS_MSAA_LEVEL, msaa);
    qgSetGraphicsSetting(QGPU_SETTINGS_SHADOWS, shadows);
    qgPrint("AO: %i | MSAA: %i | Shadows: %i\n", ao, msaa, shadows);
}
void Init() {
    qgSetRenderType(QGPU_RENDER_TYPE_LIGHT);
    qgSetFontData(3, QGPU_FONT_STYLE_REGULAR, 1, 1, 1, 1);

    qgConvertFont("fontsReadable/symbols.qfr", "fonts/symbols.qf");
    qgConvertFont("fontsReadable/polish.qfr", "fonts/polish.qf");

    qgLoadFont("fonts/symbols.qf");
    qgLoadFont("fonts/polish.qf");

    set();
}
void Update() {
    /*
    qgAddText(-(float)qgGetWidth() / 2.0f + 10, (float)qgGetHeight() / 2.0f - 10, 0, "\
`1234567890-= []\\ ;' ,./\n\
~!@#$%^&*()_+ {}| :\" <>?\n\
ABCDEFGHIJKLMNOPQRSTUVWXYZ\n\
abcdefghijklmnopqrstuvwxyz\n\n\
q;0080; q;0081; q;0082; q;0083; q;0084; q;0085; q;0086; q;0087; q;0088; q;0089; q;008A; q;008B; q;008C;\n\
q;008D; q;008E; q;008F; q;0090; q;0091; q;0092; q;0093; q;0094; q;0095; q;0096; q;0097; q;0098; q;0099;\n\n\
q;00B3; q;00B4; q;00B5; q;00B6; q;00B7; q;00B8; q;00B9; q;00BA; q;00BB;\n\
q;00BC; q;00BD; q;00BE; q;00BF; q;00C0; q;00C1; q;00C2; q;00C3; q;00C4;\n\n\
q;00C5; q;00C6; q;00C7; q;00C8; q;00C9; q;009A; q;009B; q;009C; q;009D; \n\
\n\
\n\
");
*/
    if (qgOnKey(QKEY_Q)) { ao = !ao; set(); }
    if (qgOnKey(QKEY_E)) { shadows = !shadows; set(); }

    qgAddLight(10, 0, 0, 100, 1);
    static float x = 0, s = .5f;
    x += s;
    static uint rs = 4, ors = 0;
    if (qgOnKey(QKEY_UP)) rs++;
    if (qgOnKey(QKEY_DOWN) && rs > 0) rs--;
    if (ors != rs) {
        qgPrint("Rings & Sectors: %i\n", rs);
        ors = rs;
    }
    qgSetRotation(x, x, x);
    qgAddSphere(0, 0, 0, 50, rs, rs, .6f, .6f, .6f, 1);
}

int main() {
    qgpuCreate(1280, 720, "QGPU Template Project", Init, Update);
    return 0;
}
