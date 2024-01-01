#pragma once

#include <vector>
#include <optional>
#include <string>

#include "Data/Vertex.h"
#include "Core/Material/Material.h"


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
//Vulkan Data
    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;
    VkBuffer indexBuffer;
    VkDeviceMemory indexBufferMemory;

};


namespace SG_CRes
{   
    void CreateTexture(std::vector<SG_Texture>::iterator it,
    VkDevice& device,VkPhysicalDevice& physicalDevice,VkCommandPool& commandPool,
    VkQueue& graQue);

    
    void GenerateMipmaps(VkDevice& device,VkCommandPool& commandPool,VkQueue& graQue,
    VkImage image,VkFormat imageFormat,VkPhysicalDevice& physicalDevice,
     int32_t texWidth, int32_t texHeight, uint32_t mipLevels);


    void CreateTextureView(VkDevice& device,VkImage& textureImage,VkImageView& textureImageView,uint32_t mipLevels);
    VkImageView SGvk_Device_Create_ImageView_AttachFuc(VkDevice& device,
    VkImage& image, VkFormat format,VkImageAspectFlags aspectFlags,uint32_t mipLevels);

    void SGvk_Device_Create_TextureSampler(VkDevice& device,
    VkPhysicalDevice& physicalDevice,VkSampler& textureSampler,uint32_t mipLevels);

    //LoadModel
    void loadModel_tiny_obj(std::vector<SG_Model>::iterator m_it);
    void loadModel_mesh(std::vector<SG_Model>::iterator m_it);

    void SGvk_Device_Create_VertexBuffer(std::vector<SG_Model>::iterator,VkDevice& device,
    VkPhysicalDevice& physicalDevice,VkCommandPool& cmdPool, VkQueue &endque);

    void SGvk_Device_Create_IndexBuffer(std::vector<SG_Model>::iterator,VkDevice& device,
    VkPhysicalDevice& physicaldevice,VkCommandPool &cmdPool, VkQueue &endque);


};









