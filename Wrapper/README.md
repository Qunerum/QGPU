```c
#define STATUS "STABLE"
#define LICENSE "GNU GPL v3"

typedef struct {
    char* name;
    char* version;
    char* paradigm;
    char* core_language;
    char* about;
    char* features[4];
    char* quick_start[2];
    char* code_example[9];
} Project;

int main() {
    Project qgpu = {
        .name = "QGPU",
        .version = "1.0.0",
        .paradigm = "Immediate Mode UI & 2D Graphics Wrapper",
        .core_language = "Pure C (Vulkan & GLFW)",
        .about = "A lightweight, hardware-accelerated 2D graphics wrapper that hides Vulkan's complexity.",
        .features = {
            "Blazing fast 2D rendering pipeline utilizing Vulkan API",
            "Immediate Mode GUI capabilities with custom bitmap font rendering",
            "Built-in texture loading system (.qgt file support)",
            "Simplified keyboard and mouse input handling (getKey, getMousePos)"
        },
        .quick_start = {
            "Build command: make",
            "Run command:   ./qgpuApp"
        },
        .code_example = {
            "#include \"../lib/qgpu.h\"",
            "void Init() { }",
            "void Update() {",
            "    drawRect(0, 0, 200, 100, RED);",
            "}",
            "int main() {",
            "    qgpuCreate(1280, 720, \"QGPU App\", Init, Update);",
            "    return 0;",
            "}"
        }
    };
    return 0;
}
```
