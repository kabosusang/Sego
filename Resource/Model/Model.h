#pragma once

#include <vector>
#include <optional>
#include <string>

#include "Texture/Texture2D/Texture2d.h"
#include "Vertex/Vertex.h"
#include "Core/Material/Material.h"




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




