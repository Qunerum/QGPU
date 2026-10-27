#version 450

layout(location = 0) in vec3 fragWorldPos;
layout(location = 1) in vec3 fragNormal;
layout(location = 2) in vec4 fragColor;
layout(location = 3) in vec4 fragLightSpacePos;

layout(location = 0) out vec4 outColor;

layout(set = 0, binding = 0) uniform CameraUBO {
    mat4 viewProj;
    mat4 lightViewProj;
    vec4 lightPosRange;
    vec4 lightPowerShadow;
    vec4 sceneInfo;
} cam;
layout(set = 0, binding = 1) uniform sampler2D shadowMap;

struct GPULight {
    vec4 posRange;
    vec4 dirPower;
    vec4 typeAngle;
    vec3 color;
};
layout(set = 0, binding = 2, std430) readonly buffer LightsBuffer {
    GPULight lights[];
} lightsBuf;

float shadowFactor() {
    if (cam.lightPowerShadow.y < 0.5) return 1.0;
    if (fragLightSpacePos.w <= 0.0) return 1.0;
    vec3 proj = fragLightSpacePos.xyz / fragLightSpacePos.w;
    vec2 uv = proj.xy * 0.5 + 0.5;
    if (uv.x < 0.0 || uv.x > 1.0 || uv.y < 0.0 || uv.y > 1.0 || proj.z > 1.0) return 1.0;
    float bias = 0.0002;
    float closestDepth = texture(shadowMap, uv).r;
    return (proj.z - bias > closestDepth) ? 0.35 : 1.0;
}

void main() {
    vec3 n = normalize(fragNormal);
    vec3 total = vec3(cam.sceneInfo.x);
    int count = int(cam.sceneInfo.y);
    float shadow = shadowFactor();
    for (int i = 0; i < count; i++) {
        GPULight L = lightsBuf.lights[i];
        float power = L.dirPower.w;
        if (power <= 0.0) continue;
        float contrib = 0.0;
        if (L.typeAngle.x > 1.5) {
            contrib = power * max(dot(n, -L.dirPower.xyz), 0.0);
        } else {
            vec3 toLight = L.posRange.xyz - fragWorldPos;
            float range = L.posRange.w;
            float dist = length(toLight);
            if (range <= 0.0 || dist > range) continue;
            vec3 lightDir = toLight / max(dist, 0.0001);
            float atten = 1.0 - dist / range;
            if (L.typeAngle.x > 0.5) {
                float spotCos = dot(-lightDir, L.dirPower.xyz);
                if (spotCos < L.typeAngle.y) continue;
                atten *= clamp((spotCos - L.typeAngle.y) / 0.05, 0.0, 1.0);
            }
            contrib = power * atten * max(dot(n, lightDir), 0.0);
        }
        if (i == 0) contrib *= shadow;
        total += contrib * L.color;
    }
    total = clamp(total, 0.0, 2.0);
    outColor = vec4(fragColor.rgb * total, fragColor.a);
}
