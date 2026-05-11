#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include "../include/qgpu.h"
#include "../include/qgpu_core.h"
#include "../include/qgpu_texture.h"

#define PI 3.14159265359f

float q_abs(float x) { return (x < 0) ? -x : x; }
float q_sqrt(float x) {
    if (x <= 0) return 0;
    float xhalf = 0.5f * x;
    union {
        float f;
        int i;
    } conv;
    conv.f = x;
    conv.i = 0x5f3759df - (conv.i >> 1);
    x = conv.f;
    x = x * (1.5f - xhalf * x * x);
    return 1.0f / x;
}
float q_sin(float x) {
    while (x > PI) x -= 2.0f * PI;
    while (x < -PI) x += 2.0f * PI;
    float abs_x = q_abs(x);
    float pi2 = PI * PI;
    float sin_x = (16.0f * x * (PI - abs_x)) / (5.0f * pi2 - 4.0f * x * (PI - abs_x));
    return sin_x;
}
float q_cos(float x) { return q_sin(x + (PI / 2.0f)); }

void print(const char* format, ...) {
    printf("[QGPU]: ");
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    printf("\n");
}

void qgpuCreate(int width, int height, const char* title, void (*updateFunc)())
{ 
    // load_texture("test.qgt", 0);
    if (qgpu_init(width, height, title)) { qgpu_run(updateFunc); } 
    qgpu_cleanup(); 
    cleanup_textures(); }
void drawGeometry(float posX, float posY, QGPU_Vertex* vertices, uint32_t vCount, uint32_t* indices, uint32_t iCount) { qgpu_draw_geo(vertices, vCount, indices, iCount, posX, -posY); }

void drawRect(float posX, float posY, float sizeX, float sizeY, QColor clr) {
    float r = clr.r, g = clr.g, b = clr.b, a = clr.a;
    float x = sizeX / 2, y = sizeY / 2;
    QGPU_Vertex v[] = {
        {{ -x ,  y}, {r, g, b, a}},
        {{  x ,  y}, {r, g, b, a}},
        {{  x , -y}, {r, g, b, a}},
        {{ -x , -y}, {r, g, b, a}}
    };
    uint32_t i[] = {0, 1, 2,  0, 2, 3};
    drawGeometry(posX, posY, v, 4, i, 6);
}
void drawTriangle(float posX, float posY, float p1X, float p1Y, float p2X, float p2Y, float p3X, float p3Y, QColor clr) {
    float r = clr.r, g = clr.g, b = clr.b, a = clr.a;
    QGPU_Vertex v[] = {
        {{ p1X, p1Y }, {r, g, b, a}},
        {{ p2X, p2Y }, {r, g, b, a}},
        {{ p3X, p3Y }, {r, g, b, a}}
    };
    uint32_t i[] = {0, 1, 2};
    drawGeometry(posX, posY, v, 3, i, 3);
}
void drawCircle(float posX, float posY, float radius, int segments, QColor clr) {
    float r = clr.r, g = clr.g, b = clr.b, a = clr.a;
    int vertexCount = segments + 1;
    QGPU_Vertex* v = malloc(sizeof(QGPU_Vertex) * vertexCount);
    v[0].pos[0] = 0.0f;
    v[0].pos[1] = 0.0f;
    v[0].color[0] = r;
    v[0].color[1] = g;
    v[0].color[2] = b;
    v[0].color[3] = a;

    for (int i = 0; i < segments; i++) {
        float angle = (float)i / (float)segments * 2.0f * PI;
        v[i + 1].pos[0] = q_cos(angle) * radius;
        v[i + 1].pos[1] = q_sin(angle) * radius;
        v[i + 1].color[0] = r;
        v[i + 1].color[1] = g;
        v[i + 1].color[2] = b;
        v[i + 1].color[3] = a;
    }
    int indexCount = segments * 3;
    uint32_t* indices = malloc(sizeof(uint32_t) * indexCount);

    for (int i = 0; i < segments; i++) {
        indices[i * 3 + 0] = 0;
        indices[i * 3 + 1] = i + 1;
        indices[i * 3 + 2] = (i == segments - 1) ? 1 : i + 2;
    }
    drawGeometry(posX, posY, v, vertexCount, indices, indexCount);
    free(v);
    free(indices);
}

