#include "Core/Material/Material.h"

#include <iostream>
using namespace std;

Material::~Material() {
    for (auto& tex : textures_) {
        delete tex.second; // 释放 Texture2D 对象的内存
    }
}

void Material::Parse(string material_path)
{
    //解析xml
    rapidxml::file<> xml_file(material_path.c_str());
    rapidxml::xml_document<> document;
    document.parse<0>(xml_file.data());

    //根节点
    rapidxml::xml_node<>* material_node = document.first_node("material");
    if(material_node){
        Model_name_ = material_node->first_attribute("Model")->value();
        std::cout << "Load Model:" << Model_name_ << std::endl;
    }

    rapidxml::xml_node<>* material_shader_node = material_node->first_node("shader");
    if(material_shader_node != nullptr)
    {
        rapidxml::xml_attribute<>* 
        vshader_path_attribute=material_shader_node->first_attribute("Vertshader");
        rapidxml::xml_attribute<>* 
        fshader_path_attribute=material_shader_node->first_attribute("Fragshader");
        
        vshader_path_=vshader_path_attribute->value();
        fshader_path_=fshader_path_attribute->value();
    }
    //解析Texture
    rapidxml::xml_node<>* 
    material_texture_node=material_node->first_node("texture");
    for (rapidxml::xml_node<>* texture = material_node->first_node("texture"); texture; texture = texture->next_sibling("texture"))
    {
        rapidxml::xml_attribute<>* 
        texture_name_attribute=material_texture_node->first_attribute("name");

        rapidxml::xml_attribute<>* 
        texture_image_attribute=material_texture_node->first_attribute("image");

        std::string image_path=texture_image_attribute->value();
        textures_.push_back(std::make_pair(texture_name_attribute->value(), 
        Texture2D::LoadFromFile(image_path)));
        
        material_texture_node=material_texture_node->next_sibling("texture");
    }




}

