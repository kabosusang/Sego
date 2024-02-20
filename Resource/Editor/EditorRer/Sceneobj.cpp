#include "Sceneobj.h"

uint32 Sego::MeshGeometry::GetVertexCount(FMeshData &meshData)
{
    return meshData.GetVertexCount();
}

void Sego::MeshGeometry::ReserveVertexCount(FMeshData &meshData, uint32 count)
{
    meshData.ReserveVertexCount(count);
}

void Sego::MeshGeometry::ResizeVertexCount(FMeshData &meshData, uint32 count)
{
    meshData.ResizeVertexCount(count);
}

MeshVertex &Sego::MeshGeometry::GetVertex(FMeshData &meshData, uint32 index)
{
    // TODO: 在此处插入 return 语句
    return meshData.vertices[index];
}

uint32 Sego::MeshGeometry::AddVertex(FMeshData &meshData, const MeshVertex &vertex)
{
    meshData.AddVertex(vertex);
}

uint32 Sego::MeshGeometry::AddVertex(FMeshData &meshData, const FVector3 &vPos,
 const FVector4 &color, const FVector3 &normal, const FVector3 &tangent, const FVector2 &texCoord)
{
    MeshVertex vertex;
    vertex.pos = vPos;
    vertex.color = color;
    vertex.normal = normal;
    vertex.tangent = tangent;
    vertex.texCoord = texCoord;
    meshData.AddVertex(vertex);

    return 1;
}

void Sego::MeshGeometry::SetVertex(FMeshData &meshData, int index, const MeshVertex &vertex)
{
    meshData.vertices[index] = vertex;
}

uint32 Sego::MeshGeometry::GetIndexCount(FMeshData &meshData)
{
    meshData.GetIndexCount();
}

void Sego::MeshGeometry::ReserveIndexCount(FMeshData &meshData, uint32 count)
{
    meshData.ReserveIndexCount(count);
}

void Sego::MeshGeometry::ResizeIndexCount(FMeshData &meshData, uint32 count)
{
    meshData.ResizeIndexCount(count);
}

uint32 Sego::MeshGeometry::GetIndex(FMeshData &meshData, uint32 index)
{
    meshData.GetIndices32()[index];
}

uint32 Sego::MeshGeometry::AddIndex(FMeshData &meshData, uint32 value)
{
    meshData.GetIndices32().push_back(value);
    return 1;
}

void Sego::MeshGeometry::AddIndices(FMeshData &meshData, uint32 count, uint32 *pIndex)
{
    for (uint32 i = 0; i < count; i++)
    {
        meshData.GetIndices32().push_back(pIndex[i]);
    }
}

void Sego::MeshGeometry::SetIndex(FMeshData &meshData, uint32 index, uint32 value)
{
    meshData.GetIndices32()[index] = value;

}

void Sego::MeshGeometry::AddIndexLine(FMeshData &meshData, uint32 index1, uint32 index2)
{
    meshData.AddIndexLine(index1, index2);
}

void Sego::MeshGeometry::AddIndexTriangle(FMeshData &meshData, uint32 index1, uint32 index2, uint32 index3)
{
    meshData.AddIndexTriangle(index1, index2, index3);
}

