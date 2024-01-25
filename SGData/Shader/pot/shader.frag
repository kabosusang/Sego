#version 450

layout(binding = 1) uniform sampler2D texSampler;
layout(binding = 2) uniform ViewPosBlock {
    vec3 viewpos;
}viewconstans;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in vec3 Normal_;
layout(location = 3) in vec3 FragPos;


vec3 LightPos = vec3(1.0f,5.0f,0.0f);
vec3 lightColor = vec3(1.0f);
float ambientStrength = 0.1;//环境光照常量


layout(location = 0) out vec4 outColor;
void main() {
    //ambient
    vec3 ambient = ambientStrength * lightColor;


    //diffuse
    vec3 norm = normalize(Normal_);
    vec3 lightDir = normalize(LightPos - FragPos);
    float diff = max(dot(norm,lightDir),0.0);
    vec3 diffuse = diff * lightColor; 

    //specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewconstans.viewpos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;  


    //Result
    vec4 objectColor = texture(texSampler, fragTexCoord);
    vec3 result = (ambient + diffuse + specular) * objectColor.rgb;

    outColor =vec4(result,1.0f);
}