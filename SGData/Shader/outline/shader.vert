#version 450

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec3 tangent;
layout(location = 4) in vec2 inTexCoord;


layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;

float inflateFactor = 0.05;
void main() {
    vec3 inflatedPosition = inPosition + (normal * inflateFactor); //顶点膨胀
    gl_Position = ubo.proj * ubo.view * ubo.model * vec4(inflatedPosition, 1.0);
    fragColor = inColor;
    fragTexCoord = inTexCoord;
}