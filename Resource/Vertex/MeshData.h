#include "Vertex/MeshVertex.h"

class MeshData
{ 
public:
    const size_t c_nMaxUint16 = std::numeric_limits<unsigned short>::max();
    std::vector<MeshVertex> vertices;
    std::vector<uint32_t> indices32;
    bool bISFlipY;

public:
    MeshData():bISFlipY(false){ }
    void Clear()
    {
        vertices.clear();
        indices32.clear();
    }

    std::vector<uint32_t>& GetIndices32()
    {
        return indices32;
    }

    void* GetVertexData()
    {
        return (void*)vertices.data();
    }

    unsigned int GetVertexCount()
    {
        return (unsigned int)vertices.size();
    }

    

    unsigned int GetVertexDataSize()
    {
        return (unsigned int)(vertices.size() * sizeof(MeshVertex));
    }

    void* GetIndexData()
    {
        return (void*)indices32.data();
    }

    uint32_t GetIndexCount()
    {
        return static_cast<uint32_t>(indices32.size());
    }

    MeshVertex& GetVertex(uint32 nIndex)
    {
        return vertices[nIndex];
    }
    uint32 GetIndex(uint32 nIndex)
    {
        return indices32[nIndex];
    }



    void ReserveVertexCount(unsigned int count)
    {
        vertices.reserve(count);
    }

    void ResizeVertexCount(size_t nCount)
    {
        vertices.resize(nCount);
    }

    void ReserveIndexCount(size_t nCount)
    {
        indices32.reserve(nCount);
    }
    void ResizeIndexCount(size_t nCount)
    {
        indices32.resize(nCount);
    }

    //Change  Data
    void AddIndexTriangle(uint32 nIndex1, uint32 nIndex2, uint32 nIndex3)
    {
        indices32.push_back(nIndex1);
        indices32.push_back(nIndex2);
        indices32.push_back(nIndex3);
     }










    void clear()
    {

        vertices.clear();
        indices32.clear();
    }
};
