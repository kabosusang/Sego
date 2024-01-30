#pragma once
#include <rapidxml.hpp>
#include <rapidxml_print.hpp>
#include <rapidxml_utils.hpp>
#include <string>

#include "Texture/TextureCommon/TextureCommon.h"

class MaterialShadTex
{
public:
    void Parse(std::string material_path);
    MaterialShadTex():Model_name_("null"){};

    ~MaterialShadTex();
public:
    const char* Model_name_;

    std::vector<std::string> vshader_path_;
    std::vector<std::string> fshader_path_;
    std::vector<std::pair<std::string,TextureCommon*>> textures_;

};