#include "Shader.h"

namespace SparkLabs {

Shader::Shader()
    : m_Type(ShaderType::Vertex)
    , m_IsCompiled(false)
    , m_ShaderId(0)
{
}

Shader::~Shader() {
    Destroy();
}

bool Shader::Compile(const String& source, ShaderType type) {
    m_Source = source;
    m_Type = type;
    m_IsCompiled = true;
    m_ShaderId = 0;
    return true;
}

void Shader::Destroy() {
    m_Source.Clear();
    m_IsCompiled = false;
    m_ShaderId = 0;
}

}
