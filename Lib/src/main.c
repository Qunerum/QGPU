// #define QGPU_COLORS
#define QGPU_SHAPES
#define QGPU_UI
#include "../lib/qgpu.h"

void init() {
	qgSetBackground(CLR_RGB(0.05f, 0.05f, 0.08f));
	qgSetFontData(20.0f, QGPU_FONT_STYLE_BOLD, CLR_RGBA(1.0f,1.0f,1.0f,1.0f));
}

void update() {
	static float spd = .25f;
	if (qgGetKey(QKEY_UP)) spd += 0.01f;
	if (qgGetKey(QKEY_DOWN)) spd -= 0.01f;

	qgSetCamera((Vector3){2.0f, 4.0f, 7.0f}, (Vector3){0.0f, 0.0f, 0.0f}, 60.0f);

	qgAddPointLight((Vector3){2.0f, 1.0f, 1.0f}, 50.0f, 1.0f, QGPU_RGB_WHITE);
	qgAddSpotLight((Vector3){3.0f, 2.0f, 1.0f}, (Vector3){-1.0f, 0.0f, 0.0f}, 30.0f, 20.0f, 1.0f, QGPU_RGB_WHITE);

	static float r = 0.0f, px = 0.0f, py = 0.0f;
	r += spd;
	if (qgGetKey(QKEY_W)) py += spd * 10.0f;
	if (qgGetKey(QKEY_S)) py -= spd * 10.0f;
	if (qgGetKey(QKEY_A)) px -= spd * 10.0f;
	if (qgGetKey(QKEY_D)) px += spd * 10.0f;

	qgPushClip((Vector2){px, py}, (Vector2){50.0f, 50.0f});

	qgAddRect((Vector2){px, py}, (Vector2){50.0f, 50.0f}, (ColorRGBA){1.0f,1.0f,1.0f,0.1f});

	qgSetLayerUI(1);
	qgAddText((Vector2){-585.0f, 20.0f}, " QGPU\n" QGPU_VERSION_TEXT);

	qgSetLayerUI(0);
	qgAddRect((Vector2){-540.0f, 0.0f}, (Vector2){100.0f, 50.0f}, CLR_RGBA(.5f,.2f,.2f,1.0f));

	qgAddCircle((Vector2){-540.0f, -100.0f}, 50.0f, 16.0f, CLR_RGBA(.2f,.5f,.2f,1.0f));

	qgPopClip();

	static QgButtonAction btn = 0;
	qgAddButton(&btn, (Vector2){0.0f, -200.0f}, (Vector2){100.0f, 60.0f}, CLR_RGBA(1.0f,1.0f,1.0f,1.0f));
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
	qgAddToggle(&v, (Vector2){200.0f, -100.0f}, (Vector2){50.0f, 50.0f}, CLR_RGBA(1.0f,1.0f,1.0f,1.0f), CLR_RGBA(0.0f,1.0f,0.0f,1.0f));

	static float sv = 0.0f;
	if (qgGetKey(QKEY_LEFT) && sv > 0.0f) sv -= 0.1f;
	if (qgGetKey(QKEY_RIGHT) && sv < 10.0f) sv += 0.1f;
	qgAddSlider(0.0f, 10.0f, &sv, (Vector2){0.0f, -100.0f}, (Vector2){200.0f, 20.0f}, (Vector2){10.0f, 30.0f}, CLR_RGBA(0.0f,1.0f,0.0f,1.0f), CLR_RGBA(1.0f,0.0f,0.0f,1.0f), CLR_RGBA(0.0f,0.0f,1.0f,1.0f));

	qgSetRotation((Vector3){30.0f, r, 0.0f});
	qgSetRotationPivot((Vector3){0.0f, 0.5f, 0.0f});
	qgAddBox((Vector3){0.0f, 0.5f, 0.0f}, (Vector3){1.0f, 1.0f, 1.0f}, CLR_RGBA(.8f,.6f,.6f,1.0f));

	const Vector3 p = (Vector3){0.0f, 2.0f, 0.0f};
	qgSetRotationPivot(p);
	qgSetRotation((Vector3){0.0f, -r, 0.0f});
	if (sv < 2.0f) qgAddCone(p, (Vector2){0.5f, 1.5f}, 32.0f, CLR_RGBA(.0f,.0f,1.0f,1.0f));
	else if (2.0f <= sv&&sv < 4.0f) qgAddCylinder(p, (Vector2){0.5f, 1.5f}, 32, CLR_RGBA(.0f,.0f,1.0f,1.0f));
	else if (4.0f <= sv&&sv < 6.0f) qgAddHemisphere(p, 0.5f, 8.0f, 32.0f, CLR_RGBA(.0f,.0f,1.0f,1.0f));
	else if (6.0f <= sv&&sv < 8.0f) qgAddSphere(p, 0.5f, 8.0f, 16.0f, CLR_RGBA(.0f,.0f,1.0f,1.0f));
	else if (8.0f <= sv) qgAddCapsule(p, (Vector2){0.5f, 2.0f}, 8.0f, 16.0f, CLR_RGBA(.0f,.0f,1.0f,1.0f));

	qgSetRotation((Vector3){0.0f, 0.0f, 0.0f});
	qgAddPlane((Vector3){0.0f, 0.0f, 0.0f}, (Vector2){10.0f, 10.0f}, CLR_RGBA(.6f,.6f,.6f,1.0f));

	const int hw = (int)qgGetWidth() / 2, hh = (int)qgGetHeight() / 2;

	qgAddRect((Vector2){hw - 100.0f, -hh + 50.0f}, (Vector2){70.0f, 80.0f}, CLR_RGBA(.5f,.5f,.5f,1.0f));
	qgAddRect((Vector2){hw - 120.0f, -hh + 120.0f}, (Vector2){30.0f, 40.0f}, qgGetMouse(LMB) ? CLR_RGBA(.3f,.3f,.3f,1.0f) : CLR_RGBA(.5f,.5f,.5f,1.0f));
	qgAddRect((Vector2){hw - 80.0f, -hh + 120.0f}, (Vector2){30.0f, 40.0f}, qgGetMouse(RMB) ? CLR_RGBA(.3f,.3f,.3f,1.0f) : CLR_RGBA(.5f,.5f,.5f,1.0f));

	if (qgOnKey(QKEY_SPACE)) qgLogVertices();
}
int main() { return qgpuCreate(1280, 720, "QGPU " QGPU_VERSION_TEXT, init, update); }