void Sego::MeshGeometry::SetIndexTriangle(FMeshData &meshData, uint32 indexStart, uint32 index1, uint32 index2, uint32 index3)
{
    meshData.GetIndices32()[indexStart] = index1;
    meshData.GetIndices32()[indexStart + 1] = index2;
    meshData.GetIndices32()[indexStart + 2] = index3;
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

void Sego::MeshGeometry::CreateQuad(FMeshData &meshData, float centerX, float centerY, float width, float height, float depth, bool flipV)
{
        //  0       3
        //   --------
        //   |\     |
        //   |  \   |
        //   |    \ |
        //   --------
        //  1        2     左手坐标系索引为：（0 3 2），（2 1 0）
        //                  右手为 (0 1 2)，(2 3 0)

        //FMeshVertex
        AddVertex(meshData, FMeshVertex(centerX - width/2, centerY + height/2, depth,
                                        0.0f, 0.0f, -1.0f,
                                        1.0f, 0.0f,  0.0f,
                                        0.0f, flipV ? 1.0f : 0.0f)); //顶点0

        AddVertex(meshData, FMeshVertex(centerX - width/2, centerY - height/2, depth,
                                        0.0f, 0.0f, -1.0f,
                                        1.0f, 0.0f,  0.0f,
                                        0.0f, flipV ? 0.0f : 1.0f)); //顶点1

        AddVertex(meshData, FMeshVertex(centerX + width/2, centerY - height/2, depth,
                                        0.0f, 0.0f, -1.0f,
                                        1.0f, 0.0f,  0.0f,
                                        1.0f, flipV ? 0.0f : 1.0f)); //顶点2

        AddVertex(meshData, FMeshVertex(centerX + width/2, centerY + height/2, depth,
                                        0.0f, 0.0f, -1.0f,
                                        1.0f, 0.0f,  0.0f,
                                        1.0f, flipV ? 1.0f : 0.0f)); //顶点3

        //Index
        
            //右手坐标系索引
            AddIndexTriangle(meshData, 0, 1, 2);
            AddIndexTriangle(meshData, 2, 3, 0);
        
}

void Sego::MeshGeometry::CreateGrid(FMeshData &meshData, 
float width, float height, uint32 m, uint32 n, bool flipV)
{
        // (n-1)*(m-2)                  (n-1)*(m-1)
        //      |
        //      |
        //      |
        //  n*2 ----------------        (n-1)*3
        //      |\     |\     |
        //      |  \   |  \   |
        //      |    \ |    \ |
        //  n   ----------------        (n-1)*2
        //      |\     |\(n+1)|(n+2)
        //      |  \   |  \   |
        //      |    \ |    \ |
        //      ------------------------
        //  0          1      2         (n-1)   左手坐标系索引为：（(i + 1) * n + j，(i + 1) * n + j + 1，i * n + j + 1） ，（i * n + j + 1，i * n + j，(i + 1) * n + j）

        uint32 vertexCount = m * n;
        uint32 faceCount = (m - 1) * (n - 1) * 2;

        //FMeshVertex
        float halfW = 0.5f * width;
        float halfH = 0.5f * height;

        float dx = width / (n - 1);
        float dz = height / (m - 1);

        float du = 1.0f / (n - 1);
        float dv = 1.0f / (m - 1);

        ResizeVertexCount(meshData, vertexCount);
        for (uint32 i = 0; i < m; ++i)
        {
            float y = -halfH + i * dz;
            for (uint32 j = 0; j < n; ++j)
            {
                float x = -halfW + j * dx;

                FMeshVertex vertex(   x,    y,  0.0f,
                                   0.0f, 0.0f, -1.0f,
                                   1.0f, 0.0f,  0.0f,
                                   j * du, flipV ? (1.0f - i * dv) : (i * dv));
                SetVertex(meshData, i * n + j, vertex);
            }
        }
        //meshData.RefreshAABB();

        //Index
        ResizeIndexCount(meshData, faceCount * 3);
        uint32 k = 0;
        for (uint32 i = 0; i < m - 1; ++i)
        {
            for (uint32 j = 0; j < n - 1; ++j)
            {
               
                
                    //右手坐标系索引
                    SetIndexTriangle(meshData, k + 0,
                                     (i + 1) * n + j,
                                     i * n + j,
                                     i * n + j + 1);

                    SetIndexTriangle(meshData, k + 3,
                                     i * n + j + 1,
                                     (i + 1) * n + j + 1,
                                     (i + 1) * n + j);
                k += 6;
            }
        }










}
//-------------- 圆形 -----------------
// 顶点：顶点数量：中心点+段数（segment + 1），中心点为（0，0，0），法线朝向Z-轴
// 索引：三角形数量：段数（segment），索引数量：segment*3
void Sego::MeshGeometry::CreateCircle(FMeshData &meshData, float radius, uint32 segment, bool flipV)
{
     //          *  *
    //      *          * 2
    //
    //    *	     * 0     * 1
    //			
	//       *         * segment    左手坐标系索引为：非最后一个顶点时： (0,i+2,i+1)
	//          *   *                                最后一个顶点时： (0,1,i)

    uint32 vertexCount = segment + 1;
    uint32 faceCount = segment;

    //FMeshVertex
    float thetaStep = 2.0f * SegoPi / segment;
    ResizeVertexCount(meshData, vertexCount);
    SetVertex(meshData, 
                0, 
                FMeshVertex(0.0f, 0.0f,  0.0f,
                            0.0f, 0.0f, -1.0f,
                            1.0f, 0.0f,  0.0f,
                            0.5f, 0.5f));
    int index = 1;
    for (uint32 i = 0; i < segment; i++)
    {
        float ux = cos(thetaStep * i);
        float uy = sin(thetaStep * i);

        SetVertex(meshData, 
                    index,
                    FMeshVertex(radius * ux,  radius * uy, 0.0f,
                                0.0f, 0.0f, -1.0f,
                                1.0f, 0.0f,  0.0f,
                                (1.0f + ux) / 2.0f,  flipV ? (1.0f - (1.0f - uy) / 2.0f) : (1.0f - uy) / 2.0f));
        index++;
    }

    //meshData.RefreshAABB();

    //Index
    ResizeIndexCount(meshData, faceCount * 3);
    for (uint32 i = 0; i < segment; ++i)    
        {
            if (i != segment - 1)
            {
                //右手坐标系索引
                    SetIndexTriangle(meshData, i*3 + 0,
                                     0,
                                     i + 1,
                                     i + 2);
            }
            else
            {
                //右手坐标系索引
                    SetIndexTriangle(meshData, i*3 + 0,
                                     0,
                                     i,
                                     1);
            }
        }





}

void Sego::MeshGeometry::CreateAABB(FMeshData &meshData, float width, float height, float depth, uint32 numSubdivisions, bool flipV)
{
    //     7+------+4	          0 -  9 - 19  -+-
	//     /|     /|	          1 - 12 - 18  ---
	//    / |    / |		      2 - 15 - 21  +--
	//   / 6+---/--+5	 y        3 - 10 - 20  ++-
	// 0+------+3 /		 | -z	  4 - 11 - 23  +++
	//  | /    | /    	 |/       5 - 14 - 22  +-+
	//  |/     |/     	 *---x	  6 - 13 - 17  --+
	// 1+------+2        		  7 -  8 - 16  -++


    //MeshVertex
    FMeshVertex v[24];
    float w2 = 0.5f * width;
    float h2 = 0.5f * height;
    float d2 = 0.5f * depth;

    //Front
    v[0]  = FMeshVertex(-w2, +h2, -d2,   0.0f,   0.0f,  -1.0f,   1.0f,  0.0f,   0.0f,   0.0f, flipV ? 1.0f : 0.0f); //0 -+-
    v[1]  = FMeshVertex(-w2, -h2, -d2,   0.0f,   0.0f,  -1.0f,   1.0f,  0.0f,   0.0f,   0.0f, flipV ? 0.0f : 1.0f); //1 ---
    v[2]  = FMeshVertex(+w2, -h2, -d2,   0.0f,   0.0f,  -1.0f,   1.0f,  0.0f,   0.0f,   1.0f, flipV ? 0.0f : 1.0f); //2 +--
    v[3]  = FMeshVertex(+w2, +h2, -d2,   0.0f,   0.0f,  -1.0f,   1.0f,  0.0f,   0.0f,   1.0f, flipV ? 1.0f : 0.0f); //3 ++-
    //Back
    v[4]  = FMeshVertex(+w2, +h2, +d2,   0.0f,   0.0f,   1.0f,  -1.0f,  0.0f,   0.0f,   0.0f, flipV ? 1.0f : 0.0f); //4 +++
    v[5]  = FMeshVertex(+w2, -h2, +d2,   0.0f,   0.0f,   1.0f,  -1.0f,  0.0f,   0.0f,   0.0f, flipV ? 0.0f : 1.0f); //5 +-+
    v[6]  = FMeshVertex(-w2, -h2, +d2,   0.0f,   0.0f,   1.0f,  -1.0f,  0.0f,   0.0f,   1.0f, flipV ? 0.0f : 1.0f); //6 --+
    v[7]  = FMeshVertex(-w2, +h2, +d2,   0.0f,   0.0f,   1.0f,  -1.0f,  0.0f,   0.0f,   1.0f, flipV ? 1.0f : 0.0f); //7 -++
    //Top
    v[8]  = FMeshVertex(-w2, +h2, +d2,   0.0f,   1.0f,   0.0f,   1.0f,  0.0f,   0.0f,   0.0f, flipV ? 1.0f : 0.0f); //8  - 7
    v[9]  = FMeshVertex(-w2, +h2, -d2,   0.0f,   1.0f,   0.0f,   1.0f,  0.0f,   0.0f,   0.0f, flipV ? 0.0f : 1.0f); //9  - 0
    v[10] = FMeshVertex(+w2, +h2, -d2,   0.0f,   1.0f,   0.0f,   1.0f,  0.0f,   0.0f,   1.0f, flipV ? 0.0f : 1.0f); //10 - 3
    v[11] = FMeshVertex(+w2, +h2, +d2,   0.0f,   1.0f,   0.0f,   1.0f,  0.0f,   0.0f,   1.0f, flipV ? 1.0f : 0.0f); //11 - 4
    //Bottom
    v[12] = FMeshVertex(-w2, -h2, -d2,   0.0f,  -1.0f,   0.0f,   1.0f,  0.0f,   0.0f,   0.0f, flipV ? 1.0f : 0.0f); //12 - 1
    v[13] = FMeshVertex(-w2, -h2, +d2,   0.0f,  -1.0f,   0.0f,   1.0f,  0.0f,   0.0f,   0.0f, flipV ? 0.0f : 1.0f); //13 - 6
    v[14] = FMeshVertex(+w2, -h2, +d2,   0.0f,  -1.0f,   0.0f,   1.0f,  0.0f,   0.0f,   1.0f, flipV ? 0.0f : 1.0f); //14 - 5
    v[15] = FMeshVertex(+w2, -h2, -d2,   0.0f,  -1.0f,   0.0f,   1.0f,  0.0f,   0.0f,   1.0f, flipV ? 1.0f : 0.0f); //15 - 2
    //Left
    v[16] = FMeshVertex(-w2, +h2, +d2,  -1.0f,   0.0f,   0.0f,   0.0f,  0.0f,  -1.0f,   0.0f, flipV ? 1.0f : 0.0f); //16 - 7
    v[17] = FMeshVertex(-w2, -h2, +d2,  -1.0f,   0.0f,   0.0f,   0.0f,  0.0f,  -1.0f,   0.0f, flipV ? 0.0f : 1.0f); //17 - 6
    v[18] = FMeshVertex(-w2, -h2, -d2,  -1.0f,   0.0f,   0.0f,   0.0f,  0.0f,  -1.0f,   1.0f, flipV ? 0.0f : 1.0f); //18 - 1
    v[19] = FMeshVertex(-w2, +h2, -d2,  -1.0f,   0.0f,   0.0f,   0.0f,  0.0f,  -1.0f,   1.0f, flipV ? 1.0f : 0.0f); //19 - 0
    //Right
    v[20] = FMeshVertex(+w2, +h2, -d2,   1.0f,   0.0f,   0.0f,   0.0f,  0.0f,   1.0f,   0.0f, flipV ? 1.0f : 0.0f); //20 - 3
    v[21] = FMeshVertex(+w2, -h2, -d2,   1.0f,   0.0f,   0.0f,   0.0f,  0.0f,   1.0f,   0.0f, flipV ? 0.0f : 1.0f); //21 - 2
    v[22] = FMeshVertex(+w2, -h2, +d2,   1.0f,   0.0f,   0.0f,   0.0f,  0.0f,   1.0f,   1.0f, flipV ? 0.0f : 1.0f); //22 - 5
    v[23] = FMeshVertex(+w2, +h2, +d2,   1.0f,   0.0f,   0.0f,   0.0f,  0.0f,   1.0f,   1.0f, flipV ? 1.0f : 0.0f); //23 - 4

    for(int i = 0; i < 24; i++)
    {
        AddVertex(meshData, v[i]);
    }
    //Index
    uint32 i[36];
    //右手坐标系索引
    //Front 
    i[0]  = 0;  i[1]  = 1;  i[2]  = 2;
    i[3]  = 2;  i[4]  = 3;  i[5]  = 0;
    //Back
    i[6]  = 4;  i[7]  = 5;  i[8]  = 6;
    i[9]  = 6;  i[10] = 7;  i[11] = 4;
    //Top
    i[12] = 8;  i[13] = 9;  i[14] = 10;
    i[15] = 10; i[16] = 11; i[17] = 8;
    //Bottom
    i[18] = 12; i[19] = 13; i[20] = 14;
    i[21] = 14; i[22] = 15; i[23] = 12;
    //Left
    i[24] = 16; i[25] = 17; i[26] = 18;
    i[27] = 18; i[28] = 19; i[29] = 16;
    //Right
    i[30] = 20; i[31] = 21; i[32] = 22;
    i[33] = 22; i[34] = 23; i[35] = 20;

}
//--------------- 球 ------------------
// 顶点：顶点数量：2+(stackCount-1)*(sliceCount+1),球心为原点（0,0,0)
// 索引：三角形数量：sliceCount*2+(stackCount-2)*sliceCount*2，索引数量：(sliceCount*2+(stackCount-2)*sliceCount*2)*3
// 举个例子：stackCount=30，sliceCount=30，
// 则顶点: 2+(30-1)*(30+1)=901, 三角形:30*2+(30-2)*30*2=1740, 索引:1740*3=5220
//--------------- 球 ------------------
void Sego::MeshGeometry::CreateSphere(FMeshData &meshData, 
float radius, uint32 sliceCount, uint32 stackCount, bool flipV)
{
    //           0 
    //           *   
    //	*    - 2  *  1          左手坐标系索引为：Top顶点时: (0,i+1,i)
    //        -     -          y                   中间顶点时: 两条经线围成的四边形分解为两个三角形
    //   *	     *       *     | z              Bottom顶点时: (l,l-1,l-2)
    //	  -     - l-2      |/
    //      *    -    *  l-1   *---x
    //           *
    //           l=(stackCount-1)*(sliceCount+1)+1

//FMeshVertex
        FMeshVertex topVertex(0.0f, +radius, 0.0f,  0.0f, +1.0f, 0.0f,  1.0f, 0.0f, 0.0f,  0.0f, flipV ? 1.0f : 0.0f);
        AddVertex(meshData, topVertex);
        float phiStep = SegoPi / stackCount; //纬度(0-180)
        float thetaStep = 2.0f * SegoPi / sliceCount; //经度(0-360)
        for (uint32 i = 1; i <= stackCount - 1; ++i) //从1开始是因为除去了Top顶点
        {
            //纬度
            float phi = i * phiStep;
            for (uint32 j = 0; j <= sliceCount; ++j)
            {
                //经度
                float theta = j * thetaStep;

                FMeshVertex v;
                //pos
                v.pos.x = radius * sinf(phi) * cosf(theta);
                v.pos.y = radius;/// * cosf(phi);
                v.pos.z = radius * sinf(phi) * sinf(theta);
                //color
                v.color = FVector4(1.0f, 1.0f, 1.0f, 1.0f);
                //normal
                v.normal = glm::normalize(v.pos);
                //tangent
                v.tangent.x = -radius * sinf(phi) * sinf(theta);
                v.tangent.y = 0.0f;
                v.tangent.z = +radius * sinf(phi) * cosf(theta);
                v.tangent = glm::normalize(v.tangent);
                //texCoord
                v.texCoord.x = theta /SegoPi_TWO;
                v.texCoord.y = flipV ? (1.0f - phi / SegoPi) : (phi / SegoPi);

                AddVertex(meshData, v);
            }
        }
        FMeshVertex bottomVertex(0.0f, -radius, 0.0f,  0.0f, -1.0f, 0.0f,  1.0f, 0.0f, 0.0f,  0.0f, flipV ? 0.0f : 1.0f);
        AddVertex(meshData, bottomVertex);
    //Index Top
    for (uint32 i = 1; i <= sliceCount; ++i)
    {
        //右手坐标系索引
                AddIndexTriangle(meshData, 
                                 0,
                                 i,
                                 i + 1);      
    }

    //Index Inner
        uint32 baseIndex = 1;
        uint32 ringVertexCount = sliceCount + 1;
        for (uint32 i = 0; i < stackCount - 2; ++i)
        {
            for (uint32 j = 0; j < sliceCount; ++j)
            {
                //右手坐标系索引
                AddIndexTriangle(meshData, 
                                    baseIndex + i * ringVertexCount + j,
                                    baseIndex + (i + 1) * ringVertexCount + j,
                                    baseIndex + (i + 1) * ringVertexCount + j + 1);

                AddIndexTriangle(meshData, 
                                    baseIndex + (i + 1) * ringVertexCount + j + 1,
                                    baseIndex + i * ringVertexCount + j + 1,
                                    baseIndex + i * ringVertexCount + j);
            }

        }

    //Index Bottom
        uint32 southPoleIndex = GetVertexCount(meshData) - 1;
        baseIndex = southPoleIndex - ringVertexCount;
        for (uint32 i = 0; i < sliceCount; ++i)
        {
            //右手坐标系索引
            AddIndexTriangle(meshData, 
                                southPoleIndex,
                                baseIndex + i + 1,
                                baseIndex + i);
        }
}
//--------------- 几何球体 --------------
// 顶点：未细分顶点数量：12,中心位置为（0,0,0）
// 索引：未细分索引数量：60
//--------------- 几何球体 --------------
void Sego::MeshGeometry::CreateGeosphere(FMeshData &meshData, float radius, uint32 numSubdivisions, bool flipV)
{
  
{
    //             * 4    
	//	     * 5 	    
	//         * 0  * 1           
    //    * 9              * 8    y
	//  * 11	     * 10     | z
	//	   * 2	* 3           |/
	//             * 6            *---x
	//           * 7    

        //FMeshVertex
        numSubdivisions = std::min<uint32>(numSubdivisions, 6u);
        const float X = 0.525731f;
        const float Z = 0.850651f;
        FVector3 pos[12] =
        {
            FVector3(   -X,  0.0f,     Z),  //0
            FVector3(    X,  0.0f,     Z),  //1
            FVector3(   -X,  0.0f,    -Z),  //2
            FVector3(    X,  0.0f,    -Z),  //3
            FVector3( 0.0f,     Z,     X),  //4
            FVector3( 0.0f,     Z,    -X),  //5
            FVector3( 0.0f,    -Z,     X),  //6
            FVector3( 0.0f,    -Z,    -X),  //7
            FVector3(    Z,     X,  0.0f),  //8
            FVector3(   -Z,     X,  0.0f),  //9
            FVector3(    Z,    -X,  0.0f),  //10
            FVector3(   -Z,    -X,  0.0f)   //11
        };

        //Vertex
        ResizeVertexCount(meshData, 12);
        for (uint32 i = 0; i < 12; ++i)
        {
            FMeshVertex& vertex = GetVertex(meshData, i);
            vertex.pos = pos[i];
        }

        //Index
       
            //右手坐标系索引
            uint32 k[60] =
            {
                1,  4,  0,    4, 9, 0,   4,  5, 9,   8, 5,  4,   1, 8, 4,
                1, 10,  8,   10, 3, 8,   8,  3, 5,   3, 2,  5,   3, 7, 2,
                3, 10,  7,   10, 6, 7,   6, 11, 7,   6, 0, 11,   6, 1, 0,
               10,  1,  6,   11, 0, 9,   2, 11, 9,   5, 2,  9,  11, 2, 7
            };
            AddIndices(meshData, 60, k);
      

        //SubDivide
        for (uint32 i = 0; i < numSubdivisions; ++i)
        {
            SubDivide(meshData);
        }

        uint32 countVertex = GetVertexCount(meshData);
        for (uint32 i = 0; i < countVertex; ++i)
        {
            FMeshVertex& vertex = GetVertex(meshData, i);

            vertex.normal = glm::normalize(vertex.pos);
            vertex.pos = radius * vertex.normal;

            float theta = atan2f(vertex.pos.z, vertex.pos.x);
            if (theta < 0.0f)
                theta += SegoPi_TWO;

            float phi = acosf(vertex.pos.y / radius);

            vertex.texCoord.x = theta / SegoPi_TWO;
            vertex.texCoord.y = flipV ? (1.0f - phi / SegoPi) : (phi / SegoPi);

            vertex.tangent.x = -radius * sinf(phi) * sinf(theta);
            vertex.tangent.y = 0.0f;
            vertex.tangent.z = +radius * sinf(phi) * cosf(theta);
            vertex.tangent = glm::normalize(vertex.tangent);
        }
        //meshData.RefreshAABB();
    }

}

void Sego::MeshGeometry::CreateCylinder(FMeshData &meshData, 
float bottomRadius, float topRadius, float height, float heightOffset, 
uint32 sliceCount, uint32 stackCount, bool flipV)
{
    //       * 
    //    *     *
    //    *  *  *
    //    *     *
    //    *     *
    //    *     *
    //    *  *  *
    //    *     *
    //       *  

    //FMeshVertex
    float stackHeight = height / stackCount;
    float radiusStep = (topRadius - bottomRadius) / stackCount;
    uint32 ringCount = stackCount + 1;
    for (uint32 i = 0; i < ringCount; ++i)
    {
        float y = heightOffset - 0.5f * height + i * stackHeight;
        float r = bottomRadius + i * radiusStep;

        float dTheta = 2.0f * SegoPi / sliceCount;
        for (uint32 j = 0; j <= sliceCount; ++j)
        {
            FMeshVertex vertex;

            float c = cosf(j * dTheta);
            float s = sinf(j * dTheta);

            vertex.pos = FVector3(r * c, y, r * s);
            vertex.color = FVector4(1.0f, 1.0f, 1.0f, 1.0f);

            vertex.texCoord.x = (float)j / sliceCount;
            vertex.texCoord.y = flipV ? ((float)i / stackCount) : (1.0f - (float)i / stackCount);

            vertex.tangent = FVector3(-s, 0.0f, c);

            float dr = bottomRadius - topRadius;
            FVector3 T = vertex.tangent;
            FVector3 B = FVector3(dr * c, -height, dr * s);
            vertex.normal = glm::normalize(glm::cross(T, B));

            AddVertex(meshData, vertex);
        }
    }

    uint32 ringVertexCount = sliceCount + 1;

    for (uint32 i = 0; i < stackCount; ++i)
    {
        for (uint32 j = 0; j < sliceCount; ++j)
        {
           
            //右手坐标系索引
            AddIndexTriangle(meshData, 
                                i * ringVertexCount + j,
                                (i + 1) * ringVertexCount + j + 1,
                                (i + 1) * ringVertexCount + j);

            AddIndexTriangle(meshData, 
                                i * ringVertexCount + j,
                                i * ringVertexCount + j + 1,
                                (i + 1) * ringVertexCount + j + 1);
        
           
        }
    }

        s_BuildCylinderTopCap(meshData, 
                              bottomRadius, 
                              topRadius, 
                              height, 
                              heightOffset,
                              sliceCount, 
                              stackCount, 
                              flipV
                              );
        s_BuildCylinderBottomCap(meshData, 
                                 bottomRadius, 
                                 topRadius, 
                                 height, 
                                 heightOffset,
                                 sliceCount, 
                                 stackCount, 
                                 flipV 
                                 );
    
    
}


void Sego::s_BuildCylinderTopCap(FMeshData &meshData, 
float bottomRadius, float topRadius, float height, 
float heightOffset, uint32 sliceCount, uint32 stackCount, bool flipV)
{
    uint32 baseIndex = MeshGeometry::GetVertexCount(meshData);
        float y = heightOffset + 0.5f * height;
        float dTheta = 2.0f * SegoPi / sliceCount;
        for (uint32 i = 0; i <= sliceCount; ++i)
        {
            float x = topRadius * cosf(i * dTheta);
            float z = topRadius * sinf(i * dTheta);

            float u = x / height + 0.5f;
            float v = flipV ? (1.0f - z / height + 0.5f) : (z / height + 0.5f);

            MeshGeometry::AddVertex(meshData, FMeshVertex(x, y, z, 
                                                           0.0f, 1.0f, 0.0f, 
                                                           1.0f, 0.0f, 0.0f, 
                                                           u, v));
        }
        MeshGeometry::AddVertex(meshData, FMeshVertex(0.0f, y, 0.0f, 
                                                       0.0f, 1.0f, 0.0f, 
                                                       1.0f, 0.0f, 0.0f, 
                                                       0.5f, 0.5f));

        uint32 centerIndex = MeshGeometry::GetVertexCount(meshData) - 1;
        for (uint32 i = 0; i < sliceCount; ++i)
        {
           
                //右手坐标系索引
                MeshGeometry::AddIndexTriangle(meshData, 
                                                centerIndex,
                                                baseIndex + i,
                                                baseIndex + i + 1);
         
        }
}

void Sego::s_BuildCylinderBottomCap(FMeshData &meshData, float bottomRadius, float topRadius, float height, float heightOffset, uint32 sliceCount, uint32 stackCount, bool flipV)
{

uint32 baseIndex = MeshGeometry::GetVertexCount(meshData);
        float y = heightOffset - 0.5f * height;
        float dTheta = 2.0f * SegoPi / sliceCount;
        for (uint32 i = 0; i <= sliceCount; ++i)
        {
            float x = bottomRadius * cosf(i * dTheta);
            float z = bottomRadius * sinf(i * dTheta);

            float u = x / height + 0.5f;
            float v = flipV ? (1.0f - z / height + 0.5f) : (z / height + 0.5f);

            MeshGeometry::AddVertex(meshData, FMeshVertex(x, y, z, 
                                                           0.0f, -1.0f, 0.0f, 
                                                           1.0f, 0.0f, 0.0f, 
                                                           u, v));
        }
        MeshGeometry::AddVertex(meshData, FMeshVertex(0.0f, y, 0.0f, 
                                                       0.0f, -1.0f, 0.0f, 
                                                       1.0f,  0.0f, 0.0f, 
                                                       0.5f,  0.5f));

        uint32 centerIndex = MeshGeometry::GetVertexCount(meshData) - 1;
        for (uint32 i = 0; i < sliceCount; ++i)
        {
            
                //右手坐标系索引
            MeshGeometry::AddIndexTriangle(meshData, 
                                                centerIndex,
                                                baseIndex + i + 1,
                                                baseIndex + i);
        }
}


void Sego::MeshGeometry::CreateCapsule(FMeshData &meshData, 
float radius, float height, float heightOffset, uint32 numRings, 
uint32 numSegments, uint32 numSegHeight, bool flipV)
{
    //      *** 
        //    *  *  *
        //    *  *  *
        //    *     *
        //    *     *
        //    *     *
        //    *  *  *
        //    *  *  *
        //      ***  

        //FMeshVertex
        uint32 nVertexCount = (2 * numRings + 2) * (numSegments + 1) + (numSegHeight - 1) * (numSegments + 1);
        uint32 nIndexCount = (2 * numRings + 1) * (numSegments + 1) * 6 + (numSegHeight - 1) * (numSegments + 1) * 6;
        ReserveVertexCount(meshData, nVertexCount);
        ReserveIndexCount(meshData, nIndexCount);

        float fDeltaRingAngle = (SegoPi_Half / numRings);
        float fDeltaSegAngle = (SegoPi_TWO / numSegments);

        float sphereRatio = radius / (2 * radius + height);
        float cylinderRatio = height / (2 * radius + height);
        int offset = 0;

        //1> Top half sphere
        for (uint32 ring = 0; ring <= numRings; ring++)
        {
            float r0 = radius * sinf(ring * fDeltaRingAngle);
            float y0 = radius * cosf(ring * fDeltaRingAngle);

            for (uint32 seg = 0; seg <= numSegments; seg++)
            {
                float x0 = r0 * cosf(seg * fDeltaSegAngle);
                float z0 = r0 * sinf(seg * fDeltaSegAngle);
                FVector3 pos(x0, heightOffset + 0.5f * height + y0, z0);
                FVector3 normal = glm::normalize(FVector3(x0, y0, z0));
                FVector3 tangent = glm::normalize(FVector3(-z0, y0, x0));
                float u = (float)seg / (float)numSegments;
                float v = flipV ? (1.0f - (float)ring / (float)numRings * sphereRatio) : (float)ring / (float)numRings * sphereRatio; 

                AddVertex(meshData, 
                          pos,
                          FVector4(1.0f, 1.0f, 1.0f, 1.0f),
                          normal,
                          tangent,
                          FVector2(u, v));

                
                    //右手坐标系索引
                    AddIndexTriangle(meshData, offset + numSegments + 1, offset, offset + numSegments);
                    AddIndexTriangle(meshData, offset + numSegments + 1, offset + 1, offset);

                offset ++;
            }
        }

        //2> Cylinder part
        float deltaAngle = (SegoPi_TWO/ numSegments);
        float deltamHeight = height/(float)numSegHeight;
        for (uint32 i = 1; i < numSegHeight; i++)
        {
            for (uint32 j = 0; j<=numSegments; j++)
            {
                float x0 = radius * cosf(j * deltaAngle);
                float z0 = radius * sinf(j * deltaAngle);
                FVector3 pos(x0, heightOffset + 0.5f * height - i * deltamHeight, z0);
                FVector3 normal = glm::normalize(FVector3(x0, 0, z0));
                FVector3 tangent = glm::normalize(FVector3(-z0, 0, x0));
                float u = j / (float)numSegments;
                float v = flipV ? (1.0f - i / (float)numSegHeight * cylinderRatio + sphereRatio) : (i / (float)numSegHeight * cylinderRatio + sphereRatio); 

                AddVertex(meshData, 
                          pos,
                          FVector4(1.0f, 1.0f, 1.0f, 1.0f),
                          normal,
                          tangent,
                          FVector2(u, v));

               
                    //右手坐标系索引
                    AddIndexTriangle(meshData, offset + numSegments + 1, offset, offset + numSegments);
                    AddIndexTriangle(meshData, offset + numSegments + 1, offset + 1, offset);
                
                offset ++;
            }
        }

        //3> Bottom half sphere
        for (uint32 ring = 0; ring <= numRings; ring++)
        {
            float r0 = radius * sinf (SegoPi_Half + ring * fDeltaRingAngle);
            float y0 =  radius * cosf (SegoPi_Half + ring * fDeltaRingAngle);

            for (uint32 seg = 0; seg <= numSegments; seg++)
            {
                float x0 = r0 * cosf(seg * fDeltaSegAngle);
                float z0 = r0 * sinf(seg * fDeltaSegAngle);
                FVector3 pos(x0, heightOffset - 0.5f * height + y0, z0);
                FVector3 normal = glm::normalize(FVector3(x0, y0, z0));
                FVector3 tangent = glm::normalize(FVector3(-z0, y0, x0));
                float u = (float)seg / (float)numSegments;
                float v = flipV ? (1.0f - ((float)ring / (float)numRings * sphereRatio + cylinderRatio + sphereRatio)) : ((float)ring / (float)numRings * sphereRatio + cylinderRatio + sphereRatio); 

                AddVertex(meshData, 
                          pos,
                          FVector4(1.0f, 1.0f, 1.0f, 1.0f),
                          normal,
                          tangent,
                          FVector2(u, v));

                if (ring != numRings)
                {
                    
                        //右手坐标系索引
                        AddIndexTriangle(meshData, offset + numSegments + 1, offset, offset + numSegments);
                        AddIndexTriangle(meshData, offset + numSegments + 1, offset + 1, offset);
                }
                offset ++;
            } 
        } 
}

void Sego::MeshGeometry::CreateTorus(FMeshData &meshData, float radius, float sectionRadius, uint32 numSegSection, uint32 numSegCircle, bool flipV)
{
    //         + +
    //       + * *  +  
    //     + * - -  *  +
    //    + * -    - * +
    //     + * - -  *  + 
    //       + * *  +
    //         +  +

    //FMeshVertex
    uint32 nVertexCount = (numSegCircle + 1) * (numSegSection + 1);
    uint32 nIndexCount = (numSegCircle) * (numSegSection + 1) * 6;
    ReserveVertexCount(meshData, nVertexCount);
    ReserveIndexCount(meshData, nIndexCount);

    float deltaSection = (SegoPi_TWO / numSegSection);
    float deltaCircle = (SegoPi_TWO / numSegCircle);
    int offset = 0;

    for (uint32 i = 0; i <= numSegCircle; i++)
    {   
        for (uint32 j = 0; j<= numSegSection; j++)
        {
            FVector3 c0(radius, 0.0, 0.0);
            FVector3 v0(radius + sectionRadius * cosf(j * deltaSection), sectionRadius * sinf(j * deltaSection), 0.0);
            FQuaternion qRot = glm::angleAxis(i * deltaCircle, FVector3(0.0f, 1.0f, 0.0f));
            FVector3 vPos = Transform(qRot, v0);
            FVector3 c = Transform(qRot, c0);
            float u = i / (float)numSegCircle;
            float v = flipV ? (1.0f - j / (float)numSegSection) : (j / (float)numSegSection);

            AddVertex(meshData, 
                        vPos,
                        FVector4(1.0f, 1.0f, 1.0f, 1.0f),
                        glm::normalize(vPos - c),
                        FVector3(1.0f, 0.0f, 0.0f),
                        FVector2(u, v));

            if (i != numSegCircle)
            {
                
                    //右手坐标系索引
                    AddIndexTriangle(meshData, offset + numSegSection + 1, offset + numSegSection, offset);
                    AddIndexTriangle(meshData, offset + numSegSection + 1, offset, offset + 1);
                
                
            }
            offset ++;
        }
    } 
}
