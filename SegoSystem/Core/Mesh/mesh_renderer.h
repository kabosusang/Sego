#pragma once
#include "Core/Material/Material.h"
#include "Core/Mesh/Mesh.h"

#include "SGComponent/component.h"
class MeshRenderer : public Component
{
public:
    void SetMaterial(Material* material);
    void SetMeshFilter(MeshFilter* mesh_filter);
    void Render();
private:
    Material* material_;
    MeshFilter* mesh_filter_;
    glm::mat4 model_;
};