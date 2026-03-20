#include "Texture.h"

namespace SparkLabs {

Texture::Texture()
    : m_Width(0)
    , m_Height(0)
    , m_Depth(1)
    , m_MipLevels(1)
    , m_Format(TextureFormat::RGBA8)
    , m_Type(TextureType::Tex2D)
    , m_TextureId(0)
{
}

Texture::~Texture() {
    Destroy();
}

bool Texture::LoadFromFile(const String& path) {
    return LoadFromFileInternal(path);
}

bool Texture::Create(int32 width, int32 height, TextureFormat format, const void* data) {
    m_Width = width;
    m_Height = height;
    m_Format = format;
    m_Type = TextureType::Tex2D;
    m_MipLevels = 1;

    if (data) {
        SetData(data);
    }

    return true;
}

void Texture::SetData(const void* data) {
}

void Texture::GenerateMipmaps() {
    m_MipLevels = 1;
}

bool Texture::LoadFromFileInternal(const String& path) {
    m_Width = 0;
    m_Height = 0;
    m_TextureId = 0;
    return false;
}

void Texture::Destroy() {
    m_Width = 0;
    m_Height = 0;
    m_Depth = 1;
    m_MipLevels = 1;
    m_TextureId = 0;
}

}
