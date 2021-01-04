#include "asset_manager.hpp"

AssetManager::~AssetManager()
{
    // clear out any unloaded models
    auto i = futures_.begin();
    while(i != futures_.end())
    {
        Model* m = i->second.get();
        i = futures_.erase(i);
        delete m;
    }
    
    // clear out loaded models
    auto j = models_.begin();
    while(j != models_.end())
    {
        Model* m = j->second.model_;
        j = models_.erase(j);
        delete m;
    }
    
}

Model* AssetManager::get(const std::string& file) 
{
    auto i = models_.find(file);
    if (i == models_.end())
    {
        // If we have yet to finish loading the models
        auto j = futures_.find(file);
        if (j == futures_.end())
        {
            // We still need to load it
            Model* model = new Model(file);
            model->initializeGL();
            ModelCount mc{1, model};
            models_.emplace(file, mc);
            return model;
        }
        else
        {
            // We need to wait for the future
            #ifdef DEBUG
                std::cerr << "Wait for future: " << j->second.valid() << std::endl;
            #endif
            j->second.wait();
            #ifdef DEBUG
                std::cerr << "Waiting complete: " << file << std::endl;
            #endif
            Model* model = j->second.get();
            model->initializeGL();
            #ifdef DEBUG
                std::cerr << "Retrieved model from future: " << file << std::endl;
            #endif 
            j = futures_.erase(j);
            ModelCount mc{1, model};
            models_.emplace(file, mc);
            return model;
        }   
    }
    else
    {
        ModelCount& mc = models_.at(file);
        mc.count_++;
        return mc.model_;
    }
}

void AssetManager::load(const std::string& file)
{
    if(models_.find(file) != models_.end() || futures_.find(file) != futures_.end())
    {
        // this file was already loaded
        return;
    }
    else
    {
        futures_[file] = std::async(std::launch::async, [file]
        {
            #ifdef DEBUG
                std::cout << "Loading " << file << std::endl;
            #endif
            Model* model = new Model(file);
            #ifdef DEBUG
                std::cout << "Loading complete for " << file << std::endl;
            #endif 
            return model;
        });
    }
}

void AssetManager::unload(const std::string& file)
{
    auto i = models_.find(file);
    if(i != models_.end())
    {
        
        if (--i->second.count_ == 0)
        {
            delete i->second.model_;
            models_.erase(i);
        }
    }
    else
    {
        auto j = futures_.find(file); 
        if (j != futures_.end())
        {
            delete j->second.get();
            futures_.erase(j);
        }
    }
}

