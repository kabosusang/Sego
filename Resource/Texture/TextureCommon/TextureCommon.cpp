#include "pch.h"

#include "TextureCommon.h"
#include "VK_Global_Data.h"

TextureCommon::~TextureCommon()
{
vkDestroySampler(g_device,textureSampler,nullptr);
vkDestroyImageView(g_device,textureImageView,nullptr);

vkDestroyImage(g_device,textureImage, nullptr);
vkFreeMemory(g_device, textureImageMemory, nullptr);

}
#include "Renderer/Vulkan/Vk_Create_Resource.h"
#include "Renderer/Vulkan/Vk_Device_Init.h"

void TextureCommon::LoadImageData(std::string_view Image_path,TextureCommon* texture)
{
int texChannels;
stbi_uc* pixels = stbi_load(Image_path.data(),
&texture->width_,&texture->height_,&texChannels,
STBI_rgb_alpha);

VkDeviceSize imageSize = texture->width_ * texture->height_ * 4;
    if (!pixels) {
        SG_CORE_ERROR("failed to load texture image!");
    }
        //根据颜色通道数，判断颜色格式。
        switch (texChannels) {
            case 1:
            {
                texture->texture_format_ = texColorformat::TX_ALPHA;
                break;
            }
            case 3:
            {
                texture->texture_format_ = texColorformat::TX_RGB;
                break;
            }
            case 4:
            {
                texture->texture_format_ = texColorformat::TX_RGBA;
                break;
            }
        }

}






TextureCommon *TextureCommon::LoadFromFile(std::string &image_file_path)
{
    TextureCommon* texturecommon = new TextureCommon;
   
    
    

    return texturecommon;

}


