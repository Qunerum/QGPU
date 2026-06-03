#include "../lib/qgpu.h"

float x = 1;
int toggle = 0;
float px = -800, py = 0;
float spd = 5;
QColor normal = {0.4f, 0.4f, 0.4f, 1.0f};
QColor hover  = {0.5f, 0.5f, 0.5f, 1.0f};
QColor press  = {0.3f, 0.3f, 0.3f, 1.0f};

float s = 30;
void Update() {
    // if (drawButton(0, 0, 200, 60, normal, hover, press) == 1) print("Click!");

    // if (drawSlider(0, 100, 200, 40, 10, 50, &x, 0, 2, press, normal, hover)) print("%f", x);

    // if (drawToggle(0, -100, 20, 20, &toggle, normal, hover)) print("Toggle!");

    drawText(px, py, "abc\nABCDEFGHIJKLMNOPQRSTUVWXYZ", s, normal);

    if (getKey(QKEY_UP)) s++;
    if (getKey(QKEY_DOWN) && s > 0) s--;

    // drawTextureScale(px, py, 0, 40);
    if (getKey(QKEY_W)) py += spd;
    if (getKey(QKEY_S)) py -= spd;
    if (getKey(QKEY_A)) px -= spd;
    if (getKey(QKEY_D)) px += spd;
}

int main() {
    loadTexture(FILES "test.qgt", 0);
    qgpuCreate(1280, 720, "QGPU Template Project", Update);
    return 0;
}
