#include "Resource.h"

namespace SparkLabs {

Resource::Resource()
    : m_ResourceType(ResourceType::Other)
    , m_State(ResourceState::Unloaded)
{
}

Resource::~Resource() {
    Unload();
}

const String& Resource::GetFilePath() const {
    return m_FilePath;
}

void Resource::SetFilePath(const String& path) {
    m_FilePath = path;
}

Resource::ResourceType Resource::GetResourceType() const {
    return m_ResourceType;
}

void Resource::SetResourceType(ResourceType type) {
    m_ResourceType = type;
}

Resource::ResourceState Resource::GetState() const {
    return m_State;
}

bool Resource::Load() {
    if (m_State == ResourceState::Loaded) {
        return true;
    }
    m_State = ResourceState::Loading;
    bool result = Load();
    m_State = result ? ResourceState::Loaded : ResourceState::Failed;
    return result;
}

bool Resource::Unload() {
    if (m_State == ResourceState::Unloaded) {
        return true;
    }
    bool result = Unload();
    if (result) {
        m_State = ResourceState::Unloaded;
    }
    return result;
}

bool Resource::Reload() {
    Unload();
    return Load();
}

size_t Resource::GetMemoryUsage() const {
    return 0;
}

}
