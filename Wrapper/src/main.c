#include "../lib/qgpu.h"

float x = 1;
int toggle = 0;
QColor normal = {0.4f, 0.4f, 0.4f, 1.0f};
QColor hover  = {0.5f, 0.5f, 0.5f, 1.0f};
QColor press  = {0.3f, 0.3f, 0.3f, 1.0f};

float s = 50;
int firstFrame = 1;
void Update() {
    if (firstFrame) {
        loadTexture(FILES "test.qgt", 0);
        firstFrame = 0;
    }
    // if (drawButton(0, 0, 200, 60, normal, hover, press) == 1) print("Click!");

    // if (drawSlider(0, 100, 200, 40, 10, 50, &x, 0, 2, press, normal, hover)) print("%f", x);

    // if (drawToggle(0, -100, 20, 20, &toggle, normal, hover)) print("Toggle!");

    // drawText(px, py, "`1234567890-=\n~!@#$%^&*()_+\nabcdefghijklmnopqrstuvwxyz\nABCDEFGHIJKLMNOPQRSTUVWXYZ", s, normal);

    if (getKey(QKEY_UP)) s++;
    if (getKey(QKEY_DOWN) && s > 0) s--;

    drawTextureScale(0, 0, 0, s);
}

int main() {
    qgpuCreate(1280, 720, "QGPU Template Project", Update);
    return 0;
}
