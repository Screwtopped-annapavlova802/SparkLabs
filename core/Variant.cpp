#include "Variant.h"

namespace SparkLabs {

Variant::Variant()
    : m_Type(VariantType::None)
{
    m_String = nullptr;
}

Variant::Variant(const Variant& other)
    : m_Type(VariantType::None)
{
    m_String = nullptr;
    CopyFrom(other);
}

Variant::Variant(Variant&& other) noexcept
    : m_Type(VariantType::None)
{
    m_String = nullptr;
    MoveFrom(std::move(other));
}

Variant::~Variant() {
    Destroy();
}

Variant::Variant(bool value)
    : m_Type(VariantType::Bool)
{
    m_Bool = value;
}

Variant::Variant(int32 value)
    : m_Type(VariantType::Int32)
{
    m_Int32 = value;
}

Variant::Variant(int64 value)
    : m_Type(VariantType::Int64)
{
    m_Int64 = value;
}

Variant::Variant(uint32 value)
    : m_Type(VariantType::UInt32)
{
    m_UInt32 = value;
}

Variant::Variant(uint64 value)
    : m_Type(VariantType::UInt64)
{
    m_UInt64 = value;
}

Variant::Variant(float32 value)
    : m_Type(VariantType::Float32)
{
    m_Float32 = value;
}

Variant::Variant(float64 value)
    : m_Type(VariantType::Float64)
{
    m_Float64 = value;
}

Variant::Variant(const String& value)
    : m_Type(VariantType::String)
{
    m_String = new String(value);
}

Variant::Variant(const char* value)
    : m_Type(VariantType::String)
{
    m_String = new String(value);
}

Variant::Variant(const Vector2& value)
    : m_Type(VariantType::Vector2)
{
    m_Vector2 = new Vector2(value);
}

Variant::Variant(const Vector3& value)
    : m_Type(VariantType::Vector3)
{
    m_Vector3 = new Vector3(value);
}

Variant::Variant(const Vector4& value)
    : m_Type(VariantType::Vector4)
{
    m_Vector4 = new Vector4(value);
}

Variant::Variant(const Quaternion& value)
    : m_Type(VariantType::Quaternion)
{
    m_Quaternion = new Quaternion(value);
}

Variant::Variant(const Matrix4x4& value)
    : m_Type(VariantType::Matrix4x4)
{
    m_Matrix4x4 = new Matrix4x4(value);
}

Variant& Variant::operator=(const Variant& other) {
    if (this != &other) {
        Destroy();
        CopyFrom(other);
    }
    return *this;
}

Variant& Variant::operator=(Variant&& other) noexcept {
    if (this != &other) {
        Destroy();
        MoveFrom(std::move(other));
    }
    return *this;
}

void Variant::Destroy() {
    switch (m_Type) {
        case VariantType::String:
            delete m_String;
            break;
        case VariantType::Vector2:
            delete m_Vector2;
            break;
        case VariantType::Vector3:
            delete m_Vector3;
            break;
        case VariantType::Vector4:
            delete m_Vector4;
            break;
        case VariantType::Quaternion:
            delete m_Quaternion;
            break;
        case VariantType::Matrix4x4:
            delete m_Matrix4x4;
            break;
        default:
            break;
    }
    m_Type = VariantType::None;
}

void Variant::CopyFrom(const Variant& other) {
    m_Type = other.m_Type;
    switch (m_Type) {
        case VariantType::Bool:
            m_Bool = other.m_Bool;
            break;
        case VariantType::Int32:
            m_Int32 = other.m_Int32;
            break;
        case VariantType::Int64:
            m_Int64 = other.m_Int64;
            break;
        case VariantType::UInt32:
            m_UInt32 = other.m_UInt32;
            break;
        case VariantType::UInt64:
            m_UInt64 = other.m_UInt64;
            break;
        case VariantType::Float32:
            m_Float32 = other.m_Float32;
            break;
        case VariantType::Float64:
            m_Float64 = other.m_Float64;
            break;
        case VariantType::String:
            m_String = new String(*other.m_String);
            break;
        case VariantType::Vector2:
            m_Vector2 = new Vector2(*other.m_Vector2);
            break;
        case VariantType::Vector3:
            m_Vector3 = new Vector3(*other.m_Vector3);
            break;
        case VariantType::Vector4:
            m_Vector4 = new Vector4(*other.m_Vector4);
            break;
        case VariantType::Quaternion:
            m_Quaternion = new Quaternion(*other.m_Quaternion);
            break;
        case VariantType::Matrix4x4:
            m_Matrix4x4 = new Matrix4x4(*other.m_Matrix4x4);
            break;
        default:
            break;
    }
}

void Variant::MoveFrom(Variant&& other) {
    m_Type = other.m_Type;
    switch (m_Type) {
        case VariantType::String:
            m_String = other.m_String;
            other.m_String = nullptr;
            break;
        case VariantType::Vector2:
            m_Vector2 = other.m_Vector2;
            other.m_Vector2 = nullptr;
            break;
        case VariantType::Vector3:
            m_Vector3 = other.m_Vector3;
            other.m_Vector3 = nullptr;
            break;
        case VariantType::Vector4:
            m_Vector4 = other.m_Vector4;
            other.m_Vector4 = nullptr;
            break;
        case VariantType::Quaternion:
            m_Quaternion = other.m_Quaternion;
            other.m_Quaternion = nullptr;
            break;
        case VariantType::Matrix4x4:
            m_Matrix4x4 = other.m_Matrix4x4;
            other.m_Matrix4x4 = nullptr;
            break;
        default:
            m_Bool = other.m_Bool;
            break;
    }
    other.m_Type = VariantType::None;
}

bool Variant::GetBool() const {
    return m_Type == VariantType::Bool ? m_Bool : false;
}

int32 Variant::GetInt32() const {
    return m_Type == VariantType::Int32 ? m_Int32 : 0;
}

int64 Variant::GetInt64() const {
    return m_Type == VariantType::Int64 ? m_Int64 : 0LL;
}

uint32 Variant::GetUInt32() const {
    return m_Type == VariantType::UInt32 ? m_UInt32 : 0u;
}

uint64 Variant::GetUInt64() const {
    return m_Type == VariantType::UInt64 ? m_UInt64 : 0uLL;
}

float32 Variant::GetFloat32() const {
    return m_Type == VariantType::Float32 ? m_Float32 : 0.0f;
}

float64 Variant::GetFloat64() const {
    return m_Type == VariantType::Float64 ? m_Float64 : 0.0;
}

const String& Variant::GetString() const {
    static String empty;
    return m_Type == VariantType::String ? *m_String : empty;
}

const Vector2& Variant::GetVector2() const {
    static Vector2 empty;
    return m_Type == VariantType::Vector2 ? *m_Vector2 : empty;
}

const Vector3& Variant::GetVector3() const {
    static Vector3 empty;
    return m_Type == VariantType::Vector3 ? *m_Vector3 : empty;
}

const Vector4& Variant::GetVector4() const {
    static Vector4 empty;
    return m_Type == VariantType::Vector4 ? *m_Vector4 : empty;
}

const Quaternion& Variant::GetQuaternion() const {
    static Quaternion empty;
    return m_Type == VariantType::Quaternion ? *m_Quaternion : empty;
}

const Matrix4x4& Variant::GetMatrix4x4() const {
    static Matrix4x4 empty;
    return m_Type == VariantType::Matrix4x4 ? *m_Matrix4x4 : empty;
}

void Variant::SetBool(bool value) {
    Destroy();
    m_Type = VariantType::Bool;
    m_Bool = value;
}

void Variant::SetInt32(int32 value) {
    Destroy();
    m_Type = VariantType::Int32;
    m_Int32 = value;
}

void Variant::SetInt64(int64 value) {
    Destroy();
    m_Type = VariantType::Int64;
    m_Int64 = value;
}

void Variant::SetUInt32(uint32 value) {
    Destroy();
    m_Type = VariantType::UInt32;
    m_UInt32 = value;
}

void Variant::SetUInt64(uint64 value) {
    Destroy();
    m_Type = VariantType::UInt64;
    m_UInt64 = value;
}

void Variant::SetFloat32(float32 value) {
    Destroy();
    m_Type = VariantType::Float32;
    m_Float32 = value;
}

void Variant::SetFloat64(float64 value) {
    Destroy();
    m_Type = VariantType::Float64;
    m_Float64 = value;
}

void Variant::SetString(const String& value) {
    Destroy();
    m_Type = VariantType::String;
    m_String = new String(value);
}

void Variant::SetVector2(const Vector2& value) {
    Destroy();
    m_Type = VariantType::Vector2;
    m_Vector2 = new Vector2(value);
}

void Variant::SetVector3(const Vector3& value) {
    Destroy();
    m_Type = VariantType::Vector3;
    m_Vector3 = new Vector3(value);
}

void Variant::SetVector4(const Vector4& value) {
    Destroy();
    m_Type = VariantType::Vector4;
    m_Vector4 = new Vector4(value);
}

void Variant::SetQuaternion(const Quaternion& value) {
    Destroy();
    m_Type = VariantType::Quaternion;
    m_Quaternion = new Quaternion(value);
}

void Variant::SetMatrix4x4(const Matrix4x4& value) {
    Destroy();
    m_Type = VariantType::Matrix4x4;
    m_Matrix4x4 = new Matrix4x4(value);
}

void Variant::Clear() {
    Destroy();
}

}
