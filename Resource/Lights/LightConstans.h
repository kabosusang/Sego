#pragma once
#include <glm/glm.hpp>

struct PhoneConstans
{
    //Materia
    alignas(16) glm::vec3 viewpos; //12字节 
    alignas(16) glm::vec3 diffuse = glm::vec3(.5f,0.5f,0.5f);
    alignas(16) glm::vec3 specular = glm::vec3(1.0f);
    alignas(4)  float shininess = 32.0f;
    //Light
    alignas(4) float lightambient = 0.15f; //环境光照
    alignas(4) float lightspecular = 0.05f; //镜面光分量

    //direction
    alignas(4) float constant = 1.0f;
    alignas(4) float linear    = 0.09f;
    alignas(4) float quadratic = 0.032f;
   
    PhoneConstans():viewpos(0.f),diffuse(0.5f),specular(1.0f),shininess(32.0f)
    ,lightambient(0.15f),lightspecular(0.05f),constant(1.0f),linear(0.09f),quadratic(0.032f) {}
};
extern PhoneConstans phone;
