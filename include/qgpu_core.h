#ifndef QGPU_CORE_H
#define QGPU_CORE_H

#include <vulkan/vulkan.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <stdint.h>

typedef struct {
    float pos[2];
    float color[3];
} QGPU_Vertex;

typedef void (*QGPU_UpdateCallback)();

int  qgpu_init(int width, int height, const char* title);
void qgpu_run(void (*updateFunc)());
void qgpu_draw_geo(QGPU_Vertex* vertices, uint32_t vCount, uint32_t* indices, uint32_t iCount, float offsetX, float offsetY);
void qgpu_cleanup();

#endif
