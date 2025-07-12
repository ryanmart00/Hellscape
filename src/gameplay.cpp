#include "gameplay.hpp"

#include "constants.hpp"
#include "glm/ext/quaternion_trigonometric.hpp"
#include "glad/gl.h"
#include "glm/gtc/quaternion.hpp"

Gameplay::Gameplay(GameState& state, Input** inputs, AssetManager& man, int width, int height)
    : Input{state, inputs}, manager_{man}, srcwidth{width}, srcheight{height}
{
    //Initialize Bullet
    //-----------------
    world = new Dynamics{btVector3(0,-10,0)};

    lampShader_ = new Shader(0,0, "assets/gl/lamp.vs", "assets/gl/lamp.fs");
    text_ = new Text("assets/gl/text.vs", "assets/gl/text.fs");

	//Generation of the Shader Program
	//--------------------------------
    // directional light
    /*
    dirLights.push_back(DirectionalLight
        {
            glm::vec3{-0.2f, -1.0f, -0.3f}, 
            glm::vec3{0.05f, 0.05f, 0.05f},
            glm::vec3{0.4f, 0.4f, 0.4f},
            glm::vec3{0.2f, 0.2f, 0.2f}
        });
    dirLights.push_back(DirectionalLight
        {
            glm::vec3{-1, -0.1f, 0}, 
            glm::vec3{0.05f, 0.05f, 0.05f},
            glm::vec3{0.4f, 0.4f, 0.4f},
            glm::vec3{0.2f, 0.2f, 0.2f}
        });
        */

    // point lights
    pointLights.push_back(PointLight
        {
            glm::vec3{0.7f, 2.2f, 2.0f},
            glm::vec3{0.05f, 0.05f, 0.05f},
            glm::vec3{0.8f, 0.8f, 0.8f},
            glm::vec3{1.0f, 1.0f, 1.0f},
            1.0f, 0.009f, 0.00032f
        });
    pointLights.push_back(PointLight
        {
            glm::vec3{-4.0f,  5.0f, -12.0f},
            glm::vec3{0.05f, 0.05f, 0.05f},
            glm::vec3{0.8f, 0.8f, 0.8f},
            glm::vec3{1.0f, 1.0f, 1.0f},
            1.0f, 0.009f, 0.00032f
        });
    pointLights.push_back(PointLight
        {
            glm::vec3{30.0f,  10.0f, -3.0f},
            glm::vec3{0.05f, 0.05f, 0.05f},
            glm::vec3{0.8f, 0.8f, 0.8f},
            glm::vec3{1.0f, 1.0f, 1.0f},
            1.f, 0.009f, 0.00032f
        });
    pointLights.push_back(PointLight
        {
            glm::vec3{2.3f, 5.0f, -40.0f},
            glm::vec3{0.05f, 0.05f, 0.05f},
            glm::vec3{0.8f, 0.8f, 0.8f},
            glm::vec3{1.0f, 1.0f, 1.0f},
            1.0f, 0.009f, 0.00032f
        });
	
    shader_ = new Shader{(int)dirLights.size(), (int)pointLights.size(), 
        "assets/gl/object_debug.vs", "assets/gl/object_debug.fs"};

    {
        btTransform trans;
        trans.setIdentity();
        trans.setOrigin(btVector3{00.0f,1.0f,0.0f});
        player_ = new Player{manager_, trans, glm::vec3{0,0,1}, UP, state};
        objects.push_back(player_);
    }
    btTransform trans;
    trans.setIdentity();
    // After we generate shaders load objects
    trans.setOrigin(btVector3{0.0f,00.0f,0.0f});
    
    floor = new StaticObject{nullptr, manager_, 
        "assets/Models/World/CLOSED MAP V1 TEST2.obj",
        "assets/Models/World/CLOSED MAP V1 TEST2.obj", trans};    
    
    objects.push_back(floor);

    btCollisionShape* cubeShape = new btBoxShape{btVector3{1,1,1}};

    for(int i = 0; i < NUMCUBES; ++i)
    {
        btTransform trans;
        trans.setIdentity();
        trans.setOrigin(btVector3{0, 10.0f*i, 0});
        dynamicCubes[i] = new DynamicObject{nullptr, manager_, cubeShape, 100,
            "assets/Models/cube/cube.obj", trans};    
        objects.push_back(dynamicCubes[i]);
    }
    // Bone Test
    trans.setIdentity();
    trans.setOrigin(btVector3{10.0f,-30.0f,10.0f});
    StaticObject* boneTest = new StaticObject{nullptr, manager_, 
        "assets/Models/door down angled.fbx",
        "assets/Models/door down angled.fbx", trans};
    objects.push_back(boneTest);
    bone = objects.size()-1;


    //projection: generates frustum	
    projection = glm::perspective(glm::radians(45.0f), 
        ((float)srcwidth)/((float)srcheight), 0.1f, 1000.0f); 
}


