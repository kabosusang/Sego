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

#include "Vk_Texture.h"
#include "Vk_Image.h"

TextureCommon *TextureCommon::LoadFromFile(std::vector<std::string>& image_file_path)
{
    TextureCommon* texturecommon = new TextureCommon;

    




    return texturecommon;

}

void TextureCommon::LoadTexture(std::vector<std::string>& image_file_path,int width, int height, int depth)
{

    if(this->Textype == "Vulkan_Texture_1D")
    {
        this->type = VK_IMAGE_TYPE_1D;
        vulkan::texture::CreateTexture1D_Overload_Set4(image_file_path[0],
        this->mipLevels,this->textureImage,this->textureImageMemory);

        vulkan::resource::image::CreateImageView(textureImage, 
        VK_IMAGE_VIEW_TYPE_1D, this->format,VK_IMAGE_ASPECT_COLOR_BIT, this->mipLevels,
        1,this->textureImageView);
        
    }
    else if(this->Textype == "Vulkan_Texture_2D")
    {
        this->type = VK_IMAGE_TYPE_2D;
        vulkan::texture::CreateTexture2D_Overload_Set4(image_file_path[0],
        this->mipLevels,this->textureImage,this->textureImageMemory);
        vulkan::resource::image::CreateImageView(textureImage,
        VK_IMAGE_VIEW_TYPE_2D, this->format,VK_IMAGE_ASPECT_COLOR_BIT, this->mipLevels,
        1,this->textureImageView);
    }
    else if(this->Textype == "Vulkan_Texture_2DArray")
    {
        this->type = VK_IMAGE_TYPE_2D;
        vulkan::texture::CreateTexture2DArray_Overload_Set4(image_file_path,
        this->mipLevels,this->textureImage,this->textureImageMemory);

        vulkan::resource::image::CreateImageView(textureImage,
        VK_IMAGE_VIEW_TYPE_2D_ARRAY, this->format,VK_IMAGE_ASPECT_COLOR_BIT, this->mipLevels,
        1,this->textureImageView);


    }
    else if(this->Textype == "Vulkan_Texture_3D")
    {
        this->type = VK_IMAGE_TYPE_3D;
        this->width_ = width;
        this->height_ = height;
        this->depth_ = depth;
        uint32_t size = width * height * depth;
        this->pDataRGBA = new uint8_t[size];
        vulkan::texture::CreateTexture3D(this->format,this->pDataRGBA,
        size,width,height,depth,this->textureImage,this->textureImageMemory);
        vulkan::resource::image::CreateImageView(textureImage,
        VK_IMAGE_VIEW_TYPE_3D, this->format,VK_IMAGE_ASPECT_COLOR_BIT, this->mipLevels,
        1,this->textureImageView);

        

    }
    else if(this->Textype == "Vulkan_Texture_CubeMap")
    {
        this->type = VK_IMAGE_TYPE_2D;
        this->isCubeMap = true;
        vulkan::texture::CreateTextureCubeMap_Overload_Set4(image_file_path,
        this->mipLevels,this->textureImage,this->textureImageMemory);
        vulkan::resource::image::CreateImageView(textureImage,
        VK_IMAGE_VIEW_TYPE_CUBE, this->format,VK_IMAGE_ASPECT_COLOR_BIT, this->mipLevels,
        1,this->textureImageView);
    }


    //Create Sampler
    vulkan::resource::image::CreateSampler(this->texfilter,
    this->texAddressing,this->texBorderColor,
    this->enableAnisotropy,
    this->minLod,
    this->maxLod,this->mipLodBias,this->textureSampler);

}
