#include "mesh.hpp"
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

using namespace std;

void processInput(GLFWwindow *window, Camera& cam, float dt);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void loadTexture(unsigned int& texture, const char* file, bool hasAlpha);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);


const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

Camera* cam;

int main()
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
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}   

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); 

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);


	int numAttr;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &numAttr);
	std::cout << "Max Vertex Attributes supported: " << numAttr << std::endl;


    // Load Textures
    //--------------------------------
    
    unsigned int diffuseTexture;
    loadTexture(diffuseTexture, "Textures/container2.png", true);

    unsigned int specularTexture;
    loadTexture(specularTexture, "Textures/container2_specular.png", true);


	//Generation of the Shader Program
	//--------------------------------
	

    // positions of the point lights
    glm::vec3 pointLightPositions[] = {
        glm::vec3( 0.7f,  0.2f,  2.0f),
        glm::vec3( 2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3( 0.0f,  0.0f, -3.0f)
    };
	
	Shader shader("src/gl/object.vs", "src/gl/object.fs");

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

    shader.setFloat("material.shininess", 32.0f);

    shader.setInt("material.diffuse", 0);
    shader.setInt("material.specular", 1);

    Shader lampShader("src/gl/lamp.vs", "src/gl/lamp.fs");

    glEnable(GL_DEPTH_TEST);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f,  1.0f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };
	//bind Vertex Array Object
	unsigned int cubeVAO;
	glGenVertexArrays(1, &cubeVAO);

	//bind Vertex Buffer Object
	unsigned int VBO;
	glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(cubeVAO);


	//Set the vertex attribute pointers
	//(index of attribute, size of attribute, type of attribute,
	//		normalized, size of one vertex, pointer to attribute start)
	//position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 
        (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
        (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);

    // bind light VAO
    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);

    // we already have the VBO filled
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
	
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

    Model myModel{"Models/HELLSCAPEXPORTTESTSNAKEY.FBX"};

    float dt = 0.0f;
    float lastFrame = 0.0f;

	//Main Loop
	//---------
	while(!glfwWindowShouldClose(window))
	{
        // calculate dt
        float currentFrame = glfwGetTime();
        dt = currentFrame - lastFrame;
        lastFrame = currentFrame;
            
        
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
			((float)SCR_WIDTH)/((float)SCR_HEIGHT), 0.1f, 100.0f); 
		//view: the inverse transform of the camera's local transformation
		glm::mat4 view = cam->view();
		shader.setMat4("projection", projection);
		shader.setMat4("view", view);
        shader.setVec3("viewPos", cam->position_);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
        shader.setMat4("model", model);
        myModel.Draw(shader);

		
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseTexture);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularTexture);
        // cube
		glBindVertexArray(cubeVAO);

        for (unsigned int i = 0; i < 10; i++)
        {
            glm::mat4 model{1.0f};
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f,0.3f, 0.5f));
            shader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // lamp
        lampShader.use();
        lampShader.setMat4("projection", projection);
        lampShader.setMat4("view", view);
    
        glBindVertexArray(lightVAO);

        for (unsigned int i = 0; i < 4; i++) 
        {
            glm::mat4 model{1.0f};
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3{0.2f});
            lampShader.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }


		//Handle events
		glfwSwapBuffers(window);
		glfwPollEvents();    
	}
	
	std::cout << "Closing Window" << std::endl;

	glDeleteVertexArrays(1, &cubeVAO);
    glDeleteBuffers(1, &lightVAO);
	glDeleteBuffers(1, &VBO);

    delete cam;

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
        cam.position_ += cam.getForward() * dt * 2.0f;
	}
	if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
        cam.position_ -= cam.getForward() * dt * 2.0f;
	}
	if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
        cam.position_ -= cam.getRight() * dt * 2.0f;
	}
	if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
        cam.position_ += cam.getRight() * dt * 2.0f;
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

void loadTexture(unsigned int& texture, const char* file, bool hasAlpha)
{
	int width, height, nrChannels;
	unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);

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

	stbi_image_free(data);
}
