#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <iostream>
enum PipelineType 
{
    Nonepipe = 0,
    Graphics = 1,
     stencil = 2,
    OutLine = 4,
    RayTracing = 8
};




class  PipelineStatus //最普通的光照模型
{
public:
    /* data */
    VkBool32 bStencilTest = VK_FALSE;
    VkBool32 bDepthTest = VK_TRUE;
    VkBool32 bDepthWrite = VK_TRUE;
    VkStencilOpState stencilOpFront;
    VkStencilOpState stencilOpBack;
    VkCullModeFlags VkCullstatus = VK_CULL_MODE_NONE;

    //Default Secting
    PipelineStatus(){
        stencilOpFront.failOp = VK_STENCIL_OP_REPLACE;
        stencilOpFront.passOp = VK_STENCIL_OP_REPLACE;
        stencilOpFront.depthFailOp = VK_STENCIL_OP_REPLACE;
        stencilOpFront.compareOp = VK_COMPARE_OP_ALWAYS;
        stencilOpFront.compareMask = 0xFF;
        stencilOpFront.writeMask = 0xFF;
        stencilOpFront.reference = 1;

        stencilOpBack = stencilOpFront;
       
    }
};


class PipelineStatus_stencil : public PipelineStatus
{
public:
PipelineStatus_stencil() : PipelineStatus() {
        bStencilTest = VK_TRUE;
    }

};

class PipelineStatus_OutLine : public PipelineStatus
{
public:
PipelineStatus_OutLine() : PipelineStatus() {
        bStencilTest = VK_TRUE;
        bDepthTest = VK_FALSE;
        bDepthWrite = VK_FALSE;

        stencilOpFront.failOp = VK_STENCIL_OP_KEEP;
        stencilOpFront.passOp = VK_STENCIL_OP_REPLACE;
        stencilOpFront.depthFailOp = VK_STENCIL_OP_KEEP;
        stencilOpFront.compareOp = VK_COMPARE_OP_NOT_EQUAL;
        stencilOpFront.compareMask = 0xFF;
        stencilOpFront.writeMask = 0xFF;
        stencilOpFront.reference = 1;

        stencilOpBack = stencilOpFront;
    }

};

