#ifndef KATARENGA_COMMON_CONFIG_CONFIGURATION_HPP
#define KATARENGA_COMMON_CONFIG_CONFIGURATION_HPP

// Standard Library
#include <map>

struct Configuration
{
    using Key = std::string;
    using Value = std::any;

    template<class T>
    T get(Key key) {
        if(!has(key))
            throw std::runtime_error("key not found");

        return std::any_cast<T>(map.at(key));
    }

    bool has(Key key) {
        return maps.find(key) != std::end(map);
    }

    template<class T>
    void insert(Key key, T value) {
        if(has(key))
            throw std::runtime_error("key not found");

        maps.insert(key, value);
    }

private:
    std::map<Key,Value> maps;

};

#endif // KATARENGA_COMMON_CONFIG_CONFIGURATION_HPP
