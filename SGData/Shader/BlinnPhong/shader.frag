#version 450

layout(binding = 1) uniform sampler2D texSampler;
layout(binding = 2) uniform PhoneBlock {
    vec3 viewpos;
    vec3 diffuse;
    vec3 specular;    
    float shininess;

    float lightambient; //default 0.1
    float lightspecular;//default 0.5

    float constant;
    float linear;
    float quadratic;
}Phoneconstans;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in vec3 Normal_;
layout(location = 3) in vec3 FragPos;

vec3 LightPos = vec3(1.0f,5.0f,0.0f);
vec3 lightColor = vec3(1.0f);

layout(location = 0) out vec4 outColor;
void main() {
    
    //ambient
    vec3 ambient = Phoneconstans.lightambient * texture(texSampler, fragTexCoord).rgb;

    //diffuse
    vec3 norm = normalize(Normal_);
    vec3 lightDir = normalize(LightPos - FragPos);
    float diff = max(dot(norm,lightDir),0.0);
    vec3 diffuse = (diff * Phoneconstans.diffuse)* texture(texSampler, fragTexCoord).rgb;

    //specular
    vec3 viewDir = normalize(Phoneconstans.viewpos - FragPos);
    vec3 halwayDir = normalize(lightDir + viewDir);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(norm, halwayDir), 0.0), Phoneconstans.shininess);
    vec3 specular = Phoneconstans.lightspecular * spec * Phoneconstans.specular * texture(texSampler, fragTexCoord).rgb;  



     // attenuation
    float distance    = length(LightPos - FragPos);
    float attenuation = 1.0 / (Phoneconstans.constant + Phoneconstans.linear 
    * distance + Phoneconstans.quadratic * (distance * distance));    

    //Result
    ambient  *= attenuation; 
    diffuse  *= attenuation;
    specular *= attenuation;

    //Result
    vec3 result = (ambient + diffuse + specular);

    outColor =vec4(result,1.0);
}