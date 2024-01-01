#include "Core/Texture2D/Texture2d.h"
#include "Renderer/Vulkan/Vk_Allocate.h"
#include "VK_Global_Data.h"

//texture
#include <stb_image.h>
#include "Texture2d.h"


Texture2D* Texture2D::LoadFromFile(std::string &image_file_path,VkBuffer& stagingBuffer,VkDeviceMemory& stagingBufferMemory)
{
    Texture2D* texture2d = new Texture2D();

    int channels_in_file;//通道数
    
    unsigned char* pixels = stbi_load(image_file_path.c_str(),&(texture2d->wdith_),&(texture2d->height_),
    &channels_in_file,0);

    if(pixels != nullptr)
    {
        //根据颜色通道数，判断颜色格式。
        switch (channels_in_file) {
            case 1:
            {
                texture2d->texture_format_ = texformat::TX_ALPHA;
                break;
            }
            case 3:
            {
                texture2d->texture_format_ = texformat::TX_RGB;
                break;
            }
            case 4:
            {
                texture2d->texture_format_ = texformat::TX_RGBA;
                break;
            }
        }
    }

VkDeviceSize imageSize = texture2d->wdith_ * texture2d->height_ * 4;
  
SG_Allocate::SGvk_Device_Create_Buffer(imageSize, 
VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | 
VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
stagingBuffer, 
stagingBufferMemory,g_device,g_physicalDevice);

void* data;
vkMapMemory(g_device, stagingBufferMemory, 0, imageSize, 0, &data);
memcpy(data, pixels, static_cast<size_t>(imageSize));
vkUnmapMemory(g_device, stagingBufferMemory);

stbi_image_free(pixels);

return texture2d;

}