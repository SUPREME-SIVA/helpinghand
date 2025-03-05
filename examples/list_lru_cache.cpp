#include <iostream>
#include <list>
#include <unordered_map>
#include <string>

// Example 2: LRU (Least Recently Used) Cache Implementation
// A real-world application that benefits from list's efficient insertion/removal
// and iterator stability characteristics

template <typename KeyType, typename ValueType>
class LRUCache {
private:
    // Maximum size of the cache
    size_t m_capacity;
    
    // List to store key-value pairs in order of use
    // Most recently used items at the front, least recently used at the back
    std::list<std::pair<KeyType, ValueType>> m_items;
    
    // Map to quickly look up list iterators by key
    std::unordered_map<KeyType, typename std::list<std::pair<KeyType, ValueType>>::iterator> m_lookup;
    
public:
    // Constructor with capacity parameter
    LRUCache(size_t capacity) : m_capacity(capacity) {}
    
    // Get value by key, returns nullptr if not found
    ValueType* get(const KeyType& key) {
        auto it = m_lookup.find(key);
        
        // If key is not in cache, return nullptr
        if (it == m_lookup.end()) {
            return nullptr;
        }
        
        // Move the accessed item to the front of the list (most recently used)
        // This is efficient with list's splice operation - O(1)
        m_items.splice(m_items.begin(), m_items, it->second);
        
        // Return pointer to value
        return &(it->second->second);
    }
    
    // Put key-value pair in cache
    void put(const KeyType& key, const ValueType& value) {
        // Check if key already exists
        auto it = m_lookup.find(key);
        
        if (it != m_lookup.end()) {
            // Key exists, update value and move to front
            it->second->second = value;
            m_items.splice(m_items.begin(), m_items, it->second);
        } else {
            // Check if cache is full
            if (m_items.size() >= m_capacity) {
                // Remove least recently used item (from back of list)
                // List makes this operation efficient - O(1)
                auto last = m_items.back();
                m_lookup.erase(last.first);
                m_items.pop_back();
            }
            
            // Insert new item at front
            m_items.push_front(std::make_pair(key, value));
            m_lookup[key] = m_items.begin();
        }
    }
    
    // Display current cache contents
    void display() const {
        std::cout << "Cache contents (Most recently used first):" << std::endl;
        for (const auto& item : m_items) {
            std::cout << item.first << " : " << item.second << std::endl;
        }
        std::cout << std::endl;
    }
};

int main() {
    // Create an LRU cache with capacity 3
    LRUCache<std::string, int> cache(3);
    
    std::cout << "Adding (A,1), (B,2), (C,3) to cache" << std::endl;
    cache.put("A", 1);
    cache.put("B", 2);
    cache.put("C", 3);
    cache.display();
    
    std::cout << "Accessing B" << std::endl;
    int* value = cache.get("B");
    if (value) {
        std::cout << "Value for B: " << *value << std::endl;
    }
    cache.display();  // B should move to the front
    
    std::cout << "Adding (D,4) - should evict least recently used (A)" << std::endl;
    cache.put("D", 4);
    cache.display();
    
    std::cout << "Trying to access A (which should be evicted)" << std::endl;
    value = cache.get("A");
    if (value) {
        std::cout << "Value for A: " << *value << std::endl;
    } else {
        std::cout << "A is not in cache" << std::endl;
    }
    
    std::cout << "Updating C to 30" << std::endl;
    cache.put("C", 30);
    cache.display();  // C should move to the front
    
    return 0;
}
