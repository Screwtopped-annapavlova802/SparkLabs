#ifndef SPARKLABS_ENGINE_RESOURCE_RESOURCE_H
#define SPARKLABS_ENGINE_RESOURCE_RESOURCE_H

#include <Object.h>
#include <SmartPtr.h>
#include <String.h>

namespace SparkLabs {

class Resource : public Object {
    SPARKLABS_OBJECT(Resource)
public:
    enum class ResourceState {
        Unloaded,
        Loading,
        Loaded,
        Failed
    };

    enum class ResourceType {
        Mesh,
        Texture,
        Material,
        Shader,
        Audio,
        Animation,
        Scene,
        Font,
        Other
    };

    Resource();
    virtual ~Resource();

    const String& GetFilePath() const;
    void SetFilePath(const String& path);

    ResourceType GetResourceType() const;
    void SetResourceType(ResourceType type);

    ResourceState GetState() const;

    virtual bool Load();
    virtual bool Unload();
    virtual bool Reload();

    virtual size_t GetMemoryUsage() const;

protected:
    String m_FilePath;
    ResourceType m_ResourceType;
    ResourceState m_State;
};

}

#endif