glm::vec3 Gameplay::getCenter()
{
    btTransform trans;
    player_->getWorldTransform(trans);
    return convert(trans.getOrigin());
}

Gameplay::~Gameplay()
{
    floor->softDestruct(world); 
    player_->softDestruct(world);

    objects.clear();

    // delete the world before the collision objects!
    delete world;
    #ifdef DEBUG
        std::cerr << "Successfully deleted world" << std::endl;
    #endif
    delete player_;
    #ifdef DEBUG
        std::cerr << "Successfully deleted player" << std::endl;
    #endif
    delete floor;
    #ifdef DEBUG
        std::cerr << "Successfully deleted floor" << std::endl;
    #endif
    for(int i = 0; i < NUMCUBES; i++)
    {
        delete dynamicCubes[i];
    }
    #ifdef DEBUG
        std::cerr << "Successfully deleted all dynamicCubes" << std::endl;
    #endif
    #ifdef DEBUG
        std::cerr << "Successfully deleted cube shape" << std::endl;
    #endif


}

void Gameplay::framebuffer_size_callback(GLFWwindow*, int width, int height)
{
    srcwidth= width;
    srcheight = height;
    glViewport(0, 0, width, height);
}

void Gameplay::mouseCallback(GLFWwindow*, double xpos, double ypos)
{
    // update camera position
    if(firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    double dx = -xpos + lastX;
    double dy = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    dx *= PLAYER_MOUSE_SENSITIVITY;
    dy *= PLAYER_MOUSE_SENSITIVITY;

    //TODO: stop player from looking all the way around vertically.
    glm::vec3 right = quatRight(player_->cam_);
    player_->cam_ = glm::angleAxis((float)dx, UP) * glm::angleAxis((float)dy, right)* 
        player_->cam_;
 
}
void Gameplay::mouseButtonCallback(GLFWwindow*, int button, int action, int)
{
    
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        // create ray cast
        // check collisions 
        // do something -- increase a counter?
        // incrementPoints();
        return;
    }

    return;
}
void Gameplay::pollInput(GLFWwindow* window, float dt)
{
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
        Input::contextSwitch(GameState::SETTINGS);
	}
	if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
        glm::vec3 v = 160*PLAYER_MASS * glm::cross(UP,quatRight(player_->cam_)) ;
        player_->rigidBody_->applyCentralForce(btVector3{v.x, v.y, v.z});
    }
	if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
//        cam.position_ -= glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), cam.getRight()) * dt * 5.0f;
        glm::vec3 v = -160*PLAYER_MASS * glm::cross(UP,quatRight(player_->cam_)) ;
        player_->rigidBody_->applyCentralForce(btVector3{v.x, v.y, v.z});
	}
	if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
        glm::vec3 v = -160*PLAYER_MASS * quatRight(player_->cam_);
        player_->rigidBody_->applyCentralForce(btVector3{v.x, v.y, v.z});
//        cam.position_ -= cam.getRight() * dt * 7.0f;
	}
	if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
        glm::vec3 v = 160*PLAYER_MASS * quatRight(player_->cam_);
        player_->rigidBody_->applyCentralForce(btVector3{v.x, v.y, v.z});
//        cam.position_ += cam.getRight() * dt * 7.0f;
	}
    if(glfwGetKey(window,GLFW_KEY_SPACE) == GLFW_PRESS && player_->grounded) 
    {
//        cam.position_ += glm::vec3(0.0f,1.0f,0.0f) * dt * 7.0f;

        player_->rigidBody_->applyCentralImpulse(PLAYER_MASS*btVector3{0,10,0});
    }
    if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) 
    {
//        cam.position_ -= glm::vec3(0.0f,1.0f,0.0f) * dt * 7.0f;
    }
    if(glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
    {
        std::cout << "(" << player_->rigidBody_->getCenterOfMassPosition().getX() 
            << "," << player_->rigidBody_->getCenterOfMassPosition().getY() << "," 
            << player_->rigidBody_->getCenterOfMassPosition().getZ() << ")" << std::endl;
    }
}

