#include "ResourceLoader.h"

namespace SparkLabs {

Vector<String> ResourceLoader::GetSupportedExtensions() const {
    return m_SupportedExtensions;
}

StringHash ResourceLoader::GetResourceType() const {
    return m_ResourceType;
}

void ResourceLoader::SetResourceType(StringHash type) {
    m_ResourceType = type;
}

void ResourceLoader::AddSupportedExtension(const String& ext) {
    m_SupportedExtensions.PushBack(ext);
}

}
