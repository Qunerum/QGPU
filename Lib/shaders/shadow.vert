#version 450

layout(location = 0) in vec3 inPos;

layout(set = 0, binding = 0) uniform CameraUBO {
    mat4 viewProj;
    mat4 lightViewProj;
    vec4 lightPosRange;
    vec4 lightPowerShadow;
    vec4 sceneInfo;
} cam;

void main() {
    gl_Position = cam.lightViewProj * vec4(inPos, 1.0);
}
