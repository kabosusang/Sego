#pragma once
#include <string>

#include "Vertex/MeshVertex.h"
#include "Vertex/MeshData.h"
#include "SGComponent/component.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
class MeshFilter : public Component
{
public:
    
    void LoadMesh(std::string mesh_file_path);
    void processMesh(aiMesh* mesh, const aiScene* scene);
    void CreateVulkanData();
    void processNode(aiNode* node, const aiScene* scene);
    MeshData* mesh(){return mesh_;};
    ~MeshFilter();


private:
MeshData* mesh_ ;
public:
//Vulkan Vertex Data
VkBuffer vertexBuffer;
VkDeviceMemory vertexBufferMemory;
VkBuffer indexBuffer;
VkDeviceMemory indexBufferMemory;
public:
//Uniform 
std::vector<VkBuffer> Obj_uniformBuffers_;
std::vector<VkDeviceMemory> Obj_uniformBuffersMemory_;
std::vector<void*> Obj_uniformBuffersMapped_;


};