void Gameplay::characterCallback(GLFWwindow*, unsigned int point)
{
    std::cout << point << std::endl;
}

void Gameplay::softInit()
{
    for(auto i = objects.begin(); i != objects.end(); i++)
    {
        BaseObject* obj = *i;
        obj->softInit();
    }
}

void Gameplay::hardInit()
{
#ifdef DEBUG
    std::cout << "Gameplay init" << std::endl;
#endif 
    for(auto i = objects.begin(); i != objects.end(); i++)
    {
        BaseObject* obj = *i;
        obj->hardInit(world);
    }
    StaticObject* boneTest = (StaticObject*)objects[bone];
    std::cout << *(boneTest->model_) << std::endl;
    for(auto i = boneTest->model_->meshes_.begin(); i != boneTest->model_->meshes_.end(); i++)
    {
        std::cout << "Mesh" << std::endl;
        for(auto j = i->bones_.begin(); j != i->bones_.end(); j++)
        {
            std::cout << j->name << std::endl;
            std::cout << j->offset << std::endl;
        }

    }

    shader_->use();
    shader_->setFloat("far_plane", FAR_SHADOW_CLIPPING_PLANE);
}

void Gameplay::update(float dt)
{
    Model& cube = *manager_.get("assets/Models/cube/cube.obj");
    StaticObject* boneTest = (StaticObject*)objects[bone];
    boneTest->model_->meshes_[0].bones_[0].offset *= glm::mat4_cast(glm::angleAxis(dt, UP));
    boneTest->model_->meshes_[0].bones_[1].offset *= glm::mat4(glm::angleAxis(2*dt, FORWARD));

    // bullet physics
    world->stepSimulation(dt);
        
    // Update objects
    for(auto i = objects.begin(); i != objects.end(); i++)
    {
        (*i)->Update(world, dt);
    }
    player_->Update(world, dt);
    

    //Clear
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_CULL_FACE);  

    // Compute shadows
    for(int i = 0; i < (int)dirLights.size(); i++)
    {
        dirLights.at(i).renderShadows(objects);
    }
    for(int i = 0; i < (int)pointLights.size(); i++)
    {
        pointLights.at(i).renderShadows(objects);
    }

    //Reset Viewport Size and clear again
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glViewport(0,0, srcwidth, srcheight);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //Draw
    shader_->use();

    // Apply any changes to lights
    for(int i = 0; i < (int)dirLights.size(); i++)
    {
        dirLights.at(i).updateShader(shader_, i, player_->getCameraPos());
    }
    for(int i = 0; i < (int)pointLights.size(); i++)
    {
        pointLights.at(i).updateShader(shader_, i, dirLights.size());
    }
    
    //view: the inverse transform of the camera's local transformation
    glm::mat4 view = player_->getCameraView();
    shader_->setMat4("projection", projection);
    shader_->setMat4("view", view);
    shader_->setVec3("viewPos", player_->getCameraPos());

    // Render the objects
    for(auto i = objects.begin(); i != objects.end(); i++)
    {
        (*i)->Draw(shader_, dirLights.size() + pointLights.size());
    }
    
    // lamp
    lampShader_->use();
    lampShader_->setMat4("projection", projection);
    lampShader_->setMat4("view", view);

    for (unsigned int i = 0; i < pointLights.size(); i++) 
    {
        glm::mat4 model{1.0f};
        model = glm::translate(model, pointLights[i].position_);
        model = glm::scale(model, glm::vec3{0.2f});
        lampShader_->setMat4("model", model);
        cube.Draw(lampShader_, 0);
    }

    //Show FPS
    text_->use();
    text_->setVec4("color", glm::vec4{1,1,1,.5f});
    float sx = 2.0 / srcwidth;
    float sy = 2.0 / srcheight;

    text_->renderText((std::string("FPS: ")
                + std::to_string((int)(1/dt))).c_str(),
                -1 + 8 * sx, 1 - 50 * sy, sx, sy);

#ifdef DEBUG
    world->debugDraw(projection, view);
#endif 

}

void Gameplay::stop()
{
    for(auto i = objects.begin(); i != objects.end(); i++)
    {
        BaseObject* obj = *i;
        obj->softDestruct(world);
    }

}
