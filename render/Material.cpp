#include "Material.h"
#include "ShaderProgram.h"
#include "Texture.h"

namespace SparkLabs {

Material::Material() {
}

Material::~Material() {
    Destroy();
}

void Material::SetTexture(const String& name, Texture* tex) {
    m_Textures.PushBack(SmartPtr<Texture>(tex));
}

void Material::SetFloat(const String& name, float32 value) {
    Variant variant(value);
    StringHash hash(name.c_str());
    m_Uniforms.Set(hash, &variant);
}

void Material::SetVector3(const String& name, const Vector3& value) {
    Variant variant(value);
    StringHash hash(name.c_str());
    m_Uniforms.Set(hash, &variant);
}

void Material::SetMatrix4x4(const String& name, const Matrix4x4& value) {
    Variant variant(value);
    StringHash hash(name.c_str());
    m_Uniforms.Set(hash, &variant);
}

Texture* Material::GetTexture(const String& name) const {
    return nullptr;
}

Variant* Material::GetUniform(const String& name) {
    StringHash hash(name.c_str());
    return m_Uniforms.Get(hash);
}

void Material::Bind() {
    if (m_Shader) {
        m_Shader->Bind();
    }
}

void Material::Unbind() {
    if (m_Shader) {
        m_Shader->Unbind();
    }
}

void Material::Destroy() {
    m_Shader.Reset();
    m_Uniforms.Clear();
    m_Textures.Clear();
}

}
