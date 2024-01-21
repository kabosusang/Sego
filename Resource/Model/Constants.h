#pragma once
#include <glm/glm.hpp>
#include "Lights/LightConstans.h"

//PassConstants 一些常量定义，包含摄像机相关的View，Projection矩阵，灯光相关属性
#define MAX_LIGHT_COUNT 16
struct PassConstants
{
//Model
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;

//Material
glm::vec4 g_AmbientLight = glm::vec4(1.0f); //全局环境光

//Cmera
float g_NearZ = 0.0f;
float g_FarZ = 0.0f;
float g_TotalTime = 0.0f;
float g_DeltaTime = 0.0f;

//Lights
LightConstans g_MainLight;//主光源
LightConstans g_AdditionalLights[MAX_LIGHT_COUNT];//额外光源

};



//MaterialConstants
struct MaterialConstants {
    glm::vec4 factorAmbient;    //环境光系数
    glm::vec4 factorDiffuse;    //漫反射系数
    glm::vec4 factorSpecular;   //高光光系数
    
    float shininess;        //高光power值
    float alpha;
    float lighting;
    float reserve0;
};












