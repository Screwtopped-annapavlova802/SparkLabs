#ifndef SPARKLABS_ENGINE_SCENE_NODE_H
#define SPARKLABS_ENGINE_SCENE_NODE_H

#include <Vector3.h>
#include <Quaternion.h>
#include <SmartPtr.h>
#include <String.h>

namespace SparkLabs {

class Node : public Object {
    SPARKLABS_OBJECT(Node)
public:
    Node();
    virtual ~Node();

    void SetName(const String& name);
    const String& GetName() const;

    void SetPosition(const Vector3& position);
    Vector3 GetPosition() const;

    void SetRotation(const Quaternion& rotation);
    Quaternion GetRotation() const;

    void SetScale(const Vector3& scale);
    Vector3 GetScale() const;

    void SetLocalPosition(const Vector3& position);
    Vector3 GetLocalPosition() const;

    void SetLocalRotation(const Quaternion& rotation);
    Quaternion GetLocalRotation() const;

    void SetLocalScale(const Vector3& scale);
    Vector3 GetLocalScale() const;

    Matrix4x4 GetLocalTransformMatrix() const;
    Matrix4x4 GetWorldTransformMatrix() const;

    void LookAt(const Vector3& target);
    void LookAt(const Vector3& target, const Vector3& up);

    Vector3 GetForward() const;
    Vector3 GetRight() const;
    Vector3 GetUp() const;

    Node* GetParent() const;
    void SetParent(Node* parent);
    void AddChild(Node* child);
    void RemoveChild(Node* child);
    void RemoveFromParent();
    int GetChildCount() const;
    Node* GetChild(int index);

    void SetDirty(bool dirty = true);
    bool IsDirty() const;

    void SetActive(bool active);
    bool IsActive() const;

    void SetVisible(bool visible);
    bool IsVisible() const;

    virtual void OnTransformChanged();

protected:
    String m_Name;
    Vector3 m_LocalPosition;
    Quaternion m_LocalRotation;
    Vector3 m_LocalScale;

    WeakPtr<Node> m_Parent;
    Vector< SmartPtr<Node> > m_Children;

    bool m_Dirty;
    bool m_Active;
    bool m_Visible;

private:
    void UpdateWorldTransform() const;
    mutable Matrix4x4 m_WorldTransform;
    mutable bool m_WorldTransformDirty;
};

}

#endif
