#include "qgpu.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <vulkan/vulkan_core.h>

// = = = = = = = = = = ERROR HANDLING = = = = = = = = = =
#define QGPU_ERROR(CODE, FORMAT, ...) { if (CODE) { printf("\033[1;38;5;%imQGPU Error [\033[1;38;5;%im%03i\033[1;38;5;%im] in file '%s' on line \033[1;38;5;%im%i\033[1;38;5;%im:\n —> "FORMAT"\n"RST, \
	RED, LIGHT_RED, CODE, RED, __FILE_NAME__, LIGHT_RED, __LINE__, RED, ##__VA_ARGS__); exit(1); } }
// = = = = = = = = = = = = = = = = = = = = VISUAL / START = = = = = = = = = = = = = = = = = = = =
static int _showBanner = 1, _showWelcome = 1, _showLogs = 1, qgpuClr = MAGENTA;
// ╔ ═ ╗
// ╚ ║ ╝
// = = = QPrint
static int qclamp(int v, int min, int max) { return v < min ? min : v > max ? max : v; }
static int oldClr = 255, actClr = 255, actStyle = 0; // White , Regular text
void setColor(int color) { oldClr = actClr; actClr = qclamp(color, 0, 255); }
void restoreColor() { int x = actClr; actClr = oldClr; oldClr = x; }
void setStyle(int style) { actStyle = qclamp(style, 0, 1); }
void print(const char* format, ...) { printf("\033[%i;38;5;%im", actStyle, actClr); va_list args; va_start(args, format); vprintf(format, args); va_end(args); printf(RST); }
void printc(int color, const char* format, ...) { printf("\033[%i;38;5;%im", actStyle, color); va_list args; va_start(args, format); vprintf(format, args); va_end(args); printf(RST); }
void qlog(const char* format, ...) { if (_showLogs) { printf("\033[%i;38;5;%im", actStyle, actClr); va_list args; va_start(args, format); vprintf(format, args); va_end(args); printf(RST); } }

void qgpuShowBanner(int show) { _showBanner = show; }
void qgpuShowWelcome(int show) { _showWelcome = show; }
void qgpuShowLogs(int show) { _showLogs = show; }
static void printBanner() {
	printc(165, "╔═════╗ ╔═════╗ ╔═════╗ ╔═╗ ╔═╗ \n");
	printc(164, "║ ╔═╗ ║ ║ ╔═══╝ ║ ╔═╗ ║ ║ ║ ║ ║\n");
	printc(163, "║ ║ ║ ║ ║ ║ ╔═╗ ║ ╚═╝ ║ ║ ║ ║ ║\n");
	printc(162, "║ ╚═╝ ║ ║ ╚═╝ ║ ║ ╔═══╝ ║ ╚═╝ ║\n");
	printc(161, "╚═══╗ ║ ╚═════╝ ╚═╝     ╚═════╝  \n");
	printc(160, "    ╚═╝                   \n");
}
static void c(int v) { printf("\033[0;38;5;%im██ ", v); }
static void printColors() {
	int x = GRAY;
	printc(x,"       colors ╗   ╔═══════╗\n");
	printc(x,"  ╔═════════╦═╝   ║ "); c(WHITE); c(BLACK); printc(x,"║\n");
	printc(x,"╔═╩═════════╩═════╩═══════╣\n");
	printc(x,"║ "); c(LIGHT_GRAY); c(LIGHT_RED); c(LIGHT_GREEN); c(LIGHT_YELLOW); c(LIGHT_ORANGE); c(LIGHT_BLUE); c(LIGHT_MAGENTA); c(LIGHT_CYAN); printc(x,"║\n");
	printc(x,"║ "); c(GRAY);       c(RED);       c(GREEN);       c(YELLOW);       c(ORANGE);       c(BLUE);       c(MAGENTA);       c(CYAN);       printc(x,"║\n");
	printc(x,"║ "); c(DARK_GRAY);  c(DARK_RED);  c(DARK_GREEN);  c(DARK_YELLOW);  c(DARK_ORANGE);  c(DARK_BLUE);  c(DARK_MAGENTA);  c(DARK_CYAN);  printc(x,"║\n");
	printc(x,"╚═════════════════════════╝\n");
}
// = = = = = = = = = = = = = = = = = = = = GLFW / VULKAN = = = = = = = = = = = = = = = = = = = =
typedef struct {
	qgpuWindow qwin;
	const char* engineName;

	uint32_t api_version, queueFamily;

	GLFWwindow* window;
	GLFWmonitor* monitor;

	VkAllocationCallbacks* allocator;
	VkInstance instance;
	VkPhysicalDevice physicalDevice;
	VkSurfaceKHR surface;
	VkDevice device;
} qgpuData;
// = = = = = = = = = = CALLBACKS = = = = = = = = = =
void glfwErrCallback(int code, const char* desc) { QGPU_ERROR(code, "GLFW: %s", desc) }
void exitCallback() { glfwTerminate(); }

