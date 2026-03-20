#ifndef SPARKAI_RENDER_RENDERCONTEXT_H
#define SPARKAI_RENDER_RENDERCONTEXT_H

#include "../../core/Types.h"
#include "../../core/math/Vector3.h"
#include "../../core/math/Matrix4x4.h"
#include "../../core/io/Vector.h"
#include "../../core/memory/SmartPtr.h"

namespace SparkLabs {

class Camera;

struct Light {
    Vector3 position;
    float32 intensity;
    Vector3 color;
};

class RenderContext {
public:
    RenderContext();
    ~RenderContext();

    void SetCamera(Camera* camera);
    Camera* GetCamera() const { return m_Camera; }

    void AddLight(const Light& light);
    void RemoveLight(int32 index);
    const Vector<Light>& GetLights() const { return m_Lights; }

    void SetViewProjection(const Matrix4x4& vp) { m_ViewProjection = vp; }
    const Matrix4x4& GetViewProjection() const { return m_ViewProjection; }

    void SetInvViewProjection(const Matrix4x4& invvp) { m_InvViewProjection = invvp; }
    const Matrix4x4& GetInvViewProjection() const { return m_InvViewProjection; }

    void Clear();

private:
    Camera* m_Camera;
    Vector<Light> m_Lights;
    Matrix4x4 m_ViewProjection;
    Matrix4x4 m_InvViewProjection;
};

}

#endif
