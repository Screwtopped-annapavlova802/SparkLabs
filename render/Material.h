#ifndef SPARKAI_RENDER_MATERIAL_H
#define SPARKAI_RENDER_MATERIAL_H

#include "../../core/Types.h"
#include "../../core/string/String.h"
#include "../../core/string/StringHash.h"
#include "../../core/io/Vector.h"
#include "../../core/memory/SmartPtr.h"
#include "../../core/math/Vector3.h"
#include "../../core/math/Matrix4x4.h"

namespace SparkLabs {

class Texture;
class ShaderProgram;

struct Variant {
    enum class Type {
        None,
        Int,
        Float,
        Vector3,
        Vector4,
        Matrix4x4,
        Bool
    };

    Variant() : type(Type::None), intValue(0) {}
    explicit Variant(int32 value) : type(Type::Int), intValue(value) {}
    explicit Variant(float32 value) : type(Type::Float), floatValue(value) {}
    explicit Variant(const Vector3& value) : type(Type::Vector3), vec3Value(value) {}
    explicit Variant(const Vector4& value) : type(Type::Vector4), vec4Value(value) {}
    explicit Variant(const Matrix4x4& value) : type(Type::Matrix4x4), mat4Value(value) {}
    explicit Variant(bool value) : type(Type::Bool), boolValue(value) {}

    Type type;
    union {
        int32 intValue;
        float32 floatValue;
        bool boolValue;
        Vector3 vec3Value;
        Vector4 vec4Value;
        Matrix4x4 mat4Value;
    };
};

class Material {
public:
    Material();
    ~Material();

    void SetShader(SmartPtr<ShaderProgram> shader) { m_Shader = shader; }
    SmartPtr<ShaderProgram> GetShader() const { return m_Shader; }

    void SetTexture(const String& name, Texture* tex);
    void SetFloat(const String& name, float32 value);
    void SetVector3(const String& name, const Vector3& value);
    void SetMatrix4x4(const String& name, const Matrix4x4& value);

    Texture* GetTexture(const String& name) const;
    Variant* GetUniform(const String& name);

    void Bind();
    void Unbind();

    void Destroy();

private:
    SmartPtr<ShaderProgram> m_Shader;
    HashMap<Variant> m_Uniforms;
    Vector< SmartPtr<Texture> > m_Textures;
};

}

#endif
