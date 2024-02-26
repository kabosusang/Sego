#include "SceneReGird.hpp"

namespace Sego
{
    SceneReGird::SceneReGird()
    {
        MeshGeometry::CreateGrid(this -> scene_gridmesh,1000, 1000, 10, 10, 90);
    }

    SceneReGird::~SceneReGird()
    {

    }


}

