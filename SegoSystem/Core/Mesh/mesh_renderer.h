#pragma once
#include "Material/Material.h"
#include "Core/Mesh/Mesh.h"


class MeshRenderer
{
    void SetMaterial(Material* material);
    void SetMeshFilter(MeshFilter* mesh_filter);

};