#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/scalar_constants.hpp"
#include "glm/geometric.hpp"
#include "mesh.hpp"
#include "dynamics.hpp"
#include <iostream>
#include <tgmath.h>
#define STB_IMAGE_IMPLEMENTATION
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "camera.hpp"
#include "shader.hpp"
#include "model.hpp"
#include "asset_manager.hpp"

#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include "game_object.hpp"
#include "player.hpp"
#include "constants.hpp"
#include "light.hpp"


void framebuffer_size_callback(GLFWwindow* window, int width, int height); 

Player* player;

glm::vec3 getCenter()
{
    btTransform trans;
    player->getWorldTransform(trans);
    return convert(trans.getOrigin());
}

Dynamics* world;

AssetManager* manager;

unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad()
{
    if (quadVAO == 0)
    {
        float quadVertices[] = {
            // positions        // texture Coords
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
             1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
             1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };
        // setup plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}


/**
 * Initializes the GLFW Window and starts up GLAD
 */
GLFWwindow* initWindow()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); 


	//Initialization of Window
	//------------------------
	#ifdef DEBUG
        std::cout << "Initializing Window..." << std::endl;
    #endif
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "PushGame", NULL, NULL);
	if (window == NULL)
	{
        std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
        std::exit(1);
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
        std::cerr << "Failed to initialize GLAD" << std::endl;
		std::exit(1);
	}   

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); 



	#ifdef DEBUG 
	int numAttr;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &numAttr);
        std::cout << "Max Vertex Attributes supported: " << numAttr << std::endl;
    #endif
    return window;
}

