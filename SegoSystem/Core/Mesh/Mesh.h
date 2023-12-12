#pragma once 
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>
#include <fstream>

struct Msh_Vertex
{
    glm::vec3 pos_;
    glm::vec4 color_;
    glm::vec2 uv_;
};

//Mesh数据
struct Mesh
{
    unsigned short vertex_num_; //顶点个数
    unsigned short vertex_index_num_;//索引个数
    Msh_Vertex* vertex_data_;//顶点数据
    unsigned short* vertex_index_data_;//顶点索引数据
};

struct MeshFileHead{
    char type_[4];
    int vertex_num_; //顶点个数
    int vertex_index_num_; //索引个数
};


class MeshFilter
{
    void LoadMesh(std::string mesh_file_path);
    Mesh* mesh(){return mesh_;};
    ~MeshFilter();

private:
Mesh* mesh_;
};

//texture
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

enum tex_format
{
	GL_ALPHA = 1,
	GL_RGB = 3,
	GL_RGBA = 4
};

class Texture2D
{
public:
    static Texture2D* LoadFromFile(std::string& image_file_path);
    
public:
    int wdith_;
    int height_;
    tex_format texture_format_;
};










