#ifndef QGPU_CORE_H
#define QGPU_CORE_H

#include <vulkan/vulkan.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <stdint.h>

typedef struct {
    float pos[2];
    float color[4];
} QGPU_Vertex;

int  qgpu_init(int width, int height, const char* title);
void qgpu_run(void (*updateFunc)());
void qgpu_draw_geo(QGPU_Vertex* vertices, uint32_t vCount, uint32_t* indices, uint32_t iCount, float offsetX, float offsetY);
void qgpu_cleanup();

void qgpu_get_window_size(int* width, int* height);
void qgpu_get_framebuffer_size(int* width, int* height);

int isKeyDown(int key);
void getCursorPosition(double* x, double* y);
int isMouseButtonDown(int button);

int qgpu_get_width();
int qgpu_get_height();

#endif
