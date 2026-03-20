#ifndef SPARKAI_RENDER_MESH_H
#define SPARKAI_RENDER_MESH_H

#include "../../core/Types.h"
#include "../../core/io/Vector.h"
#include "RenderDevice.h"

namespace SparkLabs {

class Mesh {
public:
    Mesh();
    ~Mesh();

    void SetVertices(const Vector<Vertex>& vertices);
    void SetIndices(const Vector<uint32_t>& indices);

    const Vector<Vertex>& GetVertices() const { return m_Vertices; }
    const Vector<uint32_t>& GetIndices() const { return m_Indices; }

    void CalculateBounds();

    const AABB& GetBounds() const { return m_Bounds; }

    int32 GetVertexCount() const { return static_cast<int32>(m_Vertices.Size()); }
    int32 GetIndexCount() const { return static_cast<int32>(m_Indices.Size()); }

    void Destroy();

private:
    Vector<Vertex> m_Vertices;
    Vector<uint32_t> m_Indices;
    AABB m_Bounds;
    int32 m_VAO;
    int32 m_VBO;
    int32 m_EBO;
};

}

#endif
