#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/scalar_constants.hpp"
#include "glm/geometric.hpp"
#include "mesh.hpp"
#include "dynamics.hpp"
#include <iostream>
#include <tgmath.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
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

#define DEBUG

using namespace std;

void processInput(GLFWwindow *window, Camera& cam, float dt);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void loadTexture(unsigned int& texture, const char* file, bool hasAlpha);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);


const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

Camera* cam;

Dynamics* world;

AssetManager* manager;

btRigidBody* addBox(btVector3 dim, float x, float y, float z, float mass)
{
    btTransform trans;
    trans.setIdentity();
    trans.setOrigin(btVector3(x,y,z));
    btBoxShape* box = new btBoxShape(dim);
    btVector3 inertia(0,0,0);
    box->calculateLocalInertia(mass, inertia);
    btMotionState* motion = new btDefaultMotionState(trans);
    btRigidBody::btRigidBodyConstructionInfo info{mass, motion, box, inertia};
    btRigidBody* body = new btRigidBody(info);
    world->addRigidBody(body);

    return body;
}

void renderBox(Model& cube, btRigidBody* box, Shader& shader)
{
    if(box->getCollisionShape()->getShapeType() != BOX_SHAPE_PROXYTYPE)
    {
        return;
    }  
    btTransform t;
    box->getMotionState()->getWorldTransform(t);
    float mat[16];
    t.getOpenGLMatrix(mat);
    shader.setMat4("model", glm::make_mat4(mat));
    cube.Draw(shader);
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
	std::cout << "Initializing Window..." << std::endl;
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "MyWindow", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
        std::exit(1);
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		std::exit(1);
	}   

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); 

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);


	int numAttr;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &numAttr);
	std::cout << "Max Vertex Attributes supported: " << numAttr << std::endl;
    return window;
}

int main()
{
    //GLFW    
    GLFWwindow* window = initWindow();
    
    //load models
//    manager = new AssetManager{};
//    manager->load("assets/Models/Island/object.dae");
//    manager->load("assets/Models/cube/cube.obj");
    

    //Initialize Bullet
    //-----------------
    world = new Dynamics(btVector3(0,-10,0));
    
    btTransform trans;
    trans.setIdentity();
    trans.setOrigin(btVector3(0,0,0));
    btStaticPlaneShape* plane = new btStaticPlaneShape(btVector3(0,1,0), 0);
    btMotionState* motion = new btDefaultMotionState(trans);
    btRigidBody::btRigidBodyConstructionInfo info{0.0, motion, plane};
    btRigidBody* body = new btRigidBody(info);
    world->addRigidBody(body);

    addBox(btVector3(1,1,1), 0, 20, 0, 1.0);

    // Load Textures
    //--------------------------------
    
//    unsigned int diffuseTexture;
//    loadTexture(diffuseTexture, "assets/Textures/container2.png", true);

//    unsigned int specularTexture;
//    loadTexture(specularTexture, "assets/Textures/container2_specular.png", true);


	//Generation of the Shader Program
	//--------------------------------
	

    // positions of the point lights
    glm::vec3 pointLightPositions[] = {
        glm::vec3( 0.7f,  0.2f,  2.0f),
        glm::vec3( 2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3( 0.0f,  0.0f, -3.0f)
    };
	
	Shader shader("assets/gl/object.vs", "assets/gl/object.fs", "assets/gl/object.gs");
    shader.use();
    // directional light
    shader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
    shader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
    shader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
    shader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
    // point light 1
    shader.setVec3("pointLights[0].position", pointLightPositions[0]);
    shader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
    shader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
    shader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
    shader.setFloat("pointLights[0].constant", 1.0f);
    shader.setFloat("pointLights[0].linear", 0.09);
    shader.setFloat("pointLights[0].quadratic", 0.032);
    // point light 2
    shader.setVec3("pointLights[1].position", pointLightPositions[1]);
    shader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
    shader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
    shader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
    shader.setFloat("pointLights[1].constant", 1.0f);
    shader.setFloat("pointLights[1].linear", 0.09);
    shader.setFloat("pointLights[1].quadratic", 0.032);
    // point light 3
    shader.setVec3("pointLights[2].position", pointLightPositions[2]);
    shader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
    shader.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
    shader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
    shader.setFloat("pointLights[2].constant", 1.0f);
    shader.setFloat("pointLights[2].linear", 0.09);
    shader.setFloat("pointLights[2].quadratic", 0.032);
    // point light 4
    shader.setVec3("pointLights[3].position", pointLightPositions[3]);
    shader.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
    shader.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
    shader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
    shader.setFloat("pointLights[3].constant", 1.0f);
    shader.setFloat("pointLights[3].linear", 0.09);
    shader.setFloat("pointLights[3].quadratic", 0.032);

    shader.setFloat("shininess1", 32.0f);

    shader.setInt("texture_diffuse1", 0);
    shader.setInt("texture_specular1", 1);

    Shader lampShader("assets/gl/lamp.vs", "assets/gl/lamp.fs");

    glEnable(GL_DEPTH_TEST);

	//Generate a camera
	cam = new Camera{glm::vec3{0.0f, 0.0f, -10.0f}, glm::vec3{0.0f, 0.0f, 1.0f}, 
		glm::vec3{0.0f, 1.0f, 0.0f}};


    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };    


//    Model& myModel = *manager->get("assets/Models/Island/object.dae");

    Model myModel{"assets/Models/Island/object.dae"};
    myModel.initializeGL();

//    std::cout << myModel.texturesLoaded_.size() << std::endl;

//    Model& cube = *manager->get("assets/Models/cube/cube.obj");
    Model cube{"assets/Models/cube/cube.obj"};
    cube.initializeGL();

    float dt = 0.0f;
    float lastFrame = 0.0f;

//    std::cout << "Snake Thing: " << std::endl << myModel;
//    std::cout << "Cube: " << std::endl << cube;

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
            
        
		//poll inputs
		processInput(window, *cam, dt);


		//Clear
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Calculate Transformations
		//model transformation (to the center of the model)


		//Draw
        shader.use();
		
		//projection: generates frustum	
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), 
			((float)SCR_WIDTH)/((float)SCR_HEIGHT), 0.1f, 1000.0f); 
		//view: the inverse transform of the camera's local transformation
		glm::mat4 view = cam->view();
		shader.setMat4("projection", projection);
		shader.setMat4("view", view);
        shader.setVec3("viewPos", cam->position_);

