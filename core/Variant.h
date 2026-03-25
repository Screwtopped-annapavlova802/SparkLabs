#ifndef SPARKLABS_CORE_VARIANT_H
#define SPARKLABS_CORE_VARIANT_H

#include "Types.h"
#include "string/String.h"
#include "math/Vector2.h"
#include "math/Vector3.h"
#include "math/Vector4.h"
#include "math/Quaternion.h"
#include "math/Matrix4x4.h"

namespace SparkLabs {

enum class VariantType {
    None,
    Bool,
    Int32,
    Int64,
    UInt32,
    UInt64,
    Float32,
    Float64,
    String,
    Vector2,
    Vector3,
    Vector4,
    Quaternion,
    Matrix4x4
};

class Variant {
public:
    Variant();
    Variant(const Variant& other);
    Variant(Variant&& other) noexcept;
    ~Variant();

    explicit Variant(bool value);
    explicit Variant(int32 value);
    explicit Variant(int64 value);
    explicit Variant(uint32 value);
    explicit Variant(uint64 value);
    explicit Variant(float32 value);
    explicit Variant(float64 value);
    explicit Variant(const String& value);
    explicit Variant(const char* value);
    explicit Variant(const Vector2& value);
    explicit Variant(const Vector3& value);
    explicit Variant(const Vector4& value);
    explicit Variant(const Quaternion& value);
    explicit Variant(const Matrix4x4& value);

    Variant& operator=(const Variant& other);
    Variant& operator=(Variant&& other) noexcept;

    VariantType GetType() const { return m_Type; }
    bool IsValid() const { return m_Type != VariantType::None; }

    bool GetBool() const;
    int32 GetInt32() const;
    int64 GetInt64() const;
    uint32 GetUInt32() const;
    uint64 GetUInt64() const;
    float32 GetFloat32() const;
    float64 GetFloat64() const;
    const String& GetString() const;
    const Vector2& GetVector2() const;
    const Vector3& GetVector3() const;
    const Vector4& GetVector4() const;
    const Quaternion& GetQuaternion() const;
    const Matrix4x4& GetMatrix4x4() const;

    void SetBool(bool value);
    void SetInt32(int32 value);
    void SetInt64(int64 value);
    void SetUInt32(uint32 value);
    void SetUInt64(uint64 value);
    void SetFloat32(float32 value);
    void SetFloat64(float64 value);
    void SetString(const String& value);
    void SetVector2(const Vector2& value);
    void SetVector3(const Vector3& value);
    void SetVector4(const Vector4& value);
    void SetQuaternion(const Quaternion& value);
    void SetMatrix4x4(const Matrix4x4& value);

    void Clear();

private:
    void Destroy();
    void CopyFrom(const Variant& other);
    void MoveFrom(Variant&& other);

    VariantType m_Type;
    union {
        bool m_Bool;
        int32 m_Int32;
        int64 m_Int64;
        uint32 m_UInt32;
        uint64 m_UInt64;
        float32 m_Float32;
        float64 m_Float64;
        String* m_String;
        Vector2* m_Vector2;
        Vector3* m_Vector3;
        Vector4* m_Vector4;
        Quaternion* m_Quaternion;
        Matrix4x4* m_Matrix4x4;
    };
};

}

#endif
