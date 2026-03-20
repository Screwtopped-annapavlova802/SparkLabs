#include "GameObject.h"

namespace SparkLabs {

GameObject::GameObject()
    : m_Layer(0)
{
    m_Name = "GameObject";
}

GameObject::~GameObject() {
    m_Components.Clear();
}

void GameObject::SetTag(const String& tag) {
    m_Tag = tag;
}

const String& GameObject::GetTag() const {
    return m_Tag;
}

void GameObject::SetLayer(int32 layer) {
    m_Layer = layer;
}

int32 GameObject::GetLayer() const {
    return m_Layer;
}

void GameObject::RemoveComponent(Component* component) {
    for (auto it = m_Components.Begin(); it != m_Components.End(); ++it) {
        if (it->Get() == component) {
            m_Components.Erase(it);
            return;
        }
    }
}

GameObject* GameObject::FindChildByName(const String& name) {
    for (int i = 0; i < GetChildCount(); ++i) {
        Node* child = GetChild(i);
        if (child->GetName() == name) {
            return DynamicCast<GameObject*>(child);
        }
        if (GameObject* grandchild = child->FindChildByName(name)) {
            return grandchild;
        }
    }
    return nullptr;
}

GameObject* GameObject::FindByTag(const String& tag) {
    if (m_Tag == tag) {
        return this;
    }
    for (int i = 0; i < GetChildCount(); ++i) {
        if (GameObject* child = DynamicCast<GameObject*>(GetChild(i))) {
            if (child->GetTag() == tag) {
                return child;
            }
            if (GameObject* found = child->FindByTag(tag)) {
                return found;
            }
        }
    }
    return nullptr;
}

void GameObject::OnUpdate(float32 deltaTime) {
    for (auto& component : m_Components) {
        if (component->IsEnabled()) {
            component->OnUpdate(deltaTime);
        }
    }
}

}
