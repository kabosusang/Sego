#pragma once
#include <rapidxml.hpp>
#include <rapidxml_print.hpp>
#include <rapidxml_utils.hpp>
#include <string>


#include "Core/Mesh/Mesh.h"
#include "Texture/Texture2D/Texture2d.h"

class Material
{
public:
    void Parse(std::string material_path);
    Material():Model_name_("null"),vshader_path_("null"),fshader_path_("null") {};

public:
    const char* Model_name_;

    std::string vshader_path_;
    std::string fshader_path_;
    std::vector<std::pair<std::string,Texture2D*>> textures_;

};