int main()
{
    //GLFW    
    GLFWwindow* window = initWindow();
    
    //model loader
    manager = new AssetManager{};

    //Initialize Bullet
    //-----------------
    world = new Dynamics(btVector3(0,-10,0));

    std::vector<BaseObject*> objects;
    std::vector<DirectionalLight> dirLights;
    std::vector<PointLight> pointLights;

	//Generation of the Shader Program
	//--------------------------------
	
    // directional light
    dirLights.push_back(DirectionalLight
        {
            glm::vec3{-0.2f, -1.0f, -0.3f}, 
            &getCenter,
            glm::vec3{0.05f, 0.05f, 0.05f},
            glm::vec3{0.4f, 0.4f, 0.4f},
            glm::vec3{0.2f, 0.2f, 0.2f}
        });

    // point lights
    /*
    pointLights.push_back(PointLight
        {
            glm::vec3{0.7f, 30.2f, 2.0f},
            glm::vec3{0.05f, 0.05f, 0.05f},
            glm::vec3{0.8f, 0.8f, 0.8f},
            glm::vec3{1.0f, 1.0f, 1.0f},
            1.0f, 0.10f, 0.010f
        });
    pointLights.push_back(PointLight
        {
            glm::vec3{-4.0f,  32.0f, -12.0f},
            glm::vec3{0.05f, 0.05f, 0.05f},
            glm::vec3{0.8f, 0.8f, 0.8f},
            glm::vec3{1.0f, 1.0f, 1.0f},
            1.0f, 0.7f, 1.8f
        });
    pointLights.push_back(PointLight
        {
            glm::vec3{0.0f,  30.0f, -3.0f},
            glm::vec3{0.05f, 0.05f, 0.05f},
            glm::vec3{0.8f, 0.8f, 0.8f},
            glm::vec3{1.0f, 1.0f, 1.0f},
            1.0f, 0.7f, 1.8f
        });
    pointLights.push_back(PointLight
        {
            glm::vec3{2.3f, 27.3f, -4.0f},
            glm::vec3{0.05f, 0.05f, 0.05f},
            glm::vec3{0.8f, 0.8f, 0.8f},
            glm::vec3{1.0f, 1.0f, 1.0f},
            1.0f, 0.7f, 1.8f
        });
    */
	Shader shader(dirLights.size(), pointLights.size(), 
            "assets/gl/object.vs", "assets/gl/object.fs");

    Shader debugDepthQuad(0,0,"assets/gl/debug_quad.vs", "assets/gl/debug_quad.fs");
    Shader lampShader(0,0, "assets/gl/lamp.vs", "assets/gl/lamp.fs");

    btTransform trans;
    trans.setIdentity();
    trans.setOrigin(btVector3{10.0f,20.0f,10.0f});
    player = new Player{manager, trans, glm::vec3{0,0,1}, UP};
    player->softInit();
    objects.push_back(player);
    // Add player's processInputs to the input managers
    InputManager::objects_[PLAYER_INPUT_INDEX] = player;

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, InputManager::mouseCallback);

    // After we generate shaders load objects
    StaticObject* floor = new StaticObject{nullptr, manager, 
        "assets/Models/Playground/hills.obj","assets/Models/Playground/hills_shape.obj",
        trans};    
    floor->softInit();
    objects.push_back(floor);

    const int NUMCUBES = 10;
    DynamicObject* dynamicCubes[NUMCUBES];
    btCollisionShape* cubeShape = new btBoxShape{btVector3{1,1,1}};
    for(int i = 0; i < NUMCUBES; ++i)
    {
        btTransform trans;
        trans.setIdentity();
        trans.setOrigin(btVector3{0, 10*i, 0});
        dynamicCubes[i] = new DynamicObject{nullptr, manager, cubeShape, 10,
            "assets/Models/cube/cube.obj", trans};    
        dynamicCubes[i]->softInit();
        objects.push_back(dynamicCubes[i]);
    }

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);


    for(auto i = objects.begin(); i != objects.end(); i++)
    {
        BaseObject* obj = *i;
        obj->hardInit(world);
    }


    Model& cube = *manager->get("assets/Models/cube/cube.obj");

    float dt = 0.0f;
    float lastFrame = 0.0f;

    //projection: generates frustum	
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 
        ((float)SCR_WIDTH)/((float)SCR_HEIGHT), 0.1f, 1000.0f); 

    debugDepthQuad.use();
    debugDepthQuad.setInt("depthMap", 0);

	//Main Loop
	//---------
	while(!glfwWindowShouldClose(window))
	{
        // calculate dt
        float currentFrame = glfwGetTime();
        dt = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // bullet physics
        world->stepSimulation(dt);
            
        // Update objects
        for(auto i = objects.begin(); i != objects.end(); i++)
        {
            (*i)->Update(world, dt);
        }
        

		//Clear
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Compute shadows
        for(int i = 0; i < (int)dirLights.size(); i++)
        {
            dirLights.at(i).renderShadows(objects);
        }

        //Reset Viewport Size and clear again
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glViewport(0,0, SCR_WIDTH, SCR_HEIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Draw
        shader.use();

        // Apply any changes to lights
        for(int i = 0; i < (int)dirLights.size(); i++)
        {
            dirLights.at(i).updateShader(shader, i);
        }
        for(int i = 0; i < (int)pointLights.size(); i++)
        {
            pointLights.at(i).updateShader(shader, i, dirLights.size());
        }
		
		//view: the inverse transform of the camera's local transformation
		glm::mat4 view = player->getCameraView();
		shader.setMat4("projection", projection);
		shader.setMat4("view", view);
        shader.setVec3("viewPos", player->getCameraPos());

        // Render the objects
        for(auto i = objects.begin(); i != objects.end(); i++)
        {
            (*i)->Draw(shader, dirLights.size());
        }
        

        debugDepthQuad.use();
        debugDepthQuad.setFloat("near_plane", NEAR_SHADOW_CLIPPING_PLANE);
        debugDepthQuad.setFloat("far_plane", FAR_SHADOW_CLIPPING_PLANE);
        glActiveTexture(GL_TEXTURE0);
        //glBindTexture(GL_TEXTURE_2D, shadow_casters.front()->depthMap_);
        //renderQuad();



        // lamp
        lampShader.use();
        lampShader.setMat4("projection", projection);
        lampShader.setMat4("view", view);
    
        for (unsigned int i = 0; i < pointLights.size(); i++) 
        {
            glm::mat4 model{1.0f};
            model = glm::translate(model, pointLights[i].position_);
            model = glm::scale(model, glm::vec3{0.2f});
            lampShader.setMat4("model", model);
            cube.Draw(lampShader, 0);
        }

        #ifdef DEBUG
            world->debugDraw(projection, view);
        #endif 
    
		//Handle events
		glfwSwapBuffers(window);
		glfwPollEvents();    
        
		//poll inputs
		InputManager::pollInput(window, dt);
	}

    player->softDestruct(world);
    floor->softDestruct(world);
    for(int i = 0; i < NUMCUBES; i++)
    {
        dynamicCubes[i]->softDestruct(world);
    }
	
	#ifdef DEBUG 
        std::cerr << "Closing Window" << std::endl;
    #endif

    objects.clear();

    // delete the world before the collision objects!
    delete world;
    #ifdef DEBUG
        std::cerr << "Successfully deleted world" << std::endl;
    #endif
    delete player;
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
    delete cubeShape;
    #ifdef DEBUG
        std::cerr << "Successfully deleted cube shape" << std::endl;
    #endif
    // delete models before the model manager!
    delete manager;
    #ifdef DEBUG
        std::cerr << "Successfully deleted asset manager" << std::endl;
    #endif

    glfwDestroyWindow(window);
	glfwTerminate();
    #ifdef DEBUG
        std::cerr << "Successfully destroyed window" << std::endl;
    #endif
}

std::ostream& operator<<(std::ostream& os, const glm::vec3& vec)
{
    os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
    return os;
} 

void framebuffer_size_callback(GLFWwindow*, int width, int height)
{
    glViewport(0, 0, width, height);
}

