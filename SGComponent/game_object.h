#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include <rttr/registration>
using namespace rttr;
class Component;

class GameObject
{
public:
    GameObject();
    GameObject(std::string name);
    ~GameObject();

    std::string& name() {return name_;}
    void set_name(std::string name) {name_ = name;}

/// 添加组件
/// param component_type_name 组件类名
/// return
Component* AddComponent(std::string component_type_name);

/// 获取组件
/// param component_type_name 组件类名
/// return
Component* GetComponent(std::string component_type_name);

std::vector<Component*> GetComponents(std::string component_type_name);

std::string name_;
std::unordered_map<std::string,std::vector<Component*>> component_type_instance_map_;
};









