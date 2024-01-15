#include "Texture2d.h"
#include "VK_Global_Data.h"



Texture2D::~Texture2D()
{
vkDestroySampler(g_device,textureSampler,nullptr);
vkDestroyImageView(g_device,textureImageView,nullptr);

vkDestroyImage(g_device,textureImage, nullptr);
vkFreeMemory(g_device, textureImageMemory, nullptr);

}

Texture2D *Texture2D::Create(uint16_t Width, uint16_t height)
{
    return nullptr;
}
#include "Renderer/Vulkan/Vk_Create_Resource.h"
#include "Renderer/Vulkan/Vk_Device_Init.h"
Texture2D* Texture2D::LoadFromFile(std::string &image_file_path)
{
    Texture2D* texture2d = new Texture2D;
   
    SG_CRes::CreateTexture(texture2d,
    image_file_path,
    g_device,
    g_physicalDevice,
    app_device->commandPool,
    app_device->graphicsqueue);

    SG_CRes::CreateTextureView(g_device,texture2d->textureImage,
    texture2d->textureImageView,texture2d->mipLevels);
    SG_CRes::SGvk_Device_Create_TextureSampler(g_device,g_physicalDevice,
    texture2d->textureSampler,texture2d->mipLevels);
    
    VkDescriptorImageInfo imageInfo{};
    imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    imageInfo.imageView = texture2d->textureImageView;
    imageInfo.sampler = texture2d->textureSampler;
    texture2d->descriptor = imageInfo;

    return texture2d;
}
