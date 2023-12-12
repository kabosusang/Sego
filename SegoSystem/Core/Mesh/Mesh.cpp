#include "Core/Mesh/Mesh.h"

using namespace std;
MeshFilter::~MeshFilter()
{
    delete mesh_;
}

void MeshFilter::LoadMesh(string mesh_file_path)
{
    //读取 Mesh文件头
    ifstream input_file_stream(mesh_file_path,ios::in | ios::binary);
    MeshFileHead mesh_file_head;

    //读取文件头
    input_file_stream.read((char*)&mesh_file_head,sizeof(mesh_file_head)); 
    //读取顶点数据
    unsigned char* vertex_data = (unsigned char*)malloc(mesh_file_head.vertex_num_*sizeof(Msh_Vertex));
    input_file_stream.read((char*)vertex_data,mesh_file_head.vertex_index_num_*sizeof(Msh_Vertex));

    //读取顶点索引数据  
    unsigned short* vertex_index_data = (unsigned short*)malloc(mesh_file_head.vertex_index_num_*sizeof(unsigned short));
    input_file_stream.read((char*)vertex_index_data,mesh_file_head.vertex_index_num_*sizeof(unsigned short));

    input_file_stream.close();

    mesh_ = new Mesh();
    mesh_->vertex_num_=mesh_file_head.vertex_num_;
    mesh_->vertex_index_num_=mesh_file_head.vertex_index_num_;
    mesh_->vertex_data_=(Msh_Vertex*)vertex_data;
    mesh_->vertex_index_data_=vertex_index_data;

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
                texture2d->texture_format_ = GL_ALPHA;
                break;
            }
            case 3:
            {
                texture2d->texture_format_ = GL_RGB;
                break;
            }
            case 4:
            {
                texture2d->texture_format_ = GL_RGBA;
                break;
            }
        }

    }


//释放图片文件内存
    stbi_image_free(data);
    return texture2d;
}






