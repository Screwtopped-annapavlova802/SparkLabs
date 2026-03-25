#ifndef SPARKLABS_CORE_MAP_H
#define SPARKLABS_CORE_MAP_H

#include "Types.h"
#include "io/Vector.h"
#include "string/StringHash.h"

namespace SparkLabs {

template<typename K, typename V>
struct KeyValuePair {
    K key;
    V value;

    KeyValuePair() = default;
    KeyValuePair(const K& k, const V& v) : key(k), value(v) {}
    KeyValuePair(K&& k, V&& v) : key(std::move(k)), value(std::move(v)) {}
};

template<typename K, typename V>
class Map {
public:
    Map() = default;
    Map(const Map& other) = default;
    Map(Map&& other) noexcept = default;
    ~Map() = default;

    Map& operator=(const Map& other) = default;
    Map& operator=(Map&& other) noexcept = default;

    size_t Size() const { return m_Pairs.Size(); }
    bool Empty() const { return m_Pairs.Empty(); }
    void Clear() { m_Pairs.Clear(); }

    bool Contains(const K& key) const {
        for (const auto& pair : m_Pairs) {
            if (pair.key == key) return true;
        }
        return false;
    }

    V& operator[](const K& key) {
        for (auto& pair : m_Pairs) {
            if (pair.key == key) return pair.value;
        }
        m_Pairs.PushBack(KeyValuePair<K, V>(key, V()));
        return m_Pairs.Back().value;
    }

    const V* Find(const K& key) const {
        for (const auto& pair : m_Pairs) {
            if (pair.key == key) return &pair.value;
        }
        return nullptr;
    }

    V* Find(const K& key) {
        for (auto& pair : m_Pairs) {
            if (pair.key == key) return &pair.value;
        }
        return nullptr;
    }

    bool Get(const K& key, V& outValue) const {
        for (const auto& pair : m_Pairs) {
            if (pair.key == key) {
                outValue = pair.value;
                return true;
            }
        }
        return false;
    }

    void Set(const K& key, const V& value) {
        for (auto& pair : m_Pairs) {
            if (pair.key == key) {
                pair.value = value;
                return;
            }
        }
        m_Pairs.PushBack(KeyValuePair<K, V>(key, value));
    }

    void Remove(const K& key) {
        for (size_t i = 0; i < m_Pairs.Size(); ++i) {
            if (m_Pairs[i].key == key) {
                m_Pairs.Erase(i);
                return;
            }
        }
    }

    Vector<K> GetKeys() const {
        Vector<K> keys;
        for (const auto& pair : m_Pairs) {
            keys.PushBack(pair.key);
        }
        return keys;
    }

    Vector<V> GetValues() const {
        Vector<V> values;
        for (const auto& pair : m_Pairs) {
            values.PushBack(pair.value);
        }
        return values;
    }

    const Vector<KeyValuePair<K, V>>& GetPairs() const { return m_Pairs; }

private:
    Vector<KeyValuePair<K, V>> m_Pairs;
};

}

#endif
