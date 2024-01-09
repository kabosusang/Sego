#pragma once

//assimp
#include <assimp/matrix4x4.h>
#include <assimp/scene.h>

#include <array>
#include <map>
#include <string>
#include <vector>
//每个网格最大骨骼数量
// 在皮肤着色器中不能高于相同的常量
#define MAX_BONES 64
// 每个顶点关联的最大骨数
#define MAX_BONES_PRE_VERTEX 4 

// 每个顶点骨骼id和权重
struct VertexBoneData
{
    std::array<uint32_t,MAX_BONES_PRE_VERTEX> IDs;
    std::array<float,MAX_BONES_PRE_VERTEX> weights;

    //添加骨骼id和权重
    void Add(uint32_t boneID,float weight)
    {
        for (uint32_t i = 0; i < MAX_BONES_PRE_VERTEX ; i++)
        {
            if (weights[i] == 0.0f)
			{
				IDs[i] = boneID;
				weights[i] = weight;
				return;
			}
        }
    }
};

//骨骼中存储的矩阵信息
struct BoneInfo
{
    aiMatrix4x4 offset;
    aiMatrix4x4 finalTransformation;

    BoneInfo()
	{
		offset = aiMatrix4x4();
		finalTransformation = aiMatrix4x4();
	};  
};

class SkinnedMesh
{
    //骨骼相关数据
    // 映射骨骼名称与索引
    std::map<std::string,uint32_t> boneMapping;
    //所有骨骼数据
    std::vector<BoneInfo> boneInfo;
    //当前使用的骨骼数
    uint32_t numBones = 0;
    // 根节点逆变换矩阵
	aiMatrix4x4 globalInverseTransform;
	// 每个顶点具备的骨骼信息
	std::vector<VertexBoneData> bones;
	// Bone transformations
	std::vector<aiMatrix4x4> boneTransforms;

    //动画速度
    float animationSpeed = 0.75f;
    //当前动画
    

};










