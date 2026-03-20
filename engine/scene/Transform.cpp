#include "Transform.h"

namespace SparkLabs {

Matrix4x4 Transform::ComposeTransform(const Vector3& position, const Quaternion& rotation, const Vector3& scale) {
    Matrix4x4 result = Matrix4x4::Identity();
    result.SetTranslation(position);
    result = result * rotation.ToMatrix();
    result.SetScale(scale);
    return result;
}

Vector3 Transform::GetForward(const Quaternion& rotation) {
    return rotation * Vector3::Forward();
}

Vector3 Transform::GetRight(const Quaternion& rotation) {
    return rotation * Vector3::Right();
}

Vector3 Transform::GetUp(const Quaternion& rotation) {
    return rotation * Vector3::Up();
}

Quaternion Transform::LookRotation(const Vector3& forward, const Vector3& up) {
    Vector3 f = forward.Normalized();
    Vector3 r = Vector3::Cross(f, up).Normalized();
    Vector3 u = Vector3::Cross(r, f);
    return Quaternion::FromLookRotation(f, u);
}

Vector3 Transform::TransformPoint(const Matrix4x4& transform, const Vector3& point) {
    return transform.TransformPoint(point);
}

Vector3 Transform::TransformVector(const Matrix4x4& transform, const Vector3& vector) {
    return transform.TransformVector(vector);
}

Vector3 Transform::TransformDirection(const Matrix4x4& transform, const Vector3& direction) {
    return transform.TransformDirection(direction);
}

Vector3 Transform::WorldToLocalPoint(const Matrix4x4& transform, const Vector3& worldPoint) {
    Matrix4x4 inv = transform.Inverse();
    return inv.TransformPoint(worldPoint);
}

Vector3 Transform::LocalToWorldPoint(const Matrix4x4& transform, const Vector3& localPoint) {
    return transform.TransformPoint(localPoint);
}

void Transform::DecomposeTransform(const Matrix4x4& transform, Vector3& position, Quaternion& rotation, Vector3& scale) {
    position = transform.GetTranslation();
    rotation = Quaternion::FromMatrix(transform.ExtractRotation());
    scale = transform.ExtractScale();
}

}
