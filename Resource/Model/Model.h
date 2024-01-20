#pragma once

#include <vector>
#include <optional>
#include <string>

#include "Texture/Texture2D/Texture2d.h"
#include "Vertex/Vertex.h"
#include "Core/Material/Material.h"
#include "Lights/LightConstans.h"



struct UniformBufferObject{
    alignas(16) glm::mat4 model;
    alignas(16) glm::mat4 view;
    alignas(16) glm::mat4 proj;
};

enum class Modeltype
{
    obj,
    mesh
};

#define MAX_LIGHT_COUNT 16
struct PassConstants
{
//Model
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;

//Material
glm::vec4 g_AmbientLight = glm::vec4(1.0f); //全局环境光

//Lights
LightConstans g_MainLight;//主光源
LightConstans g_AdditionalLights[MAX_LIGHT_COUNT];//额外光源

};


