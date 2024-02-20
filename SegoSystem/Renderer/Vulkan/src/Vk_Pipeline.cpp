#include "pch.h"
#include "../include/Vk_pipeline.h"
#include "../include/Vk_Shader.h"
#include "VK_Global_Data.h"

using namespace vulkan::resource::Shader;
void vulkan::resource::Pipeline::CreateGraphicsPipeline(std::string_view vshader_path, 
std::string_view fshader_path_,
VkVertexInputBindingDescription bindingDescription,
std::vector<VkVertexInputAttributeDescription> attributeDescriptions
)
{
/*
VkShaderModule vertShaderMoudle = CreateShaderModule(vshader_path);
VkShaderModule fragShaderModule = CreateShaderModule(fshader_path_);

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
GraphicsPipelineManagerViewportStateCreateInfo viewportState{};
viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
viewportState.viewportCount = 1;
viewportState.pViewports = &viewport;
viewportState.scissorCount = 1;
viewportState.pScissors = &scissor;*/
/*
//光栅器 
VkPipelineRasterizationStateCreateInfo rasterizer{};
rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
rasterizer.depthClampEnable = VK_FALSE;
rasterizer.rasterizerDiscardEnable = VK_FALSE;
rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
rasterizer.lineWidth = 1.0f;
rasterizer.cullMode = Pipeline_status_->VkCullstatus;
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
colorBlendAttachment.colorWriteMask = Pipeline_status_->colorWriteMask;//VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
colorBlendAttachment.blendEnable = Pipeline_status_->bBlend;
if(Pipeline_status_->bBlend)
{
    colorBlendAttachment.srcColorBlendFactor = Pipeline_status_->blendColorFactorSrc;
    colorBlendAttachment.dstColorBlendFactor = Pipeline_status_->blendColorFactorDst;
    colorBlendAttachment.colorBlendOp = Pipeline_status_->blendColorOp;
    colorBlendAttachment.srcAlphaBlendFactor = Pipeline_status_->blendAlphaFactorSrc;
    colorBlendAttachment.dstAlphaBlendFactor = Pipeline_status_->blendAlphaFactorDst;
    colorBlendAttachment.alphaBlendOp = Pipeline_status_->BlendAlphaOp;
}



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
depthStencil.depthTestEnable = Pipeline_status_->bDepthTest;
depthStencil.depthWriteEnable = Pipeline_status_->bDepthWrite;
depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
depthStencil.depthBoundsTestEnable = VK_FALSE;
depthStencil.stencilTestEnable = Pipeline_status_->bStencilTest;

if(Pipeline_status_->bStencilTest)
{
    depthStencil.front = Pipeline_status_->stencilOpFront;
    depthStencil.back  = Pipeline_status_->stencilOpBack;
}


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
    vkDestroyShaderModule(g_device,vertShaderMoudle,nullptr);
    vkDestroyShaderModule(g_device,fragShaderModule,nullptr);
*/
}
