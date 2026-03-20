#ifndef SPARKLABS_ENGINE_SCENE_COMPONENT_H
#define SPARKLABS_ENGINE_SCENE_COMPONENT_H

#include <Object.h>
#include <SmartPtr.h>
#include <StringHash.h>

namespace SparkLabs {

class Node;

class Component : public Object {
    SPARKLABS_OBJECT(Component)
public:
    Component();
    virtual ~Component();

    Node* GetOwner() const;
    void SetOwner(Node* owner);

    bool IsEnabled() const;
    void SetEnabled(bool enabled);

    virtual void OnAttached();
    virtual void OnDetached();
    virtual void OnUpdate(float32 deltaTime);
    virtual void OnFixedUpdate(float32 deltaTime);
    virtual void OnLateUpdate(float32 deltaTime);

protected:
    WeakPtr<Node> m_Owner;
    bool m_Enabled;
};

}

#endif
