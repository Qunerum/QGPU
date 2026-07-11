#version 450

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec4 inColor;

layout(location = 0) out vec2 fragUV;
layout(location = 1) out vec4 fragColor;
layout(location = 2) out flat uint outRenderType;

layout(push_constant) uniform Push {
    vec2 offset;
    vec2 screenRes;
    uint renderType;
} push;

void main() {
    vec2 finalPos = (inPos.xy + push.offset) / (push.screenRes * 0.5);

    gl_Position = vec4(finalPos.x, -finalPos.y, inPos.z, 1.0);

    fragUV = inColor.xy;
    fragColor = inColor;
    outRenderType = push.renderType;
}
