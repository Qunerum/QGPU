// #define QGPU_COLORS
#define QGPU_SHAPES
#include "../lib/qgpu.h"

void init() {
	qgSetBackground(0.05f, 0.05f, 0.08f);
}

void update() {
	qgSetCamera((Vector3){-5, 4, -7}, (Vector3){0, 0, 0}, 60.0f);
	qgAddLight((Vector3){1, 3, 1}, 10.0f, 2.0f);

	qgSetLayerUI(1);
	qgAddText((Vector2){-545, 20}, "Hello");
	qgSetLayerUI(0);
	qgAddRect((Vector2){-500, 0}, (Vector2){100, 50}, .2f,.2f,.2f,1);

	static float spd = .25f;
	if (qgGetKey(QKEY_UP)) spd += 0.01f;
	if (qgGetKey(QKEY_DOWN)) spd -= 0.01f;
	qgPrint("Speed: %.2f\n", spd);
	static float r = 0;
	r += spd;
	qgSetRotation(-90, r, 0);
	qgSetRotationPivot(0, 0.5f, 0);
	qgAddPlane((Vector3){0, 0.5f, 0}, (Vector2){1, 1}, 1,.2f,.2f,1);
	qgSetRotation(90, r, 0);
	qgAddPlane((Vector3){0, 0.5f, 0}, (Vector2){1, 1}, 1,.2f,.2f,1);

	qgSetRotation(0, 0, 0);

	qgAddPlane((Vector3){0, 0, 0}, (Vector2){10, 10}, .6f,.6f,.6f,1);
	qgAddPlane((Vector3){0, 0.5, 0}, (Vector2){2, 2}, .2f,1,.2f,1.0f);
}

int main() { qgpuCreate(1280, 720, "QGPU 2.2.0", init, update); }
