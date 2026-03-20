#include "Component.h"

namespace SparkLabs {

Component::Component()
    : m_Enabled(true)
{
}

Component::~Component() {
    if (m_Owner) {
        m_Owner->RemoveChild(this);
    }
}

Node* Component::GetOwner() const {
    return m_Owner.Get();
}

void Component::SetOwner(Node* owner) {
    if (m_Owner == owner) return;
    if (m_Owner) {
        OnDetached();
    }
    m_Owner = owner;
    if (m_Owner) {
        OnAttached();
    }
}

bool Component::IsEnabled() const {
    if (!m_Enabled) return false;
    if (m_Owner) return m_Owner->IsActive();
    return true;
}

void Component::SetEnabled(bool enabled) {
    m_Enabled = enabled;
}

void Component::OnAttached() {
}

void Component::OnDetached() {
}

void Component::OnUpdate(float32 deltaTime) {
}

void Component::OnFixedUpdate(float32 deltaTime) {
}

void Component::OnLateUpdate(float32 deltaTime) {
}

}
