#ifndef SPARKLABS_ENGINE_RESOURCE_RESOURCELOADER_H
#define SPARKLABS_ENGINE_RESOURCE_RESOURCELOADER_H

#include <String.h>
#include <Vector.h>
#include <StringHash.h>

namespace SparkLabs {

class Resource;

class ResourceLoader {
public:
    virtual ~ResourceLoader() = default;

    virtual bool CanLoad(StringHash type, const String& path) const = 0;
    virtual Resource* Load(const String& path) = 0;

    Vector<String> GetSupportedExtensions() const;
    StringHash GetResourceType() const;

protected:
    void SetResourceType(StringHash type);
    void AddSupportedExtension(const String& ext);

private:
    StringHash m_ResourceType;
    Vector<String> m_SupportedExtensions;
};

}

#endif
