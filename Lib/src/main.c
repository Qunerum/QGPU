// #define QGPU_COLORS
#define QGPU_SHAPES
#include "../lib/qgpu.h"

void init() {
	qgSetBackground(0.05f, 0.05f, 0.08f);
}

void update() {
	qgSetCamera((Vector3){2, 4, 7}, (Vector3){0, 0, 0}, 60.0f);

	qgAddLight((Vector3){2, 3, 1}, 50.0f, 1.0f);

	static float spd = .25f;
	if (qgGetKey(QKEY_UP)) spd += 0.01f;
	if (qgGetKey(QKEY_DOWN)) spd -= 0.01f;
	static float r = 0;
	r += spd;

	qgSetLayerUI(1);
	qgAddText((Vector2){-585, 20}, "QGPU\n2.3.0");

	qgSetLayerUI(0);
	qgAddRect((Vector2){-540, 0}, (Vector2){100, 50}, .5f,.2f,.2f,1);

	qgAddCircle((Vector2){-540, -100}, 50, 16, .2f,.5f,.2f,1);

	qgSetRotation(30, r, 0);
	qgSetRotationPivot(0, 1, 0);
	qgAddBox((Vector3){0, 1, 0}, (Vector3){1, 1, 1}, .8f,.6f,.6f,1);

	qgSetRotation(0, 0, 0);
	qgAddPlane((Vector3){0, 0, 0}, (Vector2){10, 10}, .6f,.6f,.6f,1);
}

int main() { qgpuCreate(1280, 720, "QGPU 2.3.0", init, update); return 0; }
