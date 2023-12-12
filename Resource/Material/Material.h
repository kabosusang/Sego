#pragma once
#include <rapidxml.hpp>
#include <rapidxml/rapidxml_print.hpp>
#include <rapidxml/rapidxml_utils.hpp>
#include <string>


#include "Core/Mesh/Mesh.h"

class Material
{
    void Parse(std::string material_path);

public:
    const char* Model_name_;

    std::string vshader_path_;
    std::string fshader_path_;
    std::vector<std::pair<std::string,Texture2D*>> textures_;

};