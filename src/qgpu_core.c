#include "../include/qgpu_core.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    GLFWwindow* window;
    VkInstance instance;
    VkSurfaceKHR surface;
    VkPhysicalDevice physicalDevice;
    VkDevice device;
    VkQueue graphicsQueue;

    VkSwapchainKHR swapchain;
    uint32_t imageCount;
    VkImage* swapchainImages;
    VkImageView* swapchainImageViews;

    VkRenderPass renderPass;
    VkPipelineLayout pipelineLayout;
    VkPipeline graphicsPipeline;
    VkCommandPool commandPool;

    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;
    VkBuffer indexBuffer;
    VkDeviceMemory indexBufferMemory;
    uint32_t indexCount;

    VkFramebuffer* swapchainFramebuffers;
    VkSemaphore imageAvailableSemaphore;
    VkSemaphore renderFinishedSemaphore;
    VkCommandBuffer currentCmd;

    uint32_t currentVOffset;
    uint32_t currentIOffset;
} InternalContext;

static InternalContext g_ctx;

static uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(g_ctx.physicalDevice, &memProperties);
    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }
    return 0;
}

static void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer* buffer, VkDeviceMemory* bufferMemory) {
    VkBufferCreateInfo bufferInfo = {
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .size = size,
        .usage = usage,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE
    };
    vkCreateBuffer(g_ctx.device, &bufferInfo, NULL, buffer);

    VkMemoryRequirements memReqs;
    vkGetBufferMemoryRequirements(g_ctx.device, *buffer, &memReqs);

    VkMemoryAllocateInfo allocInfo = {
        .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
        .allocationSize = memReqs.size,
        .memoryTypeIndex = findMemoryType(memReqs.memoryTypeBits, properties)
    };
    vkAllocateMemory(g_ctx.device, &allocInfo, NULL, bufferMemory);
    vkBindBufferMemory(g_ctx.device, *buffer, *bufferMemory, 0);
}

static VkShaderModule createShaderModule(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (!file) return VK_NULL_HANDLE;

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* code = malloc(length);
    fread(code, 1, length, file);
    fclose(file);

    VkShaderModuleCreateInfo createInfo = {
        .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
        .codeSize = length,
        .pCode = (uint32_t*)code
    };

    VkShaderModule shaderModule;
    vkCreateShaderModule(g_ctx.device, &createInfo, NULL, &shaderModule);
    free(code);
    return shaderModule;
}

