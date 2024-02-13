#include "pch.h"
#include "Vertex/MeshData.h"
#include <glm/gtx/quaternion.hpp>

typedef uint32_t uint32;
typedef MeshData FMeshData;
typedef glm::vec3 FVector3;
typedef glm::vec4 FVector4;
typedef glm::vec2 FVector2;
typedef MeshVertex FMeshVertex;
const float SegoPi = glm::pi<float>();
const float SegoPi_TWO = glm::pi<float>()* 2.0f;
const float SegoPi_Half = glm::pi<float>() /  2.0f;
typedef glm::quat FQuaternion;
typedef glm::mat4 FMatrix4;

namespace Sego
{

void s_BuildCylinderTopCap(FMeshData& meshData, 
                               float bottomRadius, 
                               float topRadius, 
                               float height, 
                               float heightOffset,
                               uint32 sliceCount, 
                               uint32 stackCount, 
                               bool flipV
                               );

void s_BuildCylinderBottomCap(FMeshData& meshData, 
                                  float bottomRadius, 
                                  float topRadius, 
                                  float height, 
                                  float heightOffset,
                                  uint32 sliceCount, 
                                  uint32 stackCount, 
                                  bool flipV
                                  );
FVector3 Transform(const FMatrix4& mat4, const FVector3& v)
{
        FVector4 vRet = mat4 * FVector4(v.x, v.y, v.z, 1.0f);
        return FVector3(vRet.x, vRet.y, vRet.z);
}
FVector3 Transform(const FQuaternion& qRot, const FVector3& v)
{
        return glm::rotate(qRot, v);
}

class MeshGeometry
{
public:
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



static void SubDivide(FMeshData& meshData); //细分
static void MidPoint(const FMeshVertex& v0, const FMeshVertex& v1, FMeshVertex& m); //中点
static void CreateTriangle(FMeshData& meshData, bool flipV); //创建三角形
static void CreateQuad(FMeshData& meshData, 
                                   float centerX, 
                                   float centerY, 
                                   float width, 
                                   float height, 
                                   float depth,
                                   bool flipV);
static void CreateGrid(FMeshData& meshData, 
                                   float width, 
                                   float height, 
                                   uint32 m, 
                                   uint32 n,
                                   bool flipV);
static void CreateCircle(FMeshData& meshData, 
                                   float radius, 
                                   uint32 sliceCount, 
                                   bool flipV);        
static void CreateAABB(FMeshData& meshData, 
                                   float width, 
                                   float height, 
                                   float depth, 
                                   uint32 numSubdivisions,
                                   bool flipV);
static void CreateSphere(FMeshData& meshData, 
                                   float radius, 
                                   uint32 sliceCount, //经度数, >=1
                                   uint32 stackCount,//纬度数, >=2
                                   bool flipV);

static void CreateGeosphere(FMeshData& meshData, 
                                        float radius, 
                                        uint32 numSubdivisions,
                                        bool flipV);

static void CreateCylinder(FMeshData& meshData,float bottomRadius, 
                               float topRadius, 
                               float height, 
                               float heightOffset,
                               uint32 sliceCount, 
                               uint32 stackCount, 
                               bool flipV);
static void CreateCapsule(FMeshData& meshData,
                                      float radius,
                                      float height,
                                      float heightOffset,
                                      uint32 numRings,
                                      uint32 numSegments,
                                      uint32 numSegHeight,
                                      bool flipV);

static void CreateCone(FMeshData& meshData,
                                   float radius,
                                   float height,
                                   float heightOffset,
                                   uint32 numSegBase,
                                   uint32 numSegHeight,
                                   bool flipV);

static void CreateTorus(FMeshData& meshData,
                                    float radius,
                                    float sectionRadius,
                                    uint32 numSegSection,
                                    uint32 numSegCircle,
                                    bool flipV);





};



};















