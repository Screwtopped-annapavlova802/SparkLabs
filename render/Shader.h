#ifndef SPARKAI_RENDER_SHADER_H
#define SPARKAI_RENDER_SHADER_H

#include "../../core/Types.h"
#include "../../core/string/String.h"
#include "../../core/memory/SmartPtr.h"

namespace SparkLabs {

class Shader {
public:
    enum class ShaderType {
        Vertex,
        Fragment,
        Geometry,
        Compute,
        TessControl,
        TessEval
    };

    Shader();
    ~Shader();

    bool Compile(const String& source, ShaderType type);
    bool IsCompiled() const { return m_IsCompiled; }

    ShaderType GetType() const { return m_Type; }
    const String& GetSource() const { return m_Source; }
    int32 GetShaderId() const { return m_ShaderId; }

    void Destroy();

private:
    ShaderType m_Type;
    String m_Source;
    bool m_IsCompiled;
    int32 m_ShaderId;
};

}

#endif
