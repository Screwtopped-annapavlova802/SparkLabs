#ifndef SPARKLABS_ENGINE_SCENE_GAMEOBJECT_H
#define SPARKLABS_ENGINE_SCENE_GAMEOBJECT_H

#include "Node.h"
#include "Component.h"
#include <Vector.h>
#include <StringHash.h>

namespace SparkLabs {

class GameObject : public Node {
    SPARKLABS_OBJECT(GameObject)
public:
    GameObject();
    virtual ~GameObject();

    void SetTag(const String& tag);
    const String& GetTag() const;

    void SetLayer(int32 layer);
    int32 GetLayer() const;

    template<typename T>
    T* AddComponent();

    template<typename T>
    T* GetComponent();

    template<typename T>
    Vector<T*> GetComponents();

    template<typename T>
    void GetComponents(Vector<T*>& results);

    void RemoveComponent(Component* component);

    GameObject* FindChildByName(const String& name);
    GameObject* FindByTag(const String& tag);

    virtual void OnUpdate(float32 deltaTime) override;

private:
    String m_Tag;
    int32 m_Layer;
    Vector< SmartPtr<Component> > m_Components;
};

template<typename T>
T* GameObject::AddComponent() {
    T* component = new T();
    component->SetOwner(this);
    m_Components.PushBack(SmartPtr<Component>(component));
    return component;
}

template<typename T>
T* GameObject::GetComponent() {
    for (auto& component : m_Components) {
        if (T* result = DynamicCast<T*>(component.Get())) {
            return result;
        }
    }
    return nullptr;
}

template<typename T>
Vector<T*> GameObject::GetComponents() {
    Vector<T*> results;
    GetComponents(results);
    return results;
}

template<typename T>
void GameObject::GetComponents(Vector<T*>& results) {
    for (auto& component : m_Components) {
        if (T* result = DynamicCast<T*>(component.Get())) {
            results.PushBack(result);
        }
    }
}

}

#endif
