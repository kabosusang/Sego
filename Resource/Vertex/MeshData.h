#include "Vertex/MeshVertex.h"


class MeshData
{  
private:
    const size_t c_nMaxUint16 = std::numeric_limits<unsigned short>::max();

    std::vector<MeshVertex> vertices;
    std::vector<uint16_t> indices16;
    std::vector<uint32_t> indices32;
    bool bISFlipY;

public:
    MeshData():bISFlipY(true){ }
    void Clear()
    {
        vertices.clear();
        indices16.clear();
        indices32.clear();
    }

    std::vector<uint16_t>& GetIndices16()
    {
        return indices16;
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
        EnsureIndices16();
        if(!indices16.empty())
        {
            return (void*)indices16.data();
        }
        return (void*)indices32.data();
    }

    bool IsIndices16()
    {
        return indices32.size() < c_nMaxUint16;
    }

    void EnsureIndices16()
    {
        if(IsIndices16() && indices16.empty())
        {
            indices16.clear();
            indices16.resize(indices32.size());
            for(size_t i =0; i < indices32.size();i++)
            {
                indices16[i] = (unsigned short)indices32[i];
            }
        }
    }

};
