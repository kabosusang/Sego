#pragma once
#include "SGComponent/transform.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

class SG_Object : public Component
{
public:
inline glm::mat4& Model_mat4() { return Model_mat4_;}

void ChangeModelPosition();

private:
    glm::mat4 Model_mat4_;

};

extern std::vector<Transform*> transform_obj;
extern std::vector<SG_Object*> sg_obj;


