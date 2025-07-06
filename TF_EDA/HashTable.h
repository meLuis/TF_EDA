#pragma once
#include <iostream>
#include <vector>
#include <list>
#include <functional>
#include <algorithm>
#include <string>
#include <stdexcept>

template <typename K, typename V>
class HashTable {
private:
    struct Node {
        K key;
        V value;

        Node(const K& k, const V& v) : key(k), value(v) {}
    };

    vector<list<Node>> buckets;
    size_t count;
    function<size_t(const K&)> hashFunc;
    float maxLoadFactor;

    size_t getBucketIndex(const K& key) const {
        return hashFunc(key) % buckets.size();
    }

    void rehash() {
        size_t newSize = buckets.size() * 2;
        vector<list<Node>> newBuckets(newSize);

        for (const auto& bucket : buckets) {
            for (const auto& node : bucket) {
                size_t newIndex = hashFunc(node.key) % newSize;
                newBuckets[newIndex].push_back(node);
            }
        }

        buckets = move(newBuckets);
    }

public:
    HashTable(
        size_t initialSize = 100,
        function<size_t(const K&)> hasher = [](const K& key) {
            return hash<K>{}(key);
        },
        float loadFactor = 0.75f
    ) : buckets(initialSize), count(0), hashFunc(hasher), maxLoadFactor(loadFactor) {
    }

    bool insert(const K& key, const V& value) {
        if (loadFactor() >= maxLoadFactor) {
            rehash();
        }

        size_t index = getBucketIndex(key);

        for (auto& node : buckets[index]) {
            if (node.key == key) {
                node.value = value;
                return false;
            }
        }

        buckets[index].push_back(Node(key, value));
        count++;
        return true;
    }

    V* find(const K& key) {
        size_t index = getBucketIndex(key);

        for (auto& node : buckets[index]) {
            if (node.key == key) {
                return &node.value;
            }
        }

        return nullptr;
    }

    const V* find(const K& key) const {
        size_t index = getBucketIndex(key);

        for (const auto& node : buckets[index]) {
            if (node.key == key) {
                return &node.value;
            }
        }

        return nullptr;
    }

    bool remove(const K& key) {
        size_t index = getBucketIndex(key);

        auto& bucket = buckets[index];
        for (auto it = bucket.begin(); it != bucket.end(); ++it) {
            if (it->key == key) {
                bucket.erase(it);
                count--;
                return true;
            }
        }

        return false;
    }

    bool contains(const K& key) const {
        return find(key) != nullptr;
    }

    vector<K> keys() const {
        vector<K> result;
        result.reserve(count);

        for (const auto& bucket : buckets) {
            for (const auto& node : bucket) {
                result.push_back(node.key);
            }
        }

        return result;
    }

    vector<V> values() const {
        vector<V> result;
        result.reserve(count);

        for (const auto& bucket : buckets) {
            for (const auto& node : bucket) {
                result.push_back(node.value);
            }
        }

        return result;
    }

    void forEach(function<void(V&)> func) {
        for (auto& bucket : buckets) {
            for (auto& node : bucket) {
                func(node.value);
            }
        }
    }

    vector<V> filter(function<bool(const V&)> predicate) const {
        vector<V> result;

        for (const auto& bucket : buckets) {
            for (const auto& node : bucket) {
                if (predicate(node.value)) {
                    result.push_back(node.value);
                }
            }
        }

        return result;
    }

    V& operator[](const K& key) {
        V* found = find(key);
        if (found) {
            return *found;
        }

        V defaultValue{};
        insert(key, defaultValue);
        return *find(key);
    }

    void clear() {
        for (auto& bucket : buckets) {
            bucket.clear();
        }
        count = 0;
    }

    size_t size() const {
        return count;
    }

    bool empty() const {
        return count == 0;
    }

    float loadFactor() const {
        return static_cast<float>(count) / buckets.size();
    }

    size_t bucketCount() const {
        return buckets.size();
    }

    void imprimirEstadisticas() const {
        cout << "Estadísticas de la Tabla Hash:" << endl;
        cout << "- Elementos: " << count << endl;
    }
};
