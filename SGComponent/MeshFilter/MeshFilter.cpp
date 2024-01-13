#include "pch.h"

#include "MeshFilter/MeshFilter.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <rttr/registration>

using namespace rttr;
RTTR_REGISTRATION//注册反射
{
    registration::class_<MeshFilter>("MeshFilter")
            .constructor<>()(rttr::policy::ctor::as_raw_ptr);
}

void MeshFilter::LoadMesh(std::string mesh_file_path)
{
//创建一个Assimp的导入器对象
Assimp::Importer import;

//使用Assimp导入模型文件
const aiScene* scene = import.ReadFile(mesh_file_path,aiProcess_Triangulate 
| aiProcess_JoinIdenticalVertices | aiProcess_SortByPType);

if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        SG_CORE_ERROR("Load Failed //////////Assimp Models");
    }


//遍历场景所有的网格
for(uint32_t i = 0; i < scene->mNumMeshes ; i++)
{
    aiMesh* mesh = scene->mMeshes[i];
    
    mesh_ = new MeshData;
    mesh_->Clear();
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
        mesh->mTextureCoords[0][j].y
       });}

        // 遍历面数据
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];

        // 遍历面的索引数据
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            unsigned int index = face.mIndices[j];

            // 处理索引数据
            mesh_->indices32.push_back(index);
        }
    }
}

CreateVulkanData();

}
#include "Renderer/Vulkan/Vk_Create_Resource.h"
#include "Renderer/Vulkan/Vk_Device_Init.h"
#include "VK_Global_Data.h"

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


