#ifndef ASSET_MANAGER_HPP
#define ASSET_MANAGER_HPP

#include "model.hpp"
#include <unordered_map>
#include <string>
#include <future>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "constants.hpp"

class AssetManager
{
public:
    AssetManager() = default;
    ~AssetManager();
    AssetManager(const AssetManager&) = delete;
    AssetManager& operator=(const AssetManager&) = delete;
    AssetManager(AssetManager&&) noexcept = delete;
    AssetManager& operator=(AssetManager&&) noexcept = delete;

    /**
     * Load's the model found in file asyncronously.
     */  
    void load(const std::string& file);

    /**
     * Returns a pointer to the loaded model in file
     *
     * get blocks until the model is loaded so, if you want 
     * async model loading call load(file) prior to calling get!
     *
     * The asset manager keeps track of how many times get was called 
     * so that it will keep the model in memory while it is being used.
     */
    Model* get(const std::string& file);
    
    /** 
     * Decrements one from file's reference count. If the reference count drops to 0, 
     * the asset manager is free to delete the model to conserve space
     */
    void unload(const std::string& file);

    /**
     * Returns true iff the model stored in file has been loaded and not yet deleted.
     */
    bool contains(const std::string& file);

private:

/*
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
*/

    struct ModelCount
    {
        ModelCount(int count, Model* model) : count_{count}, model_{model} {} 
    
        int count_;
        Model* model_;
    };
    
    std::unordered_map<std::string, ModelCount> models_;

    std::unordered_map<std::string, std::future<Model*>> futures_;
};

    /** InputManager Details
     *  Add to this namespace for each input object, 
     *  - a pointer to the object in objects_
     *  - a pointer to the mouse callback in mouseCallbacks_
     *  - a pointer to the inputPoller in inputPollers_
     *      Input pollers 
     *
     *  Kinda hacky but for now just so that things don't get wonky, InputManager will also 
     *  handle game state: the idea is that there is probably a unique input handler for each 
     *  game state right?
     */
const unsigned int NUM_STATES = 2;
enum GameState
{
    PLAYING = 0,
    SETTINGS = 1 
};


class Input
{
public:
    GameState& state_;

    Input(GameState& state)
        : state_{state}
    {
    };
    virtual void mouseCallback(GLFWwindow*, double, double) = 0;
    virtual void mouseButtonCallback(GLFWwindow*, int, int, int) = 0;
    virtual void pollInput(GLFWwindow*, float) = 0;

};


#endif
