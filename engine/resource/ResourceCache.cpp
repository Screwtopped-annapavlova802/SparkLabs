#include "ResourceCache.h"

namespace SparkLabs {

ResourceCache& ResourceCache::GetInstance() {
    static ResourceCache instance;
    return instance;
}

ResourceCache::ResourceCache()
    : m_MemoryBudget(0)
    , m_MemoryUsage(0)
{
}

ResourceCache::~ResourceCache() {
}

void ResourceCache::RegisterResource(Resource* resource) {
    LockGuard<Mutex> lock(m_Mutex);
    m_Resources.PushBack(resource);
}

void ResourceCache::UnregisterResource(Resource* resource) {
    LockGuard<Mutex> lock(m_Mutex);
    for (auto it = m_Resources.Begin(); it != m_Resources.End(); ++it) {
        if (*it == resource) {
            m_Resources.Erase(it);
            return;
        }
    }
}

Resource* ResourceCache::GetResource(const String& path) {
    LockGuard<Mutex> lock(m_Mutex);
    for (auto* resource : m_Resources) {
        if (resource->GetFilePath() == path) {
            return resource;
        }
    }
    return nullptr;
}

bool ResourceCache::HasResource(const String& path) const {
    LockGuard<Mutex> lock(m_Mutex);
    for (auto* resource : m_Resources) {
        if (resource->GetFilePath() == path) {
            return true;
        }
    }
    return false;
}

void ResourceCache::SetMemoryBudget(size_t budget) {
    m_MemoryBudget = budget;
}

size_t ResourceCache::GetMemoryBudget() const {
    return m_MemoryBudget;
}

size_t ResourceCache::GetMemoryUsage() const {
    return m_MemoryUsage;
}

void ResourceCache::GarbageCollect() {
    if (m_MemoryUsage <= m_MemoryBudget) return;
    LockGuard<Mutex> lock(m_Mutex);
    for (auto it = m_Resources.Begin(); it != m_Resources.End(); ++it) {
        if (m_MemoryUsage <= m_MemoryBudget) break;
        Resource* resource = *it;
        if (resource->GetState() == Resource::ResourceState::Loaded) {
            resource->Unload();
            m_MemoryUsage -= resource->GetMemoryUsage();
        }
    }
}

}
