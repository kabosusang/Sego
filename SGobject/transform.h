#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "SGobject/component.h"

class Transform : public Component
{
public:
    Transform();
    ~Transform();

    glm::vec3 position() const {return position_;}
    glm::vec3 rotation() const {return rotation_;}
    glm::vec3 scale() const {return scale_;}

    void set_position(glm::vec3 position) {position_ = position;}
    void set_rotation(glm::vec3 rotation) {rotation_ = rotation;}
    void set_scale(glm::vec3 scale) {scale_ = scale;}

private:
    glm::vec3 position_;
    glm::vec3 rotation_;
    glm::vec3 scale_;
};







