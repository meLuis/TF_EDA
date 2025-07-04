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

    std::vector<std::list<Node>> buckets;
    size_t count;
    std::function<size_t(const K&)> hashFunc;
    float maxLoadFactor;

    size_t getBucketIndex(const K& key) const {
        return hashFunc(key) % buckets.size();
    }

    void rehash() {
        size_t newSize = buckets.size() * 2;
        std::vector<std::list<Node>> newBuckets(newSize);

        for (const auto& bucket : buckets) {
            for (const auto& node : bucket) {
                size_t newIndex = hashFunc(node.key) % newSize;
                newBuckets[newIndex].push_back(node);
            }
        }

        buckets = std::move(newBuckets);
    }

public:
    HashTable(
        size_t initialSize = 100,
        std::function<size_t(const K&)> hasher = [](const K& key) {
            return std::hash<K>{}(key);
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

    std::vector<K> keys() const {
        std::vector<K> result;
        result.reserve(count);

        for (const auto& bucket : buckets) {
            for (const auto& node : bucket) {
                result.push_back(node.key);
            }
        }

        return result;
    }

    std::vector<V> values() const {
        std::vector<V> result;
        result.reserve(count);

        for (const auto& bucket : buckets) {
            for (const auto& node : bucket) {
                result.push_back(node.value);
            }
        }

        return result;
    }

    void forEach(std::function<void(V&)> func) {
        for (auto& bucket : buckets) {
            for (auto& node : bucket) {
                func(node.value);
            }
        }
    }

    std::vector<V> filter(std::function<bool(const V&)> predicate) const {
        std::vector<V> result;

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
        std::cout << "Estadísticas de la Tabla Hash:" << std::endl;
        std::cout << "- Elementos: " << count << std::endl;
        std::cout << "- Buckets: " << buckets.size() << std::endl;
        std::cout << "- Factor de carga: " << loadFactor() << std::endl;

        size_t bucketsVacios = 0;
        size_t tamanioBucketMaximo = 0;
        for (const auto& bucket : buckets) {
            if (bucket.empty()) {
                bucketsVacios++;
            }
            else {
                tamanioBucketMaximo = std::max(tamanioBucketMaximo, bucket.size());
            }
        }

        std::cout << "- Buckets vacíos: " << bucketsVacios << " ("
            << (static_cast<float>(bucketsVacios) / buckets.size() * 100) << "%)" << std::endl;
        std::cout << "- Tamaño máximo de bucket: " << tamanioBucketMaximo << std::endl;
    }
};
