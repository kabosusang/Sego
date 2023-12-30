#pragma once
class GameObject;

class Component {
public:
    Component();
    virtual ~Component();

    GameObject* game_object() const {return game_object_;}
    void set_game_object(GameObject* game_object){game_object_=game_object;}
private:
    GameObject* game_object_;
};