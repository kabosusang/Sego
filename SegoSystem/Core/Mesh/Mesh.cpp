#include "Core/Mesh/Mesh.h"
//texture
#include <stb_image.h>

//注册MeshFilter

#include <rttr/registration>

using namespace rttr;
RTTR_REGISTRATION//注册反射
{
    registration::class_<MeshFilter>("MeshFilter")
            .constructor<>()(rttr::policy::ctor::as_raw_ptr);
            
}



MeshFilter::~MeshFilter()
{
    delete mesh_;
}
#include <iostream>
void MeshFilter::LoadMesh(std::string mesh_file_path)
{
    //读取 Mesh文件头
    std::ifstream input_file_stream(mesh_file_path,std::ios::in | std::ios::binary);
    MeshFileHead mesh_file_head;

    //读取文件头
    input_file_stream.read((char*)&mesh_file_head,sizeof(mesh_file_head)); 
    //读取顶点数据

    std::vector<Vertex> vertex_data(mesh_file_head.vertex_num_);
    input_file_stream.read(reinterpret_cast<char*>(vertex_data.data()), 
    mesh_file_head.vertex_num_ * sizeof(Vertex));

#ifdef GLFW_INCLUDE_VULKAN
    // Step 1: Flip Y-axis
    //vertex_data.y = 1.0f - texCoord.y;
    // Step 2: Change origin from top-left to bottom-left
    //texCoord.y = 1.0f - texCoord.y;
    for (auto data = vertex_data.begin(); data != vertex_data.end(); data++ )
    {
        data->texCoord.y = 1.0f - data->texCoord.y;
    }
    
#endif

    //读取顶点索引数据  
    std::vector<unsigned short> vertex_index_data(mesh_file_head.vertex_index_num_);
    input_file_stream.read(reinterpret_cast<char*>(vertex_index_data.data()), 
    mesh_file_head.vertex_index_num_ * sizeof(unsigned short));


    mesh_ = new Mesh();
    mesh_->vertex_num_ = mesh_file_head.vertex_num_;
    mesh_->vertex_index_num_ = mesh_file_head.vertex_index_num_;
    std::cout << "vertex_num_:" <<  mesh_->vertex_num_ << std::endl;
    std::cout << "vertex_index_num_:" <<  mesh_->vertex_index_num_ << std::endl;

    mesh_->vertex_data_ = std::move(vertex_data);
    mesh_->vertex_index_data_ = std::move(vertex_index_data);
    


        


}

Texture2D* Texture2D::LoadFromFile(std::string& image_file_path)
{
    Texture2D* texture2d = new Texture2D();

    int channels_in_file;//通道数
    
    unsigned char* data = stbi_load(image_file_path.c_str(),&(texture2d->wdith_),&(texture2d->height_),
    &channels_in_file,0);

    if(data != nullptr)
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


//释放图片文件内存
    stbi_image_free(data);
    return texture2d;
}









