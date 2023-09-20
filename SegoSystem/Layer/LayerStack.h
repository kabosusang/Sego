#pragma once

#include "Layer.h"
#include "vector"

class LayerStack 
{
private:
    LayerStack() : size(0) {}

    ~LayerStack() { Clear(); }

public:

    void Push(Layer* layer) { LayerVector.emplace_back(layer); ++size;}
    void Pop() { if (!LayerVector.empty()) LayerVector.pop_back(); --size; }
    void Clear() { LayerVector.clear(); }
    
    std::vector<Layer*>::iterator begin() {return LayerVector.begin();}
    std::vector<Layer*>::iterator end() {return LayerVector.end();}

    static LayerStack* GetInstance() {
        return s_LayerStackInsatnce.get();
    }

private:
    int size;
    std::vector<Layer*> LayerVector;

private:
    static std::unique_ptr<LayerStack> s_LayerStackInsatnce;
};


