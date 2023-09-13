#include <core.h>

class Layer
{
public:
    virtual void OnAttach() = 0;
    virtual void OnDeatch() = 0;
    virtual void OnUpdate() {}
    virtual void OnRenderer() {}
    
    Layer(const std::string& name = "Layer");
    virtual ~Layer();

    inline const std::string& GetName() const { return Layer_name;}
private:
    std::string Layer_name;
};