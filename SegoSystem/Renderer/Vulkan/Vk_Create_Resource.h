#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>
#include <optional>
#include <string>


#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>
#include <array>
//Vertex
struct Vertex {
    glm::vec3 pos;
    glm::vec3 color;
    glm::vec2 texCoord;
    //绑定说明
    static VkVertexInputBindingDescription getBindingDescription() {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
 
        return bindingDescription;
    }
    //属性说明
    static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions() {
        std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};

        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, pos);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, color);

        attributeDescriptions[2].binding = 0;
        attributeDescriptions[2].location = 2;
        attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[2].offset = offsetof(Vertex, texCoord);
        
        return attributeDescriptions;
    }

bool operator==(const Vertex& other) const {
        return pos == other.pos && color == other.color && texCoord == other.texCoord;
    }
};

namespace std {
    template<> struct hash<Vertex> {
        size_t operator()(Vertex const& vertex) const {
            return ((hash<glm::vec3>()(vertex.pos) ^ (hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^ (hash<glm::vec2>()(vertex.texCoord) << 1);
        }
    };
}












/**-----------------------DATA---------------------------- 
*  SG_Texture
*  SG_Models
*/
class SG_Texture
{
public:
    std::string Texture_Path;
    std::string Texture_Name;
    //Texture
    uint32_t mipLevels;//mipmaplevel
    uint32_t layerCount; //用于天空可

    VkImage textureImage;
   
    VkDescriptorImageInfo descriptor;
    VkDeviceMemory textureImageMemory;
    //纹理图像视图
    VkImageView textureImageView;
    VkSampler textureSampler;//采样器

public:
SG_Texture(std::string texturePath,std::string textureName):Texture_Path(texturePath),Texture_Name(textureName){}

SG_Texture() {}

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

    void CreateTextureView(VkDevice& device,VkImage& textureImage,VkImageView& textureImageView);
    VkImageView SGvk_Device_Create_ImageView_AttachFuc(VkDevice& device,VkImage& image, VkFormat format,VkImageAspectFlags aspectFlags);

    void SGvk_Device_Create_TextureSampler(VkDevice& device,VkPhysicalDevice& physicalDevice,VkSampler& textureSampler);

    //LoadModel
    void loadModel_tiny_obj(std::vector<SG_Model>::iterator m_it);
    void SGvk_Device_Create_VertexBuffer(std::vector<SG_Model>::iterator,VkDevice& device,
    VkPhysicalDevice& physicalDevice,VkCommandPool& cmdPool, VkQueue &endque);

    void SGvk_Device_Create_IndexBuffer(std::vector<SG_Model>::iterator,VkDevice& device,
    VkPhysicalDevice& physicaldevice,VkCommandPool &cmdPool, VkQueue &endque);




};









