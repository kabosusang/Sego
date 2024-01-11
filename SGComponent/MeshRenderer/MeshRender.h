#include "SGComponent/component.h"
#include <glm/glm.hpp>


class Material;
class MeshFilter;
class Texture2D;

class MeshRenderer: public Component
{
public:
MeshRenderer();
~MeshRenderer();

void SetMaterial(Material* material); //设置material
Material* material(){return material_;}

void SetView(glm::mat4 view){view_ = view;};
void SetProjection(glm::mat4 projection){projection_=projection;};

void Render(); //渲染


private:
Material* material_;

glm::mat4 view_;
glm::mat4 projection_;

};