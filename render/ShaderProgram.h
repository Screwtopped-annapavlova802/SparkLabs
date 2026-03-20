#ifndef SPARKAI_RENDER_SHADERPROGRAM_H
#define SPARKAI_RENDER_SHADERPROGRAM_H

#include "../../core/Types.h"
#include "../../core/string/String.h"
#include "../../core/string/StringHash.h"
#include "../../core/io/Vector.h"
#include "../../core/memory/SmartPtr.h"
#include "Shader.h"

namespace SparkLabs {

class Texture;
class Matrix4x4;
class Vector3;
struct Variant;

class ShaderProgram {
public:
    ShaderProgram();
    ~ShaderProgram();

    void AttachShader(SmartPtr<Shader> shader);
    bool Link();

    void Bind();
    void Unbind();

    int32 GetUniformLocation(const String& name);
    void SetUniform(const String& name, const Variant& value);
    void SetTexture(const String& name, Texture* texture);

    bool IsLinked() const { return m_IsLinked; }
    int32 GetProgramId() const { return m_ProgramId; }

    void Destroy();

private:
    Vector< SmartPtr<Shader> > m_Shaders;
    int32 m_ProgramId;
    bool m_IsLinked;
    bool m_IsBound;
};

}

#endif
