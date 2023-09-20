#include <core.h>

class Layer
{
public:
    virtual void OnAttach() = 0;
    virtual void OnDeatch() = 0;
    virtual void OnUpdate() = 0;
    virtual void OnRenderer() =0;
        
    Layer(const std::string& name = "Layer");
    virtual ~Layer();

    inline const std::string& GetName() const { return Layer_name;}
private:
    std::string Layer_name;
};