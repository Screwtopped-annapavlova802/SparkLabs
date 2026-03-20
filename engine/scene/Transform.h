#ifndef SPARKLABS_ENGINE_SCENE_TRANSFORM_H
#define SPARKLABS_ENGINE_SCENE_TRANSFORM_H

#include <Vector3.h>
#include <Quaternion.h>
#include <Matrix4x4.h>

namespace SparkLabs {

class Transform {
public:
    static Matrix4x4 ComposeTransform(const Vector3& position, const Quaternion& rotation, const Vector3& scale);

    static Vector3 GetForward(const Quaternion& rotation);
    static Vector3 GetRight(const Quaternion& rotation);
    static Vector3 GetUp(const Quaternion& rotation);

    static Quaternion LookRotation(const Vector3& forward, const Vector3& up = Vector3::Up());

    static Vector3 TransformPoint(const Matrix4x4& transform, const Vector3& point);
    static Vector3 TransformVector(const Matrix4x4& transform, const Vector3& vector);
    static Vector3 TransformDirection(const Matrix4x4& transform, const Vector3& direction);

    static Vector3 WorldToLocalPoint(const Matrix4x4& transform, const Vector3& worldPoint);
    static Vector3 LocalToWorldPoint(const Matrix4x4& transform, const Vector3& localPoint);

    static void DecomposeTransform(const Matrix4x4& transform, Vector3& position, Quaternion& rotation, Vector3& scale);
};

}

#endif
