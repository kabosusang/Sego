#version 450



layout(binding = 1) uniform sampler2D texSampler;

layout(binding = 2) uniform LightConstants {
    vec4 lightCommon; // x:type y:enable(1 or 0); z:0-11; w:spotPower
    vec3 position;//directional/point/spot
    float falloffStart; //point/spot light only
    vec3 direction; //directional/spot light only
    float falloffEnd; //point/spot light only
    vec4 ambient; 
    vec4 diffuse;
    vec4 specular;
}lightCB;

layout(binding = 3) uniform MaterialConstants {
    vec4 factorAmbient;    //环境光系数
    vec4 factorDiffuse;    //漫反射系数
    vec4 factorSpecular;   //高光光系数
    
    float shininess;        //高光power值
    float alpha;
    float lighting;
    float reserve0;
}matCB;


vec3 calculate_Light_Ambient(vec3 ambientGlobal,vec3 ambientMaterial,vec3 ambientLight)
{
     return ambientGlobal * ambientMaterial * ambientLight;
     //环境光计算：PassConstants拿到全局环境光 * 物体材质环境光系数 * 灯光的环境光
}

vec3 calculate_Light_Diffuse_Lambert(vec3 diffuseMaterial,
                                   vec3 diffuseLight,
                                   vec3 L,
                                   vec3 N )
{
     return diffuseMaterial * diffuseLight * max(dot(N,L),0);
     //Lambert漫反射光计算：材质漫反射系数 * 灯光漫反射光 * max(dot(N,L),0)
}

vec3 calculate_Specular_Phone(vec3 specularMaterial,
                              vec3 specularLight,
                              float shininess,
                              vec3 posWorld,
                              vec3 posEye,
                              vec3 L,
                              vec3 N)
{
     vec3 V = normalize(posEye - posWorld);
     vec3 R = normalize(2 * max(dot(N,L),0) * N - L);
     return specularMaterial * specularLight * pow(max(dot(V,R),0),shininess);
}

vec3 calculate_Specular_BlinnPhong(vec3 specularMaterial, 
                                     vec3 specularLight,
                                     float shininess,
                                     vec3 posWorld,
                                     vec3 posEye,
                                     vec3 L,
                                     vec3 N)
{
    vec3 V = normalize(posEye - posWorld);
    vec3 H = normalize(L + V);

    return specularMaterial * specularLight * pow(max(dot(N, H), 0), shininess);
}

vec3 calculate_Light(vec3 ambientGlobal,
                       vec3 posWorld,
                       vec3 posEye,
                       vec3 N)
{
    if (lightCB.lightCommon.z == 0 || matCB.lighting != 1)
        return vec3(1, 1, 1);
    
    vec3 L;
    if (lightCB.lightCommon.x == 0)
    {
        L = - lightCB.direction;
    }
    else
    {
        vec3 posLight = lightCB.position;
        L = normalize(posLight - posWorld);
    }

    //Ambient
    vec3 colorAmbient = vec3(0,0,0); 
    if (lightCB.lightCommon.z == 1 ||
        lightCB.lightCommon.z == 5 ||
        lightCB.lightCommon.z == 6 ||
        lightCB.lightCommon.z == 7 ||
        lightCB.lightCommon.z == 10 ||
        lightCB.lightCommon.z == 11)
    {
        colorAmbient = calculate_Light_Ambient(ambientGlobal,
                                               matCB.factorAmbient.rgb,
                                               lightCB.ambient.rgb);
    }
     //Diffuse
    vec3 colorDiffuse = vec3(0,0,0); 
    if (lightCB.lightCommon.z == 2 ||
        lightCB.lightCommon.z == 5 ||
        lightCB.lightCommon.z == 8 ||
        lightCB.lightCommon.z == 9 ||
        lightCB.lightCommon.z == 10 ||
        lightCB.lightCommon.z == 11)
    {
        colorDiffuse = calculate_Light_Diffuse_Lambert(matCB.factorDiffuse.rgb,
                                                       lightCB.diffuse.rgb,
                                                       L,
                                                       N);
    }
    
    //Specular
    vec3 colorSpecular = vec3(0,0,0);
    if (lightCB.lightCommon.z == 3 ||
        lightCB.lightCommon.z == 6 ||
        lightCB.lightCommon.z == 8 ||
        lightCB.lightCommon.z == 10)
    {
        //Phong
        colorSpecular = calculate_Specular_Phone(matCB.factorSpecular.rgb, 
                                                 lightCB.specular.rgb,
                                                 matCB.shininess,
                                                 posWorld,
                                                 posEye,
                                                 L,
                                                 N);
    }
    else if (lightCB.lightCommon.z == 4 ||
             lightCB.lightCommon.z == 7 ||
             lightCB.lightCommon.z == 9 ||
             lightCB.lightCommon.z == 11)
    {
        //BlinnPhong
        colorSpecular = calculate_Specular_BlinnPhong(matCB.factorSpecular.rgb,
                                                      lightCB.specular.rgb,
                                                      matCB.shininess,
                                                      posWorld,
                                                      posEye,
                                                      L,
                                                      N);
    }
    
    return colorAmbient + colorDiffuse + colorSpecular;
}

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
}Lightcons;




layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in vec3 Mnormal;
layout(location = 3) in vec3 Mposition;


layout(location = 0) out vec4 outColor;


void main() 
{
    vec3 out_color;
 
    vec3 N = normalize(Mnormal);
    //Main Light
    vec3 colorMainLight = calculate_Light(Lightcons.g_AmbientLight.rgb,
                                            Mposition,
                                            cam.posEyeWorld, 
                                            N);
    colorLight = colorMainLight;

    //Additional Light



    //Texture
    vec3 colorTexture = texture2D.Sample(texture2DSampler, input.inTexCoord).rgb;
    //VertexColor
    vec3 colorVertex = input.inColor.rgb;

    //Final Color
    outColor = colorLight * colorTexture * colorVertex;

    return float4(outColor, 1.0);

     vec4 textureColor = texture(texSampler, fragTexCoord);
     outColor = vec4(textureColor.rgb, 0.5);
}