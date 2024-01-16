#include "pch.h"
#include "MeshRenderer/MeshRender.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>
#include <glm/gtx/euler_angles.hpp>

#include <rttr/registration>

#include "game_object.h"
#include "transform.h"
#include "MeshFilter/MeshFilter.h"
#include "Core/Material/Material.h"


#include "Renderer/Vulkan/Vk_Create_Resource.h"
#include "Renderer/Vulkan/Vk_Device_Init.h"


std::vector<MeshRenderer*> mesh_renderer;

using namespace rttr;
RTTR_REGISTRATION//注册反射
{
    registration::class_<MeshRenderer>("MeshRenderer")
            .constructor<>()(rttr::policy::ctor::as_raw_ptr);
}

void MeshRenderer::SetMaterial(Material* material) {
    material_=material;
    SG_CORE_INFO("Texture Name : {0}",material_->textures_.data()->first.c_str());

    std::vector<Texture2D*> texture2d;

    // 遍历源向量并提取 Texture2D* 添加到目标向量中
    for (const auto& pair : material_->textures_) {
        texture2d.push_back(pair.second);
    }


    //主动获取 MeshFilter 组件
    auto component_mesh_filter=game_object()->GetComponent("MeshFilter");
    auto mesh_filter=dynamic_cast<MeshFilter*>(component_mesh_filter);
    if(!mesh_filter){
        return;
    }

    SG_CRes::SGvk_CreateDescriptorSets(
        app_device->descriptorSetLayout,
        app_device->descriptorPool,
        Obj_DescriptorSets_,
        texture2d,
        mesh_filter->Obj_uniformBuffers_,
        mesh_filter->Obj_uniformBuffersMemory_,
        mesh_filter->Obj_uniformBuffersMapped_
    );

    //CreatePipeLine
    CreateGraphicsPipeline();
    
}

MeshRenderer::MeshRenderer()
{

}
#include "VK_Global_Data.h"
#include "MeshRender.h"
MeshRenderer::~MeshRenderer()
{
    auto component_mesh_filter=game_object()->GetComponent("MeshFilter");
    auto mesh_filter=dynamic_cast<MeshFilter*>(component_mesh_filter);
    //Models
    vkDestroyBuffer(g_device,mesh_filter->indexBuffer,nullptr);
    vkFreeMemory(g_device, mesh_filter->indexBufferMemory , nullptr);

    vkDestroyBuffer(g_device,mesh_filter->vertexBuffer, nullptr);//清理缓冲区
    vkFreeMemory(g_device, mesh_filter->vertexBufferMemory, nullptr);

    //Pipeline
    vkDestroyPipeline(g_device, graphicsPipeline, nullptr);
    vkDestroyPipelineLayout(g_device, pipelineLayout, nullptr);

    delete material_;
}




void MeshRenderer::Render(VkCommandBuffer& commandBuffer,uint32_t imageIndex)
{
//主动获取Transform组件 ,计算mvp
auto transform = dynamic_cast<Transform*>(game_object()->GetComponent("Transform"));
if(!transform)
{
    SG_CORE_ERROR("/////////////Failed to GetComponent->Transform");
    return;
}

auto rotation=transform->rotation();
glm::mat4 trans = glm::translate(transform->position());
glm::mat4 scale = glm::scale(transform->scale()); //缩放
glm::mat4 eulerAngleYXZ = glm::eulerAngleYXZ(glm::radians(rotation.y), glm::radians(rotation.x), glm::radians(rotation.z));
Model_mat4_ = trans * scale * eulerAngleYXZ;


//主动获取 MeshFilter 组件
    auto component_mesh_filter=game_object()->GetComponent("MeshFilter");
    auto mesh_filter=dynamic_cast<MeshFilter*>(component_mesh_filter);
    if(!mesh_filter){
        return;
    }


    //绑定图形管道
    vkCmdBindPipeline(commandBuffer,VK_PIPELINE_BIND_POINT_GRAPHICS,graphicsPipeline);

    VkDeviceSize offsets[] = {0};
    // 绑定顶点缓冲区
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, &mesh_filter->vertexBuffer ,offsets);
    // 绑定描述符集
    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, 
    pipelineLayout, 0, 1, &Obj_DescriptorSets_[app_device->currentFrame] ,0, nullptr);
    
    // 绑定索引缓冲区并绘制模型
    vkCmdBindIndexBuffer(commandBuffer, mesh_filter->indexBuffer, 0, VK_INDEX_TYPE_UINT32);
    
    vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(mesh_filter->mesh()->indices32.size()), 1, 0, 0, 0);

}

std::vector<void *>& MeshRenderer::GetuniformBuffersMapped()
{
    //主动获取 MeshFilter 组件
    auto component_mesh_filter=game_object()->GetComponent("MeshFilter");
    auto mesh_filter=dynamic_cast<MeshFilter*>(component_mesh_filter);
   
    return mesh_filter->Obj_uniformBuffersMapped_;
}

//shadermodule
VkShaderModule CreateShaderModule(const std::vector<char>& code)
{
    VkShaderModuleCreateInfo createInfo{};

    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());
    
    VkShaderModule shaderModule;
if (vkCreateShaderModule(g_device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
    SG_CORE_ERROR("failed to create shader module!");
    }
    return shaderModule;
}

//加载着色器代码
static std::vector<char>readFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::ate | std::ios::binary);
    
    if (!file.is_open()) {
        throw std::runtime_error("failed to open file!");
    }

    size_t fileSize = (size_t) file.tellg();//tellg() 用于在输入流中获取位置
    std::vector<char> buffer(fileSize);
    //回到文件开头 并读取所有字节
    file.seekg(0);//seekg()用于设置在输入流中的位置
    file.read(buffer.data(), fileSize);

    file.close();
    return buffer;
}

