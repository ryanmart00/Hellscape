#include "asset_manager.hpp"

AssetManager::~AssetManager()
{
    // clear out any unloaded models
    auto i = futures_.begin();
    while(i != futures_.end())
    {
        delete i->second.get();
        i = futures_.erase(i);
    }
    
    // clear out loaded models
    auto j = models_.begin();
    while(j != models_.end())
    {
        delete j->second.model_;
        j = models_.erase(j);
    }
    
}

Model* AssetManager::get(const std::string& file) 
{
    std::cout << "Getting " << file << std::endl;
    auto i = models_.find(file);
    if (i == models_.end())
    {
        std::cout << "Not in models_: " << file << std::endl;
        auto j = futures_.find(file);
        if (j == futures_.end())
        {
            std::cout << "Not in futures_: " << file << std::endl;
            Model* model = new Model(file);
//            model->setupModel();
            ModelCount mc{1, model};
            models_.emplace(file, mc);
            return model;
        }
        else
        {
            std::cout << "Wait for future: " << j->second.valid() << std::endl;
            j->second.wait();
            std::cout << "Waiting complete: " << file << std::endl;
            Model* model = j->second.get();
//            model->setupModel();
            std::cout << "Retrieved model from future: " << file << std::endl;
            ModelCount mc{1, model};
            models_.emplace(file, mc);
            return model;
        }   
    }
    else
    {
        std::cout << "Already loaded" << file << std::endl;
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
            std::cout << "Loading " << file << std::endl;
            Model* model = new Model(file);
            std::cout << "Loading complete for " << file << std::endl;
            return model;
        });
    }
}

void AssetManager::unload(const std::string& file)
{
    auto i = models_.find(file);
    if(i != models_.end())
    {
        delete i->second.model_;
        models_.erase(i);
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