// --- API ---
int qgpu_init(int width, int height, const char* title) {
    if (!glfwInit()) return 0;
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    g_ctx.window = glfwCreateWindow(width, height, title, NULL, NULL);
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    VkInstanceCreateInfo createInfo = {
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .enabledExtensionCount = glfwExtensionCount,
        .ppEnabledExtensionNames = glfwExtensions
    };
    vkCreateInstance(&createInfo, NULL, &g_ctx.instance);
    glfwCreateWindowSurface(g_ctx.instance, g_ctx.window, NULL, &g_ctx.surface);
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(g_ctx.instance, &deviceCount, NULL);
    VkPhysicalDevice* devices = malloc(sizeof(VkPhysicalDevice) * deviceCount);
    vkEnumeratePhysicalDevices(g_ctx.instance, &deviceCount, devices);
    g_ctx.physicalDevice = devices[0];
    float queuePriority = 1.0f;
    VkDeviceQueueCreateInfo queueCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
        .queueFamilyIndex = 0,
        .queueCount = 1,
        .pQueuePriorities = &queuePriority
    };
    const char* deviceExtensions[] = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
    VkDeviceCreateInfo deviceCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .queueCreateInfoCount = 1,
        .pQueueCreateInfos = &queueCreateInfo,
        .enabledExtensionCount = 1,
        .ppEnabledExtensionNames = deviceExtensions
    };
    vkCreateDevice(g_ctx.physicalDevice, &deviceCreateInfo, NULL, &g_ctx.device);
    vkGetDeviceQueue(g_ctx.device, 0, 0, &g_ctx.graphicsQueue);
    VkSwapchainCreateInfoKHR swapchainInfo = {
        .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .surface = g_ctx.surface,
        .minImageCount = 2,
        .imageFormat = VK_FORMAT_B8G8R8A8_UNORM,
        .imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR,
        .imageExtent = {width, height},
        .imageArrayLayers = 1,
        .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        .imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
        .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        .presentMode = VK_PRESENT_MODE_FIFO_KHR,
        .clipped = VK_TRUE
    };
    vkCreateSwapchainKHR(g_ctx.device, &swapchainInfo, NULL, &g_ctx.swapchain);
    vkGetSwapchainImagesKHR(g_ctx.device, g_ctx.swapchain, &g_ctx.imageCount, NULL);
    g_ctx.swapchainImages = malloc(sizeof(VkImage) * g_ctx.imageCount);
    vkGetSwapchainImagesKHR(g_ctx.device, g_ctx.swapchain, &g_ctx.imageCount, g_ctx.swapchainImages);
    g_ctx.swapchainImageViews = malloc(sizeof(VkImageView) * g_ctx.imageCount);
    for (uint32_t i = 0; i < g_ctx.imageCount; i++) {
        VkImageViewCreateInfo viewInfo = {
            .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            .image = g_ctx.swapchainImages[i],
            .viewType = VK_IMAGE_VIEW_TYPE_2D,
            .format = VK_FORMAT_B8G8R8A8_UNORM,
            .subresourceRange = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1}
        };
        vkCreateImageView(g_ctx.device, &viewInfo, NULL, &g_ctx.swapchainImageViews[i]);
    }
    VkAttachmentDescription colorAttachment = {
        .format = VK_FORMAT_B8G8R8A8_UNORM,
        .samples = VK_SAMPLE_COUNT_1_BIT,
        .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
        .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
        .finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
    };
    VkAttachmentReference colorRef = {0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL};
    VkSubpassDescription subpass = {.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS, .colorAttachmentCount = 1, .pColorAttachments = &colorRef};
    VkRenderPassCreateInfo rpInfo = {.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO, .attachmentCount = 1, .pAttachments = &colorAttachment, .subpassCount = 1, .pSubpasses = &subpass};
    vkCreateRenderPass(g_ctx.device, &rpInfo, NULL, &g_ctx.renderPass);
    VkShaderModule vertModule = createShaderModule("bin/vert.spv");
    VkShaderModule fragModule = createShaderModule("bin/frag.spv");

    VkPipelineShaderStageCreateInfo shaderStages[2] = {
        {.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO, .stage = VK_SHADER_STAGE_VERTEX_BIT, .module = vertModule, .pName = "main"},
        {.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO, .stage = VK_SHADER_STAGE_FRAGMENT_BIT, .module = fragModule, .pName = "main"}
    };
    VkVertexInputBindingDescription bindingDesc = {.binding = 0, .stride = sizeof(QGPU_Vertex), .inputRate = VK_VERTEX_INPUT_RATE_VERTEX};
    VkVertexInputAttributeDescription attrDesc[2] = {
        {.binding = 0, .location = 0, .format = VK_FORMAT_R32G32_SFLOAT, .offset = offsetof(QGPU_Vertex, pos)},
        {.binding = 0, .location = 1, .format = VK_FORMAT_R32G32B32_SFLOAT, .offset = offsetof(QGPU_Vertex, color)}
    };
    VkPipelineVertexInputStateCreateInfo vertexInputInfo = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
        .vertexBindingDescriptionCount = 1, .pVertexBindingDescriptions = &bindingDesc,
        .vertexAttributeDescriptionCount = 2, .pVertexAttributeDescriptions = attrDesc
    };

    VkPipelineInputAssemblyStateCreateInfo inputAssembly = {.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO, .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST};
    VkViewport viewport = {0.0f, 0.0f, (float)width, (float)height, 0.0f, 1.0f};
    VkRect2D scissor = {{0, 0}, {width, height}};
    VkPipelineViewportStateCreateInfo viewportState = {.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO, .viewportCount = 1, .pViewports = &viewport, .scissorCount = 1, .pScissors = &scissor};
    VkPipelineRasterizationStateCreateInfo rasterizer = {.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO, .lineWidth = 1.0f, .cullMode = VK_CULL_MODE_NONE, .frontFace = VK_FRONT_FACE_CLOCKWISE};
    VkPipelineMultisampleStateCreateInfo multisampling = {.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO, .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT};
    VkPipelineColorBlendAttachmentState colorBlendAttachment = {.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT};
    VkPipelineColorBlendStateCreateInfo colorBlending = {.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO, .attachmentCount = 1, .pAttachments = &colorBlendAttachment};

    VkPushConstantRange pushConstantRange = {
        .stageFlags = VK_SHADER_STAGE_VERTEX_BIT,
        .offset = 0,
        .size = sizeof(float) * 7
    };
    VkPipelineLayoutCreateInfo pipelineLayoutInfo = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
        .pushConstantRangeCount = 1,
        .pPushConstantRanges = &pushConstantRange
    };
    vkCreatePipelineLayout(g_ctx.device, &pipelineLayoutInfo, NULL, &g_ctx.pipelineLayout);
    VkGraphicsPipelineCreateInfo pipelineInfo = {
        .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
        .stageCount = 2, .pStages = shaderStages,
        .pVertexInputState = &vertexInputInfo, .pInputAssemblyState = &inputAssembly,
        .pViewportState = &viewportState, .pRasterizationState = &rasterizer,
        .pMultisampleState = &multisampling, .pColorBlendState = &colorBlending,
        .layout = g_ctx.pipelineLayout, .renderPass = g_ctx.renderPass, .subpass = 0
    };
    vkCreateGraphicsPipelines(g_ctx.device, VK_NULL_HANDLE, 1, &pipelineInfo, NULL, &g_ctx.graphicsPipeline);

    vkDestroyShaderModule(g_ctx.device, vertModule, NULL);
    vkDestroyShaderModule(g_ctx.device, fragModule, NULL);
    g_ctx.swapchainFramebuffers = malloc(sizeof(VkFramebuffer) * g_ctx.imageCount);
    for (uint32_t i = 0; i < g_ctx.imageCount; i++) {
        VkFramebufferCreateInfo fbInfo = {.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO, .renderPass = g_ctx.renderPass, .attachmentCount = 1, .pAttachments = &g_ctx.swapchainImageViews[i], .width = width, .height = height, .layers = 1};
        vkCreateFramebuffer(g_ctx.device, &fbInfo, NULL, &g_ctx.swapchainFramebuffers[i]);
    }
    VkCommandPoolCreateInfo poolInfo = {.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO, .queueFamilyIndex = 0, .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT};
    vkCreateCommandPool(g_ctx.device, &poolInfo, NULL, &g_ctx.commandPool);
    VkSemaphoreCreateInfo semInfo = {.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO};
    vkCreateSemaphore(g_ctx.device, &semInfo, NULL, &g_ctx.imageAvailableSemaphore);
    vkCreateSemaphore(g_ctx.device, &semInfo, NULL, &g_ctx.renderFinishedSemaphore);

    createBuffer(1024 * 1024, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &g_ctx.vertexBuffer, &g_ctx.vertexBufferMemory);
    createBuffer(1024 * 1024, VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &g_ctx.indexBuffer, &g_ctx.indexBufferMemory);
    return 1;
}

