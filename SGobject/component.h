
//所有组件的基类
class Component {
public:
    Component();
    ~Component();
    GameObject* game_object(){return game_object_;}
    void set_game_object(GameObject* game_object){game_object_=game_object;}
private:
    GameObject* game_object_;
};