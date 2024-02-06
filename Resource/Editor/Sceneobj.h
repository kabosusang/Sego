#include "pch.h"
#include "Vertex/MeshVertex.h"
#include "Vertex/MeshData.h"
typedef uint32_t uint32;
typedef MeshData FMeshData;
typedef glm::vec3 FVector3;
typedef glm::vec4 FVector4;
typedef glm::vec2 FVector2;
typedef MeshVertex FMeshVertex;
namespace Sego
{


static uint32 GetVertexCount(FMeshData& meshData); //获取顶点数量
static void ReserveVertexCount(FMeshData& meshData, uint32 count); //预留顶点数量
static void ResizeVertexCount(FMeshData& meshData, uint32 count); //预分配顶点数量
static MeshVertex& GetVertex(FMeshData& meshData, uint32 index); //获取顶点
static uint32 AddVertex(FMeshData& meshData, const MeshVertex& vertex); //添加顶点
static uint32 AddVertex(FMeshData& meshData, 
                            const FVector3& vPos,
                            const FVector4& color, 
                            const FVector3& normal, 
                            const FVector3& tangent, 
                            const FVector2& texCoord); //添加顶点
static void SetVertex(FMeshData& meshData, int index, const MeshVertex& vertex); //设置顶点
static uint32 GetIndexCount(FMeshData& meshData); //获取索引数量
static void ReserveIndexCount(FMeshData& meshData, uint32 count); //预留索引数量
static void ResizeIndexCount(FMeshData& meshData, uint32 count); //预分配索引数量
static uint32 GetIndex(FMeshData& meshData, uint32 index); //获取索引
static uint32 AddIndex(FMeshData& meshData, uint32 value); //添加索引
static void AddIndices(FMeshData& meshData, uint32 count, uint32* pIndex); //添加索引
static void SetIndex(FMeshData& meshData, uint32 index, uint32 value); //设置索引
static void AddIndexLine(FMeshData& meshData, uint32 index1, uint32 index2); //添加线段索引
static void AddIndexTriangle(FMeshData& meshData, uint32 index1, uint32 index2, uint32 index3); //添加三角形索引
static void SetIndexTriangle(FMeshData& meshData, uint32 indexStart, uint32 index1, uint32 index2, uint32 index3); //添加三角形索引




class MeshGeometry
{
public:
static void SubDivide(FMeshData& meshData); //细分
static void MidPoint(const FMeshVertex& v0, const FMeshVertex& v1, FMeshVertex& m); //中点
static void CreateTriangle(FMeshData& meshData, bool flipV); //创建三角形



};



};















