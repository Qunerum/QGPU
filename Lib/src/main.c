// #define QGPU_COLORS
#define QGPU_SHAPES
#define QGPU_UI
#include "../lib/qgpu.h"

void init() {
	qgSetBackground(CLR_RGB(0.05f, 0.05f, 0.08f));
	qgSetFontData(20, QGPU_FONT_STYLE_BOLD, CLR_RGBA(1, 1, 1, 1));
}

void update() {
	static float spd = .25f;
	if (qgGetKey(QKEY_UP)) spd += 0.01f;
	if (qgGetKey(QKEY_DOWN)) spd -= 0.01f;

	qgSetCamera((Vector3){2, 4, 7}, (Vector3){0, 0, 0}, 60.0f);

	qgAddLight((Vector3){2, 1, 1}, 50.0f, 1.0f);
	qgAddSpotLight((Vector3){3, 0.5f, 1}, (Vector3){-1, -0.2f, 0}, 30, 20, 1.0f);

	static float r = 0, px = 0, py = 0;
	r += spd;
	if (qgGetKey(QKEY_W)) py += spd * 5;
	if (qgGetKey(QKEY_S)) py -= spd * 5;
	if (qgGetKey(QKEY_A)) px -= spd * 5;
	if (qgGetKey(QKEY_D)) px += spd * 5;

	qgPushClip((Vector2){px, py}, (Vector2){50, 50});
	qgAddRect((Vector2){px, py}, (Vector2){50, 50}, (ColorRGBA){1,1,1,0.1f});

	qgSetLayerUI(1);
	qgAddText((Vector2){-585, 20}, " QGPU\n" QGPU_VERSION_TEXT);

	qgSetLayerUI(0);
	qgAddRect((Vector2){-540, 0}, (Vector2){100, 50}, CLR_RGBA(.5f,.2f,.2f,1));

	qgAddCircle((Vector2){-540, -100}, 50, 16, CLR_RGBA(.2f,.5f,.2f,1));

	qgPopClip();

	static QgButtonAction btn = 0;
	qgAddButton(&btn, (Vector2){0, 0}, (Vector2){100, 60}, CLR_RGBA(1,1,1,1));
	if (btn&QGPU_BTN_Press) qgPrint("Press!\n");
	if (btn&QGPU_BTN_Release) qgPrint("Release!\n");
	if (btn&QGPU_BTN_MouseEnter) qgPrint("Mouse Enter\n");
	if (btn&QGPU_BTN_Hover) {
		static uint8_t x = 32;
		x++;
		if (x >= 32) {
			qgPrint("Hover!\n");
			x = 0;
		}
	}
	if (btn&QGPU_BTN_MouseLeave) qgPrint("Mouse Leave!\n");

	static uint8_t v = 0;
	qgAddToggle(&v, (Vector2){200, 0}, (Vector2){50, 50}, CLR_RGBA(1,1,1,1), CLR_RGBA(0,1,0,1));


	static float sv = 5;
	if (qgGetKey(QKEY_LEFT) && sv > 0.0f) sv -= 0.1f;
	if (qgGetKey(QKEY_RIGHT) && sv < 10.0f) sv += 0.1f;
	qgAddSlider(0, 10, &sv, (Vector2){0, 100}, (Vector2){200, 20}, (Vector2){10, 30}, CLR_RGBA(0,1,0,1), CLR_RGBA(1,0,0,1), CLR_RGBA(0,0,1,1));

	qgSetRotation((Vector3){30, r, 0});
	qgSetRotationPivot((Vector3){0, 1, 0});
	qgAddBox((Vector3){0, 1, 0}, (Vector3){1, 1, 1}, CLR_RGBA(.8f,.6f,.6f,1));

	qgSetRotation((Vector3){0, 0, 0});
	qgAddPlane((Vector3){0, 0, 0}, (Vector2){10, 10}, CLR_RGBA(.6f,.6f,.6f,1));

	const int hw = (int)qgGetWidth() / 2, hh = (int)qgGetHeight() / 2;

	qgAddRect((Vector2){hw - 100, -hh + 50}, (Vector2){70, 80}, CLR_RGBA(.5f,.5f,.5f,1));
	qgAddRect((Vector2){hw - 120, -hh + 120}, (Vector2){30, 40}, qgGetMouse(LMB) ? CLR_RGBA(.3f,.3f,.3f,1) : CLR_RGBA(.5f,.5f,.5f,1));
	qgAddRect((Vector2){hw - 80, -hh + 120}, (Vector2){30, 40}, qgGetMouse(RMB) ? CLR_RGBA(.3f,.3f,.3f,1) : CLR_RGBA(.5f,.5f,.5f,1));
}

int main() { return qgpuCreate(1280, 720, "QGPU " QGPU_VERSION_TEXT, init, update); }
