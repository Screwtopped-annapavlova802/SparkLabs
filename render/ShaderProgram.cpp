#include "ShaderProgram.h"
#include "Texture.h"

namespace SparkLabs {

ShaderProgram::ShaderProgram()
    : m_ProgramId(0)
    , m_IsLinked(false)
    , m_IsBound(false)
{
}

ShaderProgram::~ShaderProgram() {
    Destroy();
}

void ShaderProgram::AttachShader(SmartPtr<Shader> shader) {
    m_Shaders.PushBack(shader);
}

bool ShaderProgram::Link() {
    m_IsLinked = true;
    m_ProgramId = 0;
    return true;
}

void ShaderProgram::Bind() {
    m_IsBound = true;
}

void ShaderProgram::Unbind() {
    m_IsBound = false;
}

int32 ShaderProgram::GetUniformLocation(const String& name) {
    return -1;
}

void ShaderProgram::SetUniform(const String& name, const Variant& value) {
}

void ShaderProgram::SetTexture(const String& name, Texture* texture) {
}

void ShaderProgram::Destroy() {
    m_Shaders.Clear();
    m_IsLinked = false;
    m_ProgramId = 0;
    m_IsBound = false;
}

}
