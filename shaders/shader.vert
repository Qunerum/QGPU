#version 450

layout(location = 0) in vec2 inPos;
layout(location = 1) in vec3 inColor;

layout(push_constant) uniform Push {
    vec2 offset;    // Pozycja obiektu w pikselach
    vec2 screenRes; // Rozdzielczość okna (width, height)
} push;

layout(location = 0) out vec3 fragColor;

void main() {
    // 1. Przeliczamy pozycję wierzchołka (inPos) oraz offsetu na system Vulkana (-1 do 1)
    // Dzielimy przez push.screenRes * 0.5, aby przejść z pikseli na zakres 0..2, a potem -1.0

    vec2 posInPixels = inPos + push.offset;
    vec2 ndcPos = (posInPixels / (push.screenRes * 0.5));

    // Vulkan ma odwróconą oś Y w stosunku do ekranu, więc dajemy -ndcPos.y
    gl_Position = vec4(ndcPos.x, -ndcPos.y, 0.0, 1.0);
    fragColor = inColor;
}
