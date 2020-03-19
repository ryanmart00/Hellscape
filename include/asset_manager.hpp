#ifndef ASSET_MANAGER_HPP
#define ASSET_MANAGER_HPP

class AssetManager
{
public:
    AssetManager();
    ~AssetManager();

    void load(const char* file);

    void* get(const char* file) const;
    
    bool contains(const char* file);

private:
    std::unordered_set
};
#endif
