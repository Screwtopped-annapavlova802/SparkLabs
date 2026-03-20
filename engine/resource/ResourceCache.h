#ifndef SPARKLABS_ENGINE_RESOURCE_RESOURCECACHE_H
#define SPARKLABS_ENGINE_RESOURCE_RESOURCECACHE_H

#include "Resource.h"
#include <Vector.h>
#include <Mutex.h>
#include <Signal.h>

namespace SparkLabs {

class ResourceCache {
public:
    static ResourceCache& GetInstance();

    void RegisterResource(Resource* resource);
    void UnregisterResource(Resource* resource);

    template<typename T>
    T* GetResource(const String& path);

    Resource* GetResource(const String& path);

    bool HasResource(const String& path) const;

    void SetMemoryBudget(size_t budget);
    size_t GetMemoryBudget() const;
    size_t GetMemoryUsage() const;

    void GarbageCollect();

    Signal<void(Resource*)> ResourceLoadedSignal;
    Signal<void(Resource*)> ResourceUnloadedSignal;

private:
    ResourceCache();
    ~ResourceCache();
    ResourceCache(const ResourceCache&) = delete;
    ResourceCache& operator=(const ResourceCache&) = delete;

    Vector<Resource*> m_Resources;
    size_t m_MemoryBudget;
    size_t m_MemoryUsage;
    Mutex m_Mutex;
};

template<typename T>
T* ResourceCache::GetResource(const String& path) {
    return DynamicCast<T*>(GetResource(path));
}

}

#endif
