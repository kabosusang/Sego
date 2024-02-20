#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <iostream>
enum PipelineType 
{
    Nonepipe = 0,
    Graphics = 1,
    Stencil  = 2,
    OutLine  = 4,
    Blend    = 8,
    RayTracing = 16,
    
};


class  PipelineStatus //最普通的光照模型
{
public:
    /* Stencil */
    VkBool32 bStencilTest = VK_FALSE;
    VkBool32 bDepthTest = VK_TRUE;
    VkBool32 bDepthWrite = VK_TRUE;
    VkStencilOpState stencilOpFront;
    VkStencilOpState stencilOpBack;
    VkCullModeFlags VkCullstatus = VK_CULL_MODE_NONE;
    /* Blend */
    VkBool32 bBlend = VK_FALSE;
    VkBlendFactor blendColorFactorSrc; //原始颜色混合因子
    VkBlendFactor blendColorFactorDst; //目标颜色混合因子
    VkBlendOp blendColorOp; //颜色混合操作类型
    VkBlendFactor blendAlphaFactorSrc; //原始Alpha混合因子
    VkBlendFactor blendAlphaFactorDst; //目标Alpha混合因子
    VkBlendOp BlendAlphaOp; //Alpha混合操作类型

    VkColorComponentFlags colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;; //RGBA通道写入使能Mask



    //Default Secting
    PipelineStatus(){
        
       
    }
};

//StenCil
class PipelineStatus_stencil : public PipelineStatus
{
public:
PipelineStatus_stencil() : PipelineStatus() {
        bStencilTest = VK_TRUE;


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

//OutLine
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
/*

    VkBool32 bBlend = VK_FALSE;
    VkBlendFactor blendColorFactorSrc; //原始颜色混合因子
    VkBlendFactor blendColorFactorDst; //目标颜色混合因子
    VkBlendOp blendColorOp; //颜色混合操作类型
    VkBlendFactor blendAlphaFactorSrc; //原始Alpha混合因子
    VkBlendFactor blendAlphaFactorDst; //目标Alpha混合因子
    VkBlendOp BlendAlphaOp; //Alpha混合操作类型
    VkColorComponentFlags colorWriteMask; //RGBA通道写入使能Mask

    blendEnable： VK_TRUE
    srcColorBlendFactor：VK_BLEND_FACTOR_SRC_ALPHA
    dstColorBlendFactor：VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA
    colorBlendOp：VK_BLEND_OP_ADD
    srcAlphaBlendFactor：VK_BLEND_FACTOR_ONE
    dstAlphaBlendFactor：VK_BLEND_FACTOR_ZERO
    colorWriteMask：VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT
    实际公式:
    Color = RGBsrc * Asrc + RGBdst *（1 - Asrc）

*/



////////////////////////Blend////////////////////////////////////
class PipelineStatus_Blend : public PipelineStatus
{
public:
PipelineStatus_Blend() : PipelineStatus() {
        bBlend = VK_TRUE;
        blendColorFactorSrc = VK_BLEND_FACTOR_SRC_ALPHA;
        blendColorFactorDst = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
        blendColorOp = VK_BLEND_OP_ADD;
        blendAlphaFactorSrc = VK_BLEND_FACTOR_ONE;
        blendAlphaFactorDst = VK_BLEND_FACTOR_ZERO;
    
        //因为是透明物体所以禁止深度测试
        bDepthTest = VK_FALSE;
        bDepthWrite = VK_FALSE;

    }

};