void MeshRenderer::CreateGraphicsPipeline()
{
auto vertShaderCode = readFile(material_->vshader_path_);
auto fragShaderCode = readFile(material_->fshader_path_);
std::cout << "vertShaderCode has Filesize : " <<vertShaderCode.size() << std::endl;
std::cout << "fragShaderCode has Filesize : " <<fragShaderCode.size() << std::endl;
//创建着色器模块
VkShaderModule vertShaderMoudle = CreateShaderModule(vertShaderCode);
VkShaderModule fragShaderModule = CreateShaderModule(fragShaderCode);

//着色器阶段创建
//vert
VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;

vertShaderStageInfo.module = vertShaderMoudle;
vertShaderStageInfo.pName = "main";

//frag
VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
fragShaderStageInfo.module = fragShaderModule;
fragShaderStageInfo.pName = "main";

VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};

//顶点输入
VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
auto bindingDescription = MeshVertex::getBindingDescription();
auto attributeDescriptions = MeshVertex::getAttributeDescriptions();
vertexInputInfo.vertexBindingDescriptionCount = 1;
vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

//输入程序集
VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
inputAssembly.primitiveRestartEnable = VK_FALSE;

//指定计数
VkPipelineViewportStateCreateInfo viewportState{};
viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
viewportState.viewportCount = 1;
viewportState.scissorCount = 1;

/*如果没有动态状态，则需要使用
VkPipelineViewportStateCreateInfo viewportState{};
viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
viewportState.viewportCount = 1;
viewportState.pViewports = &viewport;
viewportState.scissorCount = 1;
viewportState.pScissors = &scissor;*/

//光栅器 
VkPipelineRasterizationStateCreateInfo rasterizer{};
rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
rasterizer.depthClampEnable = VK_FALSE;
rasterizer.rasterizerDiscardEnable = VK_FALSE;
rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
rasterizer.lineWidth = 1.0f;
rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
rasterizer.depthBiasEnable = VK_FALSE;
rasterizer.depthBiasConstantFactor = 0.0f; // Optional
rasterizer.depthBiasClamp = 0.0f; // Optional
rasterizer.depthBiasSlopeFactor = 0.0f; // Optional

//多重采样
VkPipelineMultisampleStateCreateInfo multisampling{};
multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
multisampling.sampleShadingEnable = VK_FALSE;
multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
multisampling.minSampleShading = 1.0f; // Optional
multisampling.pSampleMask = nullptr; // Optional
multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
multisampling.alphaToOneEnable = VK_FALSE; // Optional


//深度和模板测试
//颜色混合

VkPipelineColorBlendAttachmentState colorBlendAttachment{};
colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
colorBlendAttachment.blendEnable = VK_FALSE;

VkPipelineColorBlendStateCreateInfo colorBlending{};
colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
colorBlending.logicOpEnable = VK_FALSE;
colorBlending.logicOp = VK_LOGIC_OP_COPY;
colorBlending.attachmentCount = 1;
colorBlending.pAttachments = &colorBlendAttachment;
colorBlending.blendConstants[0] = 0.0f;
colorBlending.blendConstants[1] = 0.0f;
colorBlending.blendConstants[2] = 0.0f;
colorBlending.blendConstants[3] = 0.0f;

//动态状态
    std::vector<VkDynamicState> dynamicStates = {
    VK_DYNAMIC_STATE_VIEWPORT,    
    VK_DYNAMIC_STATE_SCISSOR
    };
    
    VkPipelineDynamicStateCreateInfo dynamicState{};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
    dynamicState.pDynamicStates = dynamicStates.data();
    

//管道布局
VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
pipelineLayoutInfo.setLayoutCount = 1; // Optional
pipelineLayoutInfo.pSetLayouts = &app_device->descriptorSetLayout; // Optional
pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional

if (vkCreatePipelineLayout(g_device, &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
    SG_CORE_ERROR("failed to create pipeline layout!");
}
//深度
VkPipelineDepthStencilStateCreateInfo depthStencil{};
depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
depthStencil.depthTestEnable = VK_TRUE;
depthStencil.depthWriteEnable = VK_TRUE;
depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
depthStencil.depthBoundsTestEnable = VK_FALSE;
depthStencil.stencilTestEnable = VK_FALSE;


VkGraphicsPipelineCreateInfo pipelineInfo{};
pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
pipelineInfo.stageCount = 2;
pipelineInfo.pStages = shaderStages;
//深度
pipelineInfo.pDepthStencilState = &depthStencil;

pipelineInfo.pVertexInputState = &vertexInputInfo;
pipelineInfo.pInputAssemblyState = &inputAssembly;
pipelineInfo.pViewportState = &viewportState;
pipelineInfo.pRasterizationState = &rasterizer;
pipelineInfo.pMultisampleState = &multisampling;

pipelineInfo.pColorBlendState = &colorBlending;
pipelineInfo.pDynamicState = &dynamicState;

pipelineInfo.layout = pipelineLayout;

pipelineInfo.renderPass = app_device->renderPass;
pipelineInfo.subpass = 0;

pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
pipelineInfo.basePipelineIndex = -1; // Optional

if (vkCreateGraphicsPipelines(g_device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline) != VK_SUCCESS) {
    SG_CORE_ERROR("failed to create graphics pipeline!");
}
    //销毁着色器模块
    vkDestroyShaderModule(g_device,fragShaderModule,nullptr);
    vkDestroyShaderModule(g_device,vertShaderMoudle,nullptr);

}

void MeshRenderer::RecreatePipeline()
{
vkDestroyPipeline(g_device, graphicsPipeline, nullptr);
vkDestroyPipelineLayout(g_device, pipelineLayout, nullptr);

CreateGraphicsPipeline(); //重新创建

}
