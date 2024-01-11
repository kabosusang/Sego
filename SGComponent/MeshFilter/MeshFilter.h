#pragma once
#include <string>

#include "Vertex/MeshVertex.h"
#include "Vertex/MeshData.h"
#include "SGComponent/component.h"

class MeshFilter : public Component
{
public:
    
    void LoadMesh(std::string mesh_file_path);
    MeshData* mesh(){return mesh_;};
    ~MeshFilter();

private:
MeshData* mesh_ ;
};
