#pragma once 

#include <glm/gtx/hash.hpp>
#include <fstream>
#include "Data/Vertex.h"
#include <cstdlib>

#include <vector>
//Mesh数据
struct Mesh
{
    unsigned short vertex_num_; //顶点个数
    unsigned short vertex_index_num_;//索引个数
    std::vector<Vertex> vertex_data_;//顶点数据
    std::vector<unsigned short> vertex_index_data_;//顶点索引数据
};

struct MeshFileHead{
    char type_[4];
    unsigned short vertex_num_; //顶点个数
    unsigned short vertex_index_num_; //索引个数
};

class MeshFilter
{
public:
    
    void LoadMesh(std::string mesh_file_path);
    Mesh* mesh(){return mesh_;};
    ~MeshFilter();

private:
Mesh* mesh_ ;
};












