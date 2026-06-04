#version 450

layout(location = 0) in vec2 inPos;
layout(location = 1) in vec4 inColor; // Wykorzystamy inColor.xy jako współrzędne UV!

layout(location = 0) out vec2 fragUV;

layout(push_constant) uniform Push {
    vec2 offset;
    vec2 screenRes;
} push;

void main() {
    vec2 finalPos = (inPos + push.offset) / (push.screenRes * 0.5);
    gl_Position = vec4(finalPos.x, -finalPos.y, 0.0, 1.0);
    // Przekazujemy UV (współrzędne x i y) do shadera fragmentów
    fragUV = inColor.xy;
}
