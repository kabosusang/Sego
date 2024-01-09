#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <array>
#include <vector>
#include <glm/gtx/hash.hpp>

//MeshVertex
struct MeshVertex {
    glm::vec3 pos;
    glm::vec3 color;
    glm::vec3 normal;
    glm::vec3 tangent; //顶点切线
    glm::vec2 texCoord;
    
    MeshVertex(){}
    MeshVertex( const glm::vec3& _pos,
                const glm::vec3& _color,
                const glm::vec3& _normal,
                const glm::vec3& _tangent,
                const glm::vec2& _texCoord)
            : pos(_pos)
            , color(_color)
            , normal(_normal)
            , tangent(_tangent)
            , texCoord(_texCoord){}

    MeshVertex( float px,   float py,   float pz,
                float nx,   float ny,   float nz,
                float tx,   float ty,   float tz,
                float u,    float v)
            :pos(px,py,pz)
            ,color(1.0f,1.0f,1.0f)
            ,normal(nx,ny,nz)
            ,tangent(tx,ty,tz)
            ,texCoord(u,v){}

    MeshVertex( float px,   float py,   float pz,
                float cx,   float cy,   float cz,
                float nx,   float ny,   float nz,
                float tx,   float ty,   float tz,
                float u,    float v)
            :pos(px,py,pz)
            ,color(cx,cy,cz)
            ,normal(nx,ny,nz)
            ,tangent(tx,ty,tz)
            ,texCoord(u,v){}   




    //绑定说明
    static VkVertexInputBindingDescription getBindingDescription() {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(MeshVertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
 
        return bindingDescription;
    }
    //属性说明
    static std::array<VkVertexInputAttributeDescription, 5> getAttributeDescriptions() {
        std::array<VkVertexInputAttributeDescription, 5> attributeDescriptions{};

        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(MeshVertex, pos);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(MeshVertex, color);

        attributeDescriptions[2].binding = 0;
        attributeDescriptions[2].location = 2;
        attributeDescriptions[2].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[2].offset = offsetof(MeshVertex, normal);
        
        attributeDescriptions[3].binding = 0;
        attributeDescriptions[3].location = 3;
        attributeDescriptions[3].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[3].offset = offsetof(MeshVertex, tangent);

        attributeDescriptions[4].binding = 0;
        attributeDescriptions[4].location = 4;
        attributeDescriptions[4].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[4].offset = offsetof(MeshVertex, texCoord);

        return attributeDescriptions;
    }



bool operator==(const MeshVertex& other) const {
        return this->pos == other.pos 
        && this->color == other.color 
        && this->normal == other.normal
        && this->tangent == other.tangent
        && this->texCoord == other.texCoord;
    }
};

namespace std {
    template<> struct hash<MeshVertex> {
        size_t operator()(MeshVertex const& vertex) const {
            return ((hash<glm::vec3>()(vertex.pos) ^ 
            (hash<glm::vec3>()(vertex.color) << 1)) >> 1) 
            ^ (hash<glm::vec2>()(vertex.texCoord) << 1);
        }
    };
}
