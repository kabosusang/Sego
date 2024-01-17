#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>


class  PipelineStatus //最普通的光照模型
{
public:
    /* data */
    VkBool32 bStencilTest = VK_FALSE;
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

        stencilOpBack.failOp = VK_STENCIL_OP_REPLACE;
        stencilOpBack.passOp = VK_STENCIL_OP_REPLACE;
        stencilOpBack.depthFailOp = VK_STENCIL_OP_REPLACE;
        stencilOpBack.compareOp = VK_COMPARE_OP_ALWAYS;
        stencilOpBack.compareMask = 0xFF;
        stencilOpBack.writeMask = 0xFF;
        stencilOpBack.reference = 1;
    }
};

class PipelineStatus_OutLine : public PipelineStatus
{
PipelineStatus_OutLine() : PipelineStatus() {
        bStencilTest = VK_TRUE;
    }


};

