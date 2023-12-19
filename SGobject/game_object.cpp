#include "game_object.h"

Component* GameObject::AddComponent(std::string component_type_name){
    type t = type::get_by_name(component_type_name.c_str());
    variant var = t.create();//创建实例
    Component* component=var.get_value<Component*>();
    component->set_game_object(this);


    return component;
}