//        world->debugDraw(projection, view);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0,-50,0));
        model = glm::rotate(model, -glm::pi<float>()/2, glm::vec3(1,0,0));
        shader.setMat4("model", model);
        myModel.Draw(shader);

        for (auto i = world->bodies_.begin(); i != world->bodies_.end(); ++i)
        {
            renderBox(cube, *i, shader);
        }
		
//        for (unsigned int i = 0; i < 10; i++)
//        {
//            glm::mat4 model{1.0f};
//            model = glm::translate(model, cubePositions[i]);
//            float angle = 20.0f * i;
//            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f,0.3f, 0.5f));
//            shader.setMat4("model", model);
//            cube.Draw(shader);
//        }

        // lamp
        lampShader.use();
        lampShader.setMat4("projection", projection);
        lampShader.setMat4("view", view);
    
        for (unsigned int i = 0; i < 4; i++) 
        {
            glm::mat4 model{1.0f};
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3{0.2f});
            lampShader.setMat4("model", model);
            cube.Draw(lampShader);
        }


		//Handle events
		glfwSwapBuffers(window);
		glfwPollEvents();    
        
	}
	
	std::cout << "Closing Window" << std::endl;

    delete cam;
    delete world;
    delete manager;

	glfwTerminate();
	return 0;
}

std::ostream& operator<<(std::ostream& os, const glm::vec3& vec)
{
    os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
    return os;
} 

void processInput(GLFWwindow *window, Camera& cam, float dt)
{
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
        cam.position_ += glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), cam.getRight()) * dt * 10.0f;
	}
	if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
        cam.position_ -= glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), cam.getRight()) * dt * 5.0f;
	}
	if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
        cam.position_ -= cam.getRight() * dt * 7.0f;
	}
	if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
        cam.position_ += cam.getRight() * dt * 7.0f;
	}
    if(glfwGetKey(window,GLFW_KEY_SPACE) == GLFW_PRESS) 
    {
        cam.position_ += glm::vec3(0.0f,1.0f,0.0f) * dt * 7.0f;
    }
    if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) 
    {
        cam.position_ -= glm::vec3(0.0f,1.0f,0.0f) * dt * 7.0f;
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

double lastX = 400, lastY = 300;
bool firstMouse = true;

const glm::vec3 UP{0.0f, 1.0f, 0.0f};

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
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

    double sensitivity = 0.005;
    dx *= sensitivity;
    dy *= sensitivity;

    cam->rotate(dy, cam->getRight());
    cam->rotate(dx, UP);
    
}

void loadTexture(unsigned int& texture, const char* file, bool hasAlpha, std::mutex& glmutex)
{
	int width, height, nrChannels;
	unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);

    glmutex.lock();
	glGenTextures(1, &texture);

	glBindTexture(GL_TEXTURE_2D, texture);

	// set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	if(data)
	{
        if(hasAlpha)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, 
                GL_UNSIGNED_BYTE, data);
        }
        else 
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, 
                GL_UNSIGNED_BYTE, data);
        }
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
        std::exit(1);
	}
    glmutex.unlock();

	stbi_image_free(data);
}