void qgpu_draw_geo(QGPU_Vertex* vertices, uint32_t vCount, uint32_t* indices, uint32_t iCount, float offsetX, float offsetY) {
    if (g_ctx.device == VK_NULL_HANDLE || g_ctx.currentCmd == VK_NULL_HANDLE) return;

    VkDeviceSize vSize = sizeof(QGPU_Vertex) * vCount;
    VkDeviceSize iSize = sizeof(uint32_t) * iCount;

    void* data;
    vkMapMemory(g_ctx.device, g_ctx.vertexBufferMemory, g_ctx.currentVOffset, vSize, 0, &data);
    memcpy(data, vertices, vSize);
    vkUnmapMemory(g_ctx.device, g_ctx.vertexBufferMemory);

    vkMapMemory(g_ctx.device, g_ctx.indexBufferMemory, g_ctx.currentIOffset, iSize, 0, &data);
    memcpy(data, indices, iSize);
    vkUnmapMemory(g_ctx.device, g_ctx.indexBufferMemory);

    int w, h;
    glfwGetFramebufferSize(g_ctx.window, &w, &h);
    float pushData[4] = { offsetX, offsetY, (float)w, (float)h };
    vkCmdPushConstants(g_ctx.currentCmd, g_ctx.pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(float) * 4, pushData);

    VkDeviceSize vOffsets[] = { g_ctx.currentVOffset };
    vkCmdBindVertexBuffers(g_ctx.currentCmd, 0, 1, &g_ctx.vertexBuffer, vOffsets);
    vkCmdBindIndexBuffer(g_ctx.currentCmd, g_ctx.indexBuffer, g_ctx.currentIOffset, VK_INDEX_TYPE_UINT32);

    vkCmdDrawIndexed(g_ctx.currentCmd, iCount, 1, 0, 0, 0);

    g_ctx.currentVOffset += vSize;
    g_ctx.currentIOffset += iSize;
}

