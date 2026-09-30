// #define QGPU_COLORS
#define QGPU_SHAPES
#include "../lib/qgpu.h"

void init() {
	qgSetBackground(CLR_RGB(0.05f, 0.05f, 0.08f));
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
	qgAddText((Vector2){-585, 20}, "QGPU\n2.3.1");

	qgSetLayerUI(0);
	qgAddRect((Vector2){-540, 0}, (Vector2){100, 50}, CLR_RGBA(.5f,.2f,.2f,1));

	qgAddCircle((Vector2){-540, -100}, 50, 16, CLR_RGBA(.2f,.5f,.2f,1));

	qgSetRotation((Vector3){30, r, 0});
	qgSetRotationPivot((Vector3){0, 1, 0});
	qgAddBox((Vector3){0, 1, 0}, (Vector3){1, 1, 1}, CLR_RGBA(.8f,.6f,.6f,1));

	qgSetRotation((Vector3){0, 0, 0});
	qgAddPlane((Vector3){0, 0, 0}, (Vector2){10, 10}, CLR_RGBA(.6f,.6f,.6f,1));
	qgLogVertices();
}

int main() { return qgpuCreate(1280, 720, "QGPU 2.3.1", init, update); }
