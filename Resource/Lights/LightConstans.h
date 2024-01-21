#pragma once
#include <glm/glm.hpp>

struct LightConstans
{
    glm::vec4 commoon; // x:type y:enable(1 or 0); z:0-11; w:spotPower
    glm::vec3 position;//directional/point/spot
    float falloffStart; //point/spot light only
    glm::vec3 direction; //directional/spot light only
    float falloffEnd; //point/spot light only
    glm::vec4 ambient; 
    glm::vec4 diffuse;
    glm::vec4 specular;

 
    LightConstans():commoon(0,0,0,64)
    ,position(0.0f,100.0f,0.0f)
    ,falloffStart(1.0f)
    ,direction(0.0f,-1.0f,0.0f)
    ,falloffEnd(10.0f)
    ,ambient(1.0f,1.0f,1.0f,1.0f)
    ,diffuse(1.0f,1.0f,1.0f,1.0f)
    ,specular(1.0f,1.0f,1.0f,1.0f)
    {

    }
};


/*
这个结构体是用来表示光源的参数的，下面是各个参数的含义：
common：一个 glm::vec4 类型的变量，包含了多个信息：
x：光源类型，具体取值根据实际需求定义，可能表示不同种类的光源（例如，0 表示方向光，1 表示点光源等）。
y：光源是否启用的标志位，通常为 1 或 0，表示光源是否被激活。
z：用于存储额外的信息，具体取值根据实际需求定义。
w：聚光灯的光束强度。

position：一个 glm::vec3 类型的变量，用于表示光源的位置或方向。具体含义根据光源类型而定，可能表示光源的位置（对于点光源）或光源的方向（对于方向光和聚光灯）。
falloffStart：一个 float 类型的变量，仅对点光源和聚光灯有效，表示光线开始衰减的距离。
direction：一个 glm::vec3 类型的变量，仅对方向光和聚光灯有效，表示光源的方向。
falloffEnd：一个 float 类型的变量，仅对点光源和聚光灯有效，表示光线完全衰减的距离。
ambient、diffuse、specular：都是 glm::vec4 类型的变量，分别表示环境光、漫反射光和镜面光的颜色。每个颜色由四个分量组成（红、绿、蓝和透明度）。*/


//common
enum VulkanLightingType
{
        Vulkan_Lighting_Node = 0,                                //0:    None
        Vulkan_Lighting_Ambient,                                 //1:    Ambient
        Vulkan_Lighting_DiffuseLambert,                          //2:    DiffuseLambert
        Vulkan_Lighting_SpecularPhong,                           //3:    SpecularPhong
        Vulkan_Lighting_SpecularBlinnPhong,                      //4:    SpecularBlinnPhong
        Vulkan_Lighting_AmbientDiffuseLambert,                   //5:    Ambient + DiffuseLambert
        Vulkan_Lighting_AmbientSpecularPhong,                    //6:    Ambient + SpecularPhong
        Vulkan_Lighting_AmbientSpecularBlinnPhong,               //7:    Ambient + SpecularBlinnPhong
        Vulkan_Lighting_DiffuseLambertSpecularPhong,             //8:    DiffuseLambert + SpecularPhong
        Vulkan_Lighting_DiffuseLambertSpecularBlinnPhong,        //9:    DiffuseLambert + SpecularBlinnPhong
        Vulkan_Lighting_AmbientDiffuseLambertSpecularPhong,      //10:   Ambient + DiffuseLambert + SpecularPhong
        Vulkan_Lighting_AmbientDiffuseLambertSpecularBlinnPhong, //11:   Ambient + DiffuseLambert + SpecularBlinnPhong
};


