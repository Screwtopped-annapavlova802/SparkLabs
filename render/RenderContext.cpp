#include "RenderContext.h"
#include "RenderDevice.h"

namespace SparkLabs {

RenderContext::RenderContext()
    : m_Camera(nullptr)
{
    m_ViewProjection.LoadIdentity();
    m_InvViewProjection.LoadIdentity();
}

RenderContext::~RenderContext() {
    Clear();
}

void RenderContext::SetCamera(Camera* camera) {
    m_Camera = camera;
}

void RenderContext::AddLight(const Light& light) {
    m_Lights.PushBack(light);
}

void RenderContext::RemoveLight(int32 index) {
    if (index >= 0 && index < static_cast<int32>(m_Lights.Size())) {
        m_Lights.Erase(index);
    }
}

void RenderContext::Clear() {
    m_Camera = nullptr;
    m_Lights.Clear();
    m_ViewProjection.LoadIdentity();
    m_InvViewProjection.LoadIdentity();
}

}