void drawLine(float x1, float y1, float x2, float y2, float thickness, QColor clr) {
    float r = clr.r, g = clr.g, b = clr.b, a = clr.a;
    float dx = x2 - x1,
    dy = y2 - y1,
    length = q_sqrt(dx * dx + dy * dy);
    if (length <= 0.0f) return;
    float ux = dx / length,
    uy = dy / length,
    nx = -uy * (thickness / 2.0f),
    ny = ux * (thickness / 2.0f),
    midX = (x1 + x2) / 2.0f,
    midY = (y1 + y2) / 2.0f,
    hdx = dx / 2.0f,
    hdy = dy / 2.0f;
    QGPU_Vertex v[] = {
        {{-hdx + nx, -hdy + ny}, {r, g, b, a}},
        {{ hdx + nx,  hdy + ny}, {r, g, b, a}},
        {{ hdx - nx,  hdy - ny}, {r, g, b, a}},
        {{-hdx - nx, -hdy - ny}, {r, g, b, a}}
    };
    uint32_t i[] = {0, 1, 2, 0, 2, 3};
    drawGeometry(midX, midY, v, 4, i, 6);
}
void drawWireRect(float posX, float posY, float sizeX, float sizeY, float thickness, QColor clr) {
    float x = sizeX / 2, y = sizeY / 2,
    x1 = posX - x, y1 = posY + y,
    x2 = posX + x, y2 = posY + y,
    x3 = posX + x, y3 = posY - y,
    x4 = posX - x, y4 = posY - y;
    drawLine(x1, y1, x2, y2, thickness, clr);
    drawLine(x2, y2, x3, y3, thickness, clr);
    drawLine(x3, y3, x4, y4, thickness, clr);
    drawLine(x4, y4, x1, y1, thickness, clr);
}
void drawWireTriangle(float posX, float posY, float p1X, float p1Y, float p2X, float p2Y, float p3X, float p3Y, float thickness, QColor clr) {
    float r = clr.r, g = clr.g, b = clr.b, a = clr.a;
    QGPU_Vertex v[12];
    uint32_t indices[18];
    float pointsX[4] = {p1X, p2X, p3X, p1X};
    float pointsY[4] = {p1Y, p2Y, p3Y, p1Y};
    for (int i = 0; i < 3; i++) {
        float x1 = pointsX[i], y1 = pointsY[i],
        x2 = pointsX[i+1], y2 = pointsY[i+1],
        dx = x2 - x1, dy = y2 - y1,
        len = q_sqrt(dx * dx + dy * dy),
        nx = -dy / len * (thickness / 2.0f),
        ny =  dx / len * (thickness / 2.0f);
        int vo = i * 4, io = i * 6;
        v[vo + 0] = (QGPU_Vertex){{x1 + nx, y1 + ny}, {r, g, b, a}};
        v[vo + 1] = (QGPU_Vertex){{x2 + nx, y2 + ny}, {r, g, b, a}};
        v[vo + 2] = (QGPU_Vertex){{x2 - nx, y2 - ny}, {r, g, b, a}};
        v[vo + 3] = (QGPU_Vertex){{x1 - nx, y1 - ny}, {r, g, b, a}};
        indices[io+0] = vo+0; indices[io+1] = vo+1; indices[io+2] = vo+2;
        indices[io+3] = vo+0; indices[io+4] = vo+2; indices[io+5] = vo+3;
    }
    drawGeometry(posX, posY, v, 12, indices, 18);
}
void drawWireCircle(float posX, float posY, float radius, int segments, float thickness, QColor clr) {
    float r = clr.r, g = clr.g, b = clr.b, a = clr.a;
    if (segments < 3) segments = 3;
    int vCount = segments * 4,
    iCount = segments * 6;
    QGPU_Vertex* v = malloc(sizeof(QGPU_Vertex) * vCount);
    uint32_t* indices = malloc(sizeof(uint32_t) * iCount);
    for (int i = 0; i < segments; i++) {
        float a1 = (float)i / (float)segments * 2.0f * PI,
        a2 = (float)(i + 1) / (float)segments * 2.0f * PI,
        x1 = q_cos(a1) * radius,
        y1 = q_sin(a1) * radius,
        x2 = q_cos(a2) * radius,
        y2 = q_sin(a2) * radius,
        dx = x2 - x1,
        dy = y2 - y1,
        len = q_sqrt(dx * dx + dy * dy),
        nx = -dy / len * (thickness / 2.0f),
        ny =  dx / len * (thickness / 2.0f);
        int vo = i * 4, io = i * 6;
        v[vo + 0] = (QGPU_Vertex){{x1 + nx, y1 + ny}, {r, g, b, a}};
        v[vo + 1] = (QGPU_Vertex){{x2 + nx, y2 + ny}, {r, g, b, a}};
        v[vo + 2] = (QGPU_Vertex){{x2 - nx, y2 - ny}, {r, g, b, a}};
        v[vo + 3] = (QGPU_Vertex){{x1 - nx, y1 - ny}, {r, g, b, a}};
        indices[io + 0] = vo + 0; indices[io + 1] = vo + 1; indices[io + 2] = vo + 2;
        indices[io + 3] = vo + 0; indices[io + 4] = vo + 2; indices[io + 5] = vo + 3;
    }
    drawGeometry(posX, posY, v, vCount, indices, iCount);

    free(v);
    free(indices);
}

void drawTextureScale(float posX, float posY, int textureID, float scale) { drawTextureScaling(textureID, scale, posX, posY); }

int getKey(int key) { return isKeyDown(key); }
int onKey(int key) { return isKeyPressed(key); }

int getMouse(int button) { return isMouseButtonDown(button); }
int onMouse(int button) { return isMouseButtonPressed(button); }

void getMousePos(double* x, double* y) { getCursorPosition(x, y); }

int getWidth() { return qgpu_get_width(); }
int getHeight() { return qgpu_get_height(); }

int AABB(float x, float y, float posX, float posY, float width, float height) { return  (posX - width / 2 <= x && x <= posX + width / 2) && (posY - height / 2 <= y && y <= posY + height / 2); }
