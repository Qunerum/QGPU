#version 450

layout(location = 0) in vec2 inPos;
layout(location = 1) in vec4 inColor;

layout(location = 0) out vec4 fragColor;

layout(push_constant) uniform PC {
    vec2 screenSize;
} pc;

void main() {
    vec2 ndc = inPos / (pc.screenSize * 0.5);
    ndc.y = -ndc.y; // Vulkan NDC has +Y down
    gl_Position = vec4(ndc, 0.0, 1.0);
    fragColor = inColor;
}
