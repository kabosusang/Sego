#include "ImguiLayer.h"
#include "LayerStack.h"

std::unique_ptr<LayerStack> LayerStack::s_LayerStackInsatnce = std::make_unique<LayerStack>();

void Init_LayerStack()
{
    LayerStack::GetInstance()->Push(new ImguiLayer);


}

void ImguiLayer::OnAttach()
{


}
/*
virtual void OnDeatch() override;
    virtual void OnUpdate() override;
    virtual void OnRenderer() override;
*/
void ImguiLayer::OnUpdate()
{

}

void ImguiLayer::OnRenderer()
{

}

void ImguiLayer::OnDeatch()
{

}