void qgpu_run(void (*updateFunc)()) {
    while (!glfwWindowShouldClose(g_ctx.window)) {
        g_ctx.currentVOffset = 0;
        g_ctx.currentIOffset = 0;
        glfwPollEvents();
        uint32_t imageIndex;
        vkAcquireNextImageKHR(g_ctx.device, g_ctx.swapchain, UINT64_MAX, g_ctx.imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);

        VkCommandBufferAllocateInfo allocInfo = {.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO, .commandPool = g_ctx.commandPool, .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY, .commandBufferCount = 1};
        vkAllocateCommandBuffers(g_ctx.device, &allocInfo, &g_ctx.currentCmd);

        VkCommandBufferBeginInfo beginInfo = {.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO, .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT};
        vkBeginCommandBuffer(g_ctx.currentCmd, &beginInfo);

        VkClearValue clearColor = {{{0.1f, 0.1f, 0.1f, 1.0f}}};
        VkRenderPassBeginInfo rpBegin = {.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO, .renderPass = g_ctx.renderPass, .framebuffer = g_ctx.swapchainFramebuffers[imageIndex], .renderArea = {{0, 0}, {800, 600}}, .clearValueCount = 1, .pClearValues = &clearColor};

        vkCmdBeginRenderPass(g_ctx.currentCmd, &rpBegin, VK_SUBPASS_CONTENTS_INLINE);
        vkCmdBindPipeline(g_ctx.currentCmd, VK_PIPELINE_BIND_POINT_GRAPHICS, g_ctx.graphicsPipeline);

        updateFunc();

        vkCmdEndRenderPass(g_ctx.currentCmd);
        vkEndCommandBuffer(g_ctx.currentCmd);

        VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        VkSubmitInfo submitInfo = {.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO, .waitSemaphoreCount = 1, .pWaitSemaphores = &g_ctx.imageAvailableSemaphore, .pWaitDstStageMask = waitStages, .commandBufferCount = 1, .pCommandBuffers = &g_ctx.currentCmd, .signalSemaphoreCount = 1, .pSignalSemaphores = &g_ctx.renderFinishedSemaphore};
        vkQueueSubmit(g_ctx.graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);

        VkPresentInfoKHR presentInfo = {.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR, .waitSemaphoreCount = 1, .pWaitSemaphores = &g_ctx.renderFinishedSemaphore, .swapchainCount = 1, .pSwapchains = &g_ctx.swapchain, .pImageIndices = &imageIndex};
        vkQueuePresentKHR(g_ctx.graphicsQueue, &presentInfo);

        vkQueueWaitIdle(g_ctx.graphicsQueue);
        vkFreeCommandBuffers(g_ctx.device, g_ctx.commandPool, 1, &g_ctx.currentCmd);
        g_ctx.currentCmd = VK_NULL_HANDLE;
    }
}

void qgpu_cleanup() {
    vkDeviceWaitIdle(g_ctx.device);
    vkDestroyBuffer(g_ctx.device, g_ctx.vertexBuffer, NULL);
    vkFreeMemory(g_ctx.device, g_ctx.vertexBufferMemory, NULL);
    vkDestroyBuffer(g_ctx.device, g_ctx.indexBuffer, NULL);
    vkFreeMemory(g_ctx.device, g_ctx.indexBufferMemory, NULL);
    vkDestroyPipeline(g_ctx.device, g_ctx.graphicsPipeline, NULL);
    vkDestroyPipelineLayout(g_ctx.device, g_ctx.pipelineLayout, NULL);
    vkDestroySemaphore(g_ctx.device, g_ctx.imageAvailableSemaphore, NULL);
    vkDestroySemaphore(g_ctx.device, g_ctx.renderFinishedSemaphore, NULL);
    vkDestroyCommandPool(g_ctx.device, g_ctx.commandPool, NULL);
    for (uint32_t i = 0; i < g_ctx.imageCount; i++) { vkDestroyFramebuffer(g_ctx.device, g_ctx.swapchainFramebuffers[i], NULL); vkDestroyImageView(g_ctx.device, g_ctx.swapchainImageViews[i], NULL); }
    vkDestroyRenderPass(g_ctx.device, g_ctx.renderPass, NULL);
    vkDestroySwapchainKHR(g_ctx.device, g_ctx.swapchain, NULL);
    vkDestroyDevice(g_ctx.device, NULL);
    vkDestroySurfaceKHR(g_ctx.instance, g_ctx.surface, NULL);
    vkDestroyInstance(g_ctx.instance, NULL);
    glfwDestroyWindow(g_ctx.window);
    glfwTerminate();
}
