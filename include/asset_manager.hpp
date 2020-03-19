#ifndef ASSET_MANAGER_HPP
#define ASSET_MANAGER_HPP

#include <unordered_map>
#include <string>
#include "model.hpp"

class AssetManager
{
public:
    AssetManager();
    ~AssetManager();

    void load(const std::string file);

    Model* get(const std::string file) const;
    
    bool contains(const std::string file);

private:
    struct MyHash
    {
        const uint32_t Prime = 0x01000193;  //   16777619
        const uint32_t Seed = 0x811C9DC5;   // 2166136261
        const uint32_t ModPrime = 2147483647;

        std::size_t operator()(const std::string& data) const noexcept
        {
            uint32_t hash = Seed;
            const char* ptr = data.c_str();
            size_t numBytes = data.size();
            while (numBytes--) 
            {
                hash = (*ptr++ ^ hash) * Prime;
            }
            return hash % ModPrime;
        }

    };
    std::unordered_map<const char*, Model*, MyHash> map_;
};
#endif