// = = = = = = = = = = CODE = = = = = = = = = =
qgpuData data;
static void start() {
	setStyle(BOLD);
	if (_showBanner) printBanner();
	if (_showWelcome) {
		printc(ORANGE, "The application was made with the "); printc(qgpuClr, "QGPU"); printc(ORANGE, " library.\n");
		printc(qgpuClr, "  QGPU"); printc(GRAY, " repo "); printc(LIGHT_MAGENTA, "https://github.com/Qunerum/"); printc(qgpuClr, "QGPU\n");
		printColors();
	}
	setStyle(REGULAR);
}
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
static void setupErrorHandling() {
	glfwSetErrorCallback(glfwErrCallback);
	atexit(exitCallback);
}
static void logInfo() {
	uint32_t instApiVer;
	QGPU_ERROR(vkEnumerateInstanceVersion(&instApiVer), "Couldn't enumerate instance version")
	uint32_t apiVerVariant = VK_API_VERSION_VARIANT(instApiVer);
	uint32_t apiVerMajor = VK_API_VERSION_MAJOR(instApiVer);
	uint32_t apiVerMinor = VK_API_VERSION_MINOR(instApiVer);
	uint32_t apiVerPatch = VK_API_VERSION_PATCH(instApiVer);
	qlog("Vulkan API %i.%i.%i.%i\n", apiVerVariant, apiVerMajor, apiVerMinor, apiVerPatch);
	qlog("QLFW %s\n", glfwGetVersionString());
}
static void createInstance() {
	uint32_t reqExtCount;
	const char** reqExts = glfwGetRequiredInstanceExtensions(&reqExtCount);
	VkApplicationInfo applicationInfo = {
		.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
		.pApplicationName = data.qwin.appName,
		.pEngineName = data.engineName,
		.apiVersion = data.api_version
	};
	VkInstanceCreateInfo createInfo = {
		.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
		.pApplicationInfo = &applicationInfo,
		.enabledExtensionCount = reqExtCount,
		.ppEnabledExtensionNames = reqExts
	};
	QGPU_ERROR(vkCreateInstance(&createInfo, data.allocator, &data.instance), "Couldn't create instance")
}
static void createWindow() {
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, data.qwin.resizable);
	if (data.qwin.fullscreen) data.monitor = glfwGetPrimaryMonitor();
	data.window = glfwCreateWindow(data.qwin.width, data.qwin.height, data.qwin.title, data.monitor, NULL);
}
static void selectPhysicalDevice() {
	uint32_t count;
	QGPU_ERROR(vkEnumeratePhysicalDevices(data.instance, &count, NULL), "Couldn't enumerate physical devices count")
	QGPU_ERROR(count == 0, "Couldn't find a Vulkan supported physical device")
	QGPU_ERROR(vkEnumeratePhysicalDevices(data.instance, &(uint32_t){1}, &data.physicalDevice), "Couldn't enumerate physical devices count")
}
static void createSurface() { QGPU_ERROR(glfwCreateWindowSurface(data.instance, data.window, data.allocator, &data.surface), "Couldn't create window surface"); }
static void selectQueneFamily() {
	data.queueFamily = UINT32_MAX;
	uint32_t count;
	vkGetPhysicalDeviceQueueFamilyProperties(data.physicalDevice, &count, NULL);
	VkQueueFamilyProperties* queneFamilies = malloc(count*sizeof(VkQueueFamilyProperties));
	QGPU_ERROR(queneFamilies == NULL, "Couldn't allocate memory")
	vkGetPhysicalDeviceQueueFamilyProperties(data.physicalDevice, &count, queneFamilies);
	for (int i = 0; i < count; i++) {
		VkQueueFamilyProperties props = queneFamilies[i];
		if (props.queueFlags & VK_QUEUE_GRAPHICS_BIT && glfwGetPhysicalDevicePresentationSupport(data.instance, data.physicalDevice, i)) {
			data.queueFamily = i;
			break;
		}
	}
	QGPU_ERROR(data.queueFamily == UINT32_MAX, "Couldn't find a suitable queue family")
	free(queneFamilies);
}
static void createDevice() {
	QGPU_ERROR(vkCreateDevice(data.physicalDevice, &(VkDeviceCreateInfo) {
		.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
		.pQueueCreateInfos = &(VkDeviceQueueCreateInfo) {
			.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
			.queueFamilyIndex = data.queueFamily,
			.queueCount = 1,
			.pQueuePriorities = &(float){1.0}
		},
		.queueCreateInfoCount = 1,
		.enabledExtensionCount = 1,
		.ppEnabledExtensionNames = &(const char*) {VK_KHR_SWAPCHAIN_EXTENSION_NAME}
	}, data.allocator, &data.device), "Couldn't create device")
}
static void getQuene() {

}
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
static void loop() {
	print("Hello\n");
	while (!glfwWindowShouldClose(data.window)) {
		glfwPollEvents();
	}
}
static void cleanup() {
	vkDestroyDevice(data.device, data.allocator);
	vkDestroySurfaceKHR(data.instance, data.surface, data.allocator);
	vkDestroyInstance(data.instance, data.allocator);
	glfwDestroyWindow(data.window);
}
int qgpuInit(const char* title, int width, int height) {
	start();
	data = (qgpuData){
		.qwin = {
			.title = title,
			.width = width,
			.height = height
		},
		.api_version = VK_API_VERSION_1_4
	};
	// = = = > Init
	setupErrorHandling();
	logInfo();
	createWindow();
	createInstance();
	selectPhysicalDevice();
	createSurface();
	selectQueneFamily();
	createDevice();
	getQuene();
	// < = = =
	loop();
	cleanup();
	return EXIT_SUCCESS;
}
