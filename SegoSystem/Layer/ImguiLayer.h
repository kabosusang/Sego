#pragma once

#include "Layer.h"

class Sego_API ImguiLayer : public Layer
{
public:
    ImguiLayer() :Layer("ImguiLayer") {}

    virtual void OnAttach() override;
    virtual void OnDeatch() override;
    virtual void OnUpdate() override;
    virtual void OnRenderer() override;


};