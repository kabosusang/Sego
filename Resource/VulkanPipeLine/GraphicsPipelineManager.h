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
GraphicsPipelineManager(){}
~GraphicsPipelineManager();


void CreateGraphicsPipeline(std::string vshader_path,std::string fshader_path_);
void RecreatePipeline(std::string vshader_path,std::string fshader_path_);
////////////////Vulkan Status Function/////////////////////////
void CleanPipeline();
//InputputData
void InputPipeLineStatus(PipelineType type);

VkPipelineLayout& GetPipelineLayout() {return pipelineLayout;}
VkPipeline&  GetGraphicsPipeline() {return graphicsPipeline;}
private:
//Pipeline
VkPipelineLayout pipelineLayout; 
VkPipeline graphicsPipeline;

//Pipeline Status
PipelineStatus* Pipeline_status_;


};




