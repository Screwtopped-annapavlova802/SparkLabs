#ifndef SPARKLABS_ENGINE_SCENE_SCENE_H
#define SPARKLABS_ENGINE_SCENE_SCENE_H

#include <SmartPtr.h>
#include <Vector.h>
#include <String.h>

namespace SparkLabs {

class GameObject;

class Scene : public Object {
    SPARKLABS_OBJECT(Scene)
public:
    Scene();
    virtual ~Scene();

    void SetName(const String& name);
    const String& GetName() const;

    GameObject* GetRoot() const;

    GameObject* CreateEntity(const String& name = "Entity");
    void AddEntity(GameObject* entity);
    void RemoveEntity(GameObject* entity);

    GameObject* FindEntity(StringHash id);
    GameObject* FindEntityByName(const String& name);

    Vector<GameObject*> GetEntitiesWithTag(const String& tag);

    void Update(float32 deltaTime);
    void FixedUpdate(float32 deltaTime);
    void LateUpdate(float32 deltaTime);

    virtual void OnUpdate(float32 deltaTime);

private:
    String m_Name;
    SmartPtr<GameObject> m_Root;
    Vector< SmartPtr<GameObject> > m_Entities;
};

}

#endif
