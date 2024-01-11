#include "Vertex/MeshVertex.h"


class MeshData
{ 
public:
    const size_t c_nMaxUint16 = std::numeric_limits<unsigned short>::max();
    std::vector<MeshVertex> vertices;
    std::vector<uint32_t> indices32;
    bool bISFlipY;
public:
    MeshData():bISFlipY(true){ }
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

  
 
};
