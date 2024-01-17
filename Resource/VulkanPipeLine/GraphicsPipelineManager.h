#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
////////////////////////Status////////////////////////////
#include "VulkanStatus/Pipeline.h"

#include <string>
#include <vector>


class GraphicsPipelineManager
{
public:
GraphicsPipelineManager(){pipelineLayout.resize(1);graphicsPipeline.resize(1);}
~GraphicsPipelineManager();


void CreateGraphicsPipeline();
void RecreatePipeline();
////////////////Vulkan Status Function/////////////////////////
void CleanPipeline();
//InputputData
void InputShaderPath(std::string vshader_path,std::string fshader_path);
void InputPipeLineStatus();

//outputputData
uint8_t GetPipeLineCount() { return pipeLineCount;}
std::vector<VkPipelineLayout>& GetPipelineLayout() {return pipelineLayout;}
std::vector<VkPipeline>& GetGraphicsPipeline() {return graphicsPipeline;}
private:
//Pipeline
uint8_t pipeLineCount = 1;
std::vector<VkPipelineLayout> pipelineLayout; 
std::vector<VkPipeline> graphicsPipeline;

//ShaderPath
std::vector<std::string> vshader_path_;
std::vector<std::string> fshader_path_;
//Pipeline Status
std::vector<PipelineStatus*> Pipeline_status_;



};




