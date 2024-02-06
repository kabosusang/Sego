#include "Sceneobj.h"


uint32 Sego::GetVertexCount(FMeshData &meshData)
{
    return uint32();
}

void Sego::ReserveVertexCount(FMeshData &meshData, uint32 count)
{
    meshData.ReserveVertexCount(count);
}

void Sego::ResizeVertexCount(FMeshData &meshData, uint32 count)
{
    meshData.ResizeVertexCount(count);
}

MeshVertex &Sego::GetVertex(FMeshData &meshData, uint32 index)
{
    // TODO: 在此处插入 return 语句
    return meshData.vertices[index];
}

uint32 Sego::AddVertex(FMeshData &meshData, const MeshVertex &vertex)
{
    return uint32();
}

uint32 Sego::AddVertex(FMeshData &meshData, const FVector3 &vPos, const FVector4 &color, const FVector3 &normal, const FVector3 &tangent, const FVector2 &texCoord)
{
    return uint32();
}

void Sego::SetVertex(FMeshData &meshData, int index, const MeshVertex &vertex)
{
}

uint32 Sego::GetIndexCount(FMeshData &meshData)
{
    meshData.GetIndexCount();
}

void Sego::ReserveIndexCount(FMeshData &meshData, uint32 count)
{
    meshData.ReserveIndexCount(count);
}

void Sego::ResizeIndexCount(FMeshData &meshData, uint32 count)
{
    meshData.ResizeIndexCount(count);
}

uint32 Sego::GetIndex(FMeshData &meshData, uint32 index)
{
    meshData.GetIndices32()[index];
}

uint32 Sego::AddIndex(FMeshData &meshData, uint32 value)
{
    return uint32();
}

void Sego::AddIndices(FMeshData &meshData, uint32 count, uint32 *pIndex)
{
}

void Sego::SetIndex(FMeshData &meshData, uint32 index, uint32 value)
{
}

void Sego::AddIndexLine(FMeshData &meshData, uint32 index1, uint32 index2)
{
}

void Sego::AddIndexTriangle(FMeshData &meshData, uint32 index1, uint32 index2, uint32 index3)
{
    meshData.AddIndexTriangle(index1, index2, index3);
}

void Sego::SetIndexTriangle(FMeshData &meshData, uint32 indexStart, uint32 index1, uint32 index2, uint32 index3)
{
}

///  MeshGeometry
//Loop Subdivision(把每个三角形细分为四个新的小三角形)
void Sego::MeshGeometry::SubDivide(FMeshData &meshData)
{
    //      (0)
        //       v0
        //       *
        //      / \
        //  (3)/   \(5)
        //  m0*-----*m2
        //   / \   / \
        //  /   \ /   \
        // *-----*-----*
        // v1    m1    v2
        //(1)    (4)   (2)
    // Subdivide each triangle into four triangles.
    FMeshData inputCopy = meshData;
    ResizeVertexCount(meshData,0);
    ResizeIndexCount(meshData,0);

    uint32 numTris = GetIndexCount(inputCopy) / 3;

    for (uint32 i = 0; i < numTris; ++i)
        {
            const auto& v0 = inputCopy.GetVertex(inputCopy.GetIndex(i * 3 + 0));
            const auto& v1 = inputCopy.GetVertex(inputCopy.GetIndex(i * 3 + 1));
            const auto& v2 = inputCopy.GetVertex(inputCopy.GetIndex(i * 3 + 2));

            //mid
            MeshVertex m0,m1,m2;
            MidPoint(v0, v1, m0);
            MidPoint(v1, v2, m1);
            MidPoint(v0, v2, m2);

            //new
            AddVertex(meshData, v0); // 0
            AddVertex(meshData, v1); // 1
            AddVertex(meshData, v2); // 2
            AddVertex(meshData, m0); // 3
            AddVertex(meshData, m1); // 4
            AddVertex(meshData, m2); // 5

            
                //右手系坐标
                AddIndexTriangle(meshData, 
                                 i * 6 + 0,
                                 i * 6 + 3,
                                 i * 6 + 5);

                AddIndexTriangle(meshData, 
                                 i * 6 + 3,
                                 i * 6 + 4,
                                 i * 6 + 5);

                AddIndexTriangle(meshData, 
                                 i * 6 + 5,
                                 i * 6 + 4,
                                 i * 6 + 2);

                AddIndexTriangle(meshData, 
                                 i * 6 + 3,
                                 i * 6 + 1,
                                 i * 6 + 4);
}
}

void Sego::MeshGeometry::MidPoint(const FMeshVertex &v0, const FMeshVertex &v1, FMeshVertex &m)
{
    m.pos = 0.5f * (v0.pos + v1.pos);
    m.color = 0.5f * (v0.color + v1.color);
    m.normal = glm::normalize(0.5f * (v0.normal + v1.normal));
    m.tangent = glm::normalize(0.5f * (v0.tangent + v1.tangent));
    m.texCoord = 0.5f * (v0.texCoord + v1.texCoord);
}

void Sego::MeshGeometry::CreateTriangle(FMeshData &meshData, bool flipV)
{

    //        0 
    //        /\
    //       /  \
    //    1 ------ 2      左手坐标系索引为：（0 2 1） 右手为 (0 1 2)
    //FMeshVertex
    AddVertex(meshData, FMeshVertex( 0.0f,  0.5f,   0.0f,
                                        0.0f,  0.0f,  -1.0f,
                                        1.0f,  0.0f,   0.0f,
                                        0.5f,  flipV ? 1.0f : 0.0f)); //顶点0

    AddVertex(meshData, FMeshVertex(-0.5f, -0.5f,   0.0f,
                                        0.0f,  0.0f,  -1.0f,
                                        1.0f,  0.0f,   0.0f,
                                        0.0f,  flipV ? 0.0f : 1.0f)); //顶点1

    AddVertex(meshData, FMeshVertex( 0.5f, -0.5f,   0.0f,
                                        0.0f,  0.0f,  -1.0f,
                                        1.0f,  0.0f,   0.0f,
                                        1.0f,  flipV ? 0.0f : 1.0f)); //顶点2


    //Index
    AddIndexTriangle(meshData, 0, 1, 2); 
}


