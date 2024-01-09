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

/**-----------------------DATA---------------------------- 
*  SG_Texture
*  SG_Models
*/

class SG_Texture
{
 
public:
    std::string Texture_Path;
    std::string Texture_Name;
    Texture2D Texture_Attribute;

    //Texture
    uint32_t mipLevels;//mipmaplevel
    uint32_t layerCount; //用于天空盒
    
    VkImage textureImage;
    VkDescriptorImageInfo descriptor;
    VkDeviceMemory textureImageMemory;
    //纹理图像视图
    VkImageView textureImageView;
    VkSampler textureSampler;//采样器


public:
SG_Texture(std::string texturePath,std::string textureName):Texture_Path(texturePath),Texture_Name(textureName){}
SG_Texture() {}
~SG_Texture() {}

};
//TextureImage
//加载模型

class SG_Model
{
public:
SG_Model(std::string path,std::string name);
public:
    std::string Model_Path;
    std::string Model_Name;
//Texutre
    std::vector<SG_Texture> m_Texture;
//Data
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    Modeltype model_type = Modeltype::obj;
//Material
    Material Model_Material;
//Vulkan Vertex Data
    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;
    VkBuffer indexBuffer;
    VkDeviceMemory indexBufferMemory;
//Uniform 
    std::vector<VkBuffer> Obj_uniformBuffers_;
    std::vector<VkDeviceMemory> Obj_uniformBuffersMemory_;
    std::vector<void*> Obj_uniformBuffersMapped_;
//DescriptorSets
    std::vector<VkDescriptorSet> Obj_DescriptorSets_;
//state
    VkBool32 cfg_isBlend_;
    VkBool32 cfg_isDepthTest_;
};
