#include <unordered_map>
#include <string>
#include <vector>
#include <rttr/registration>
using namespace rttr;



#include "component.h"

class GameObject{

/// 添加组件
/// param component_type_name 组件类名
/// return

Component* AddComponent(std::string component_type_name);

/// 获取组件
/// param component_type_name 组件类名
/// return
Component* GetComponent(std::string component_type_name);



};

std::unordered_map<std::string,std::vector<Component*>> component_type_instance_map_;








