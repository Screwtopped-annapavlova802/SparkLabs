#include "Mesh.h"

namespace SparkLabs {

Mesh::Mesh()
    : m_VAO(0)
    , m_VBO(0)
    , m_EBO(0)
{
}

Mesh::~Mesh() {
    Destroy();
}

void Mesh::SetVertices(const Vector<Vertex>& vertices) {
    m_Vertices = vertices;
    CalculateBounds();
}

void Mesh::SetIndices(const Vector<uint32_t>& indices) {
    m_Indices = indices;
}

void Mesh::CalculateBounds() {
    if (m_Vertices.Empty()) {
        m_Bounds.min = Vector3(0.0f, 0.0f, 0.0f);
        m_Bounds.max = Vector3(0.0f, 0.0f, 0.0f);
        return;
    }

    m_Bounds.min = m_Vertices[0].position;
    m_Bounds.max = m_Vertices[0].position;

    for (size_t i = 1; i < m_Vertices.Size(); ++i) {
        const Vector3& pos = m_Vertices[i].position;

        if (pos.x < m_Bounds.min.x) m_Bounds.min.x = pos.x;
        if (pos.y < m_Bounds.min.y) m_Bounds.min.y = pos.y;
        if (pos.z < m_Bounds.min.z) m_Bounds.min.z = pos.z;

        if (pos.x > m_Bounds.max.x) m_Bounds.max.x = pos.x;
        if (pos.y > m_Bounds.max.y) m_Bounds.max.y = pos.y;
        if (pos.z > m_Bounds.max.z) m_Bounds.max.z = pos.z;
    }
}

void Mesh::Destroy() {
    m_Vertices.Clear();
    m_Indices.Clear();
    m_VAO = 0;
    m_VBO = 0;
    m_EBO = 0;
}

}
