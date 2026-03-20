#ifndef SPARKAI_RENDER_RENDERDEVICE_H
#define SPARKAI_RENDER_RENDERDEVICE_H

#include "../../core/Types.h"
#include "../../core/string/String.h"
#include "../../core/io/Vector.h"
#include "../../core/math/Vector2.h"
#include <cstdint>

namespace SparkLabs {

struct RenderSettings {
    int32 width;
    int32 height;
    bool vsync;
    bool debug;
    int32 msaaSamples;
};

struct DrawCall {
    int32 vertexCount;
    int32 firstVertex;
    int32 instanceCount;
    int32 baseInstance;
};

struct AABB {
    Vector3 min;
    Vector3 max;

    AABB() : min(Vector3(0.0f, 0.0f, 0.0f)), max(Vector3(0.0f, 0.0f, 0.0f)) {}
    AABB(const Vector3& min_, const Vector3& max_) : min(min_), max(max_) {}
};

struct Vertex {
    Vector3 position;
    Vector3 normal;
    Vector2 texCoord;
    Vector3 tangent;
    Vector3 bitangent;
};

class Camera {
public:
    Camera() : m_FOV(60.0f), m_Aspect(1.77f), m_Near(0.1f), m_Far(1000.0f) {}
    virtual ~Camera() {}

    void SetPosition(const Vector3& pos) { m_Position = pos; }
    Vector3 GetPosition() const { return m_Position; }

    void SetRotation(const Vector3& rot) { m_Rotation = rot; }
    Vector3 GetRotation() const { return m_Rotation; }

    void SetFOV(float32 fov) { m_FOV = fov; }
    float32 GetFOV() const { return m_FOV; }

    void SetAspect(float32 aspect) { m_Aspect = aspect; }
    float32 GetAspect() const { return m_Aspect; }

    void SetNearPlane(float32 near) { m_Near = near; }
    float32 GetNearPlane() const { return m_Near; }

    void SetFarPlane(float32 far) { m_Far = far; }
    float32 GetFarPlane() const { return m_Far; }

    Matrix4x4 GetViewMatrix() const;
    Matrix4x4 GetProjectionMatrix() const;
    Matrix4x4 GetViewProjectionMatrix() const;

private:
    Vector3 m_Position;
    Vector3 m_Rotation;
    float32 m_FOV;
    float32 m_Aspect;
    float32 m_Near;
    float32 m_Far;
};

class RenderDevice {
public:
    enum class Backend {
        OpenGL,
        Vulkan,
        Metal,
        DirectX
    };

    virtual ~RenderDevice() {}

    virtual void Initialize(const RenderSettings& settings) = 0;
    virtual void Shutdown() = 0;
    virtual void BeginFrame() = 0;
    virtual void EndFrame() = 0;
    virtual void Draw(const DrawCall& call) = 0;
    virtual void SetViewport(int32 x, int32 y, int32 width, int32 height) = 0;

    Backend GetBackend() const { return m_Backend; }

protected:
    RenderDevice(Backend backend) : m_Backend(backend) {}

    Backend m_Backend;
    RenderSettings m_Settings;
};

}

#endif
