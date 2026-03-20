#ifndef SPARKAI_RENDER_TEXTURE_H
#define SPARKAI_RENDER_TEXTURE_H

#include "../../core/Types.h"
#include "../../core/string/String.h"
#include "../../core/memory/SmartPtr.h"

namespace SparkLabs {

class Texture {
public:
    enum class TextureFormat {
        R8,
        RG8,
        RGB8,
        RGBA8,
        R16F,
        RG16F,
        RGB16F,
        RGBA16F,
        D24S8
    };

    enum class TextureType {
        Tex2D,
        Tex3D,
        CubeMap,
        Tex2DArray
    };

    Texture();
    virtual ~Texture();

    int32 GetWidth() const { return m_Width; }
    int32 GetHeight() const { return m_Height; }
    int32 GetDepth() const { return m_Depth; }
    int32 GetMipLevels() const { return m_MipLevels; }
    TextureFormat GetFormat() const { return m_Format; }
    TextureType GetType() const { return m_Type; }

    virtual bool LoadFromFile(const String& path);
    virtual bool Create(int32 width, int32 height, TextureFormat format, const void* data);
    virtual void Destroy();

    void SetData(const void* data);
    void GenerateMipmaps();

    int32 GetTextureId() const { return m_TextureId; }

private:
    bool LoadFromFileInternal(const String& path);

    int32 m_Width;
    int32 m_Height;
    int32 m_Depth;
    int32 m_MipLevels;
    TextureFormat m_Format;
    TextureType m_Type;
    int32 m_TextureId;
};

}

#endif
