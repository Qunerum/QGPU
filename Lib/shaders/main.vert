#version 450

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec4 inColor;

layout(location = 0) out vec3 fragWorldPos;
layout(location = 1) out vec3 fragNormal;
layout(location = 2) out vec4 fragColor;
layout(location = 3) out vec4 fragLightSpacePos;

layout(set = 0, binding = 0) uniform CameraUBO {
    mat4 viewProj;
    mat4 lightViewProj;
    vec4 lightPosRange;
    vec4 lightPowerShadow;
    vec4 sceneInfo;
} cam;

void main() {
    fragWorldPos = inPos;
    fragNormal = inNormal;
    fragColor = inColor;
    fragLightSpacePos = cam.lightViewProj * vec4(inPos, 1.0);
    gl_Position = cam.viewProj * vec4(inPos, 1.0);
}
