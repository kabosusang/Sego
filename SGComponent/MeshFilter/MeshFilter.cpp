#include "pch.h"

#include "MeshFilter/MeshFilter.h"


#include <rttr/registration>

using namespace rttr;
RTTR_REGISTRATION//注册反射
{
    registration::class_<MeshFilter>("MeshFilter")
            .constructor<>()(rttr::policy::ctor::as_raw_ptr);
}


void MeshFilter::processMesh(aiMesh* mesh, const aiScene* scene)
{

//遍历场景所有的网格
for(uint32_t i = 0; i < scene->mNumMeshes ; i++)
{
    aiMesh* mesh = scene->mMeshes[i];


#ifdef GLFW_INCLUDE_VULKAN
 
    for (unsigned int j = 0; j < mesh->mNumVertices; j++) {
        mesh_->vertices.push_back({
        mesh->mVertices[j].x,
        mesh->mVertices[j].y,
        mesh->mVertices[j].z,

        mesh->mNormals[j].x,
        mesh->mNormals[j].y,
        mesh->mNormals[j].z,

        mesh->mTangents[j].x,
        mesh->mTangents[j].y,
        mesh->mTangents[j].z,

        mesh->mTextureCoords[0][j].x,
        1.0f - mesh->mTextureCoords[0][j].y
       });}
}
#endif
        // 遍历面数据
    for (unsigned int p = 0; p < mesh->mNumFaces; p++) {
        auto& face = mesh->mFaces[p];

        // 遍历面的索引数据
        for (unsigned int k = 0; k < face.mNumIndices; k++) {
            unsigned int index = face.mIndices[k];

            // 处理索引数据
            mesh_->indices32.push_back(index);
        }
    }

        // 处理材料
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        // 我们假设在着色器中采样器名称有一个约定。每个漫反射纹理都应该被命名
        // 为'texture_diffuseN'，其中N是一个从1到MAX_SAMPLER_NUMBER的连续数字。
        // 这同样适用于其他纹理，如下列表总结:
        // diffuse: texture_diffuseN
        // specular: texture_specularN
        // normal: texture_normalN

        
        
}


void MeshFilter::processNode(aiNode* node, const aiScene* scene)
{

// process each mesh located at the current node处理当前结点的每个网络
   for (unsigned int i = 0; i < node->mNumMeshes; i++)
   {
       // the node object only contains indices to index the actual objects in the scene. 
       // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
       aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
       processMesh(mesh, scene); //将数据存储再mesh_中
   }
   // after we've processed all of the meshes (if any) we then recursively process each of the children nodes处理当前结点的所有子节点
   for (unsigned int i = 0; i < node->mNumChildren; i++)
   {
       processNode(node->mChildren[i], scene);
   }

}

void MeshFilter::LoadMesh(std::string mesh_file_path)
{
//创建新Mesh
mesh_ = new MeshData;
//创建一个Assimp的导入器对象
Assimp::Importer import;

//使用Assimp导入模型文件
const aiScene* scene = import.ReadFile(mesh_file_path,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType);

if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        SG_CORE_ERROR("Load Failed //////////Assimp Models::{0}",import.GetErrorString());
    }

// process ASSIMP's root node recursively
processNode(scene->mRootNode, scene);//处理结点


CreateVulkanData();

}
#include "Renderer/Vulkan/Vk_Create_Resource.h"
#include "Renderer/Vulkan/Vk_Device_Init.h"
#include "VK_Global_Data.h"
#include "MeshFilter.h"

void MeshFilter::CreateVulkanData()
{
SG_CRes::SGvk_Device_Create_VertexBuffer(mesh_->vertices,vertexBuffer,vertexBufferMemory,
g_device,g_physicalDevice,
app_device->commandPool,app_device->presentQueue);

SG_CRes::SGvk_Device_Create_IndexBuffer(mesh_->GetIndices32(),
indexBuffer,indexBufferMemory,
g_device,g_physicalDevice,
app_device->commandPool,app_device->presentQueue);

SG_CRes::SGvk_CreateUniformBuffers(Obj_uniformBuffers_,Obj_uniformBuffersMemory_,Obj_uniformBuffersMapped_);

}

MeshFilter::~MeshFilter()
{

 //清理均匀缓冲区
        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            vkDestroyBuffer(g_device, Obj_uniformBuffers_[i], nullptr);
            vkFreeMemory(g_device, Obj_uniformBuffersMemory_[i], nullptr);
            }

        vkDestroyDescriptorSetLayout(g_device, descriptorSetLayout_, nullptr);



    delete mesh_;
}
