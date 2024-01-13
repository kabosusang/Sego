#pragma once
#include <string>

#include "Vertex/MeshVertex.h"
#include "Vertex/MeshData.h"
#include "SGComponent/component.h"

class MeshFilter : public Component
{
public:
    
    void LoadMesh(std::string mesh_file_path);
    void CreateVulkanData();

    MeshData* mesh(){return mesh_;};
    ~MeshFilter();

private:
MeshData* mesh_ ;

//Vulkan Vertex Data
VkBuffer vertexBuffer;
VkDeviceMemory vertexBufferMemory;
VkBuffer indexBuffer;
VkDeviceMemory indexBufferMemory;

//Uniform 
std::vector<VkBuffer> Obj_uniformBuffers_;
std::vector<VkDeviceMemory> Obj_uniformBuffersMemory_;
std::vector<void*> Obj_uniformBuffersMapped_;


};
