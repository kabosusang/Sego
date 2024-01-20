#version 450
struct LightConstans
{
    vec4 commoon; // x:type y:enable(1 or 0); z:0-11; w:spotPower
    vec3 position;//directional/point/spot
    float falloffStart; //point/spot light only
    vec3 direction; //directional/spot light only
    float falloffEnd; //point/spot light only
    vec4 ambient; 
    vec4 diffuse;
    vec4 specular;
};


#define MAX_LIGHT_COUNT 16
layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
//Material
vec4 g_AmbientLight; //全局环境光

//Lights
LightConstans g_MainLight;//主光源
LightConstans g_AdditionalLights[MAX_LIGHT_COUNT];//额外光源
} ubo;


layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec3 tangent;
layout(location = 4) in vec2 inTexCoord;


layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;
layout(location = 2) out vec3 Mnormal;
layout(location = 3) out vec3 Mposition;



float inflateFactor = 0.05;
void main() {
    gl_Position = ubo.proj * ubo.view * ubo.model * vec4(inPosition, 1.0);
    fragColor = inColor;
    fragTexCoord = inTexCoord;
    Mnormal = normal;
    Mposition = inPosition;
}