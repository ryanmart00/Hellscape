#include <glad/gl.h>
#ifndef GLFW_GUARD
#define GLFW_GUARD
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#endif



#include <cstdlib>
#include <iostream>

#include "asset_manager.hpp"
#include "gameplay.hpp"

#include <iostream>
#include <tgmath.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <math.h>

#include "constants.hpp"
#include "inputs.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "gameplay.hpp"
#include "text.hpp"

static void error_callback(int error, const char* description)
{
    std::cout << description << std::endl;
}



//Initializes the GLFW Window and starts up GLAD
static GLFWwindow* initWindow()
{
    glfwSetErrorCallback(error_callback);
	if(!glfwInit())
    {
        std::cerr << "GLFW init failed" << std::endl;
        std::exit(1);
    }
    std::cout << "GLFW init succeeded" << std::endl;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); 


	//Initialization of Window
	//------------------------
	#ifdef DEBUG
        std::cout << "Initializing Window..." << std::endl;
    #endif
	GLFWwindow* window = glfwCreateWindow(800, 800, "Hellscape", NULL, NULL);
	if (window == NULL)
	{
        std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
        std::exit(1);
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGL(glfwGetProcAddress))
	{
        std::cerr << "Failed to initialize GLAD" << std::endl;
		std::exit(1);
	}   


	#ifdef DEBUG 
	int numAttr;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &numAttr);
        std::cout << "Max Vertex Attributes supported: " << numAttr << std::endl;
    #endif
    return window;
}


/*
int main()
{
    GLFWwindow* window = initWindow();
    while(!glfwWindowShouldClose(window))
	{
		//Handle events
		glfwSwapBuffers(window);
		glfwPollEvents();    
	}
	

}
*/

Settings* settings;

Matchmaking* match;

Gameplay* gameplay;

GameState state;
Input* input_handlers[NUM_STATES];    

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    input_handlers[state]->framebuffer_size_callback(window,width,height);
}

void mouseCallback(GLFWwindow* window, double X, double Y)
{
    input_handlers[state]->mouseCallback(window, X, Y);
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods )
{
    input_handlers[state]->mouseButtonCallback(window, button, action, mods);
}

void pollInput(GLFWwindow* window, float dt)
{
    input_handlers[state]->pollInput(window,dt);
}

void characterCallback(GLFWwindow* window, unsigned int point)
{
    input_handlers[state]->characterCallback(window,point);
}

unsigned int quadVAO = 0;
unsigned int quadVBO;
static void renderQuad()
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


/*
// commented out when pushing to main until it is properly debugged
// horrible practice, apologies =P 
GLuint chairVAO = 0;
GLuint chairVBO = 0;
std::vector<glm::vec3> generateCrosshair(glm::vec3 cameraPos)
{
    // i imagine we'll want to move to a texture eventually
    const int NUM_VERTS = 100;
    const float RADIUS = 0.30f;
    const float ANGLE_INCREMENT = 2*M_PI / NUM_VERTS;
    int triangleCount = NUM_VERTS - 2;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> temp;

    // positions
    for (int i = 0; i < NUM_VERTS; i++)
    {
        float currentAngle = ANGLE_INCREMENT * i;
        float x = cameraPos[0] + RADIUS * cos(glm::radians(currentAngle));
        float y = cameraPos[1] + RADIUS * sin(glm::radians(currentAngle));
        float z = cameraPos[2] + 0.0f;
        temp.push_back(glm::vec3(x, y, z));
    }

    for (int i = 0; i < triangleCount; i++)
    {
        vertices.push_back(temp[0]);
        vertices.push_back(temp[i + 1]);
        vertices.push_back(temp[i + 2]);
    }

    // building this vector every main loop iteration doesn't seem great
    return vertices;
}

void renderCrosshair(Shader& shader, glm::vec3 cameraPos)
{
    std::vector<glm::vec3> crosshairData = generateCrosshair(cameraPos);

    if (chairVAO == 0)
    {
        glClearColor(0.0f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        shader.use();

        glGenVertexArrays(1, &chairVAO);
        glGenBuffers(1, &chairVBO);
        glBindBuffer(GL_ARRAY_BUFFER, chairVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * crosshairData.size(), &crosshairData[0], GL_DYNAMIC_DRAW);
        //glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat), 0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
        glEnableVertexAttribArray(0);
    }

    glBindVertexArray(chairVAO);
    glBindBuffer(GL_ARRAY_BUFFER, chairVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * crosshairData.size(), &crosshairData[0], GL_DYNAMIC_DRAW);
    glDrawArrays(GL_TRIANGLES, 0, crosshairData.size());
    glBindVertexArray(0);
    glActiveTexture(0);
}
*/

int main()
{
    //model loader
    AssetManager manager{};

    state = GameState::SETTINGS;
    //GLFW    
    GLFWwindow* window = initWindow();
    
    // Settings input manager
    {
        settings = new Settings(state, input_handlers);
        input_handlers[GameState::SETTINGS] = settings;
    }

    // Matchmaking input manager
    {
        match = new Matchmaking(state, input_handlers);
        input_handlers[GameState::MATCH] = match;
    }

    // Gameplay input manager
    {
        gameplay = new Gameplay(state, input_handlers, manager,800,800);
        input_handlers[GameState::PLAYING] = gameplay;
        gameplay->softInit();
    }
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetCharCallback(window, characterCallback);
    glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);
    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    float dt = 0.0f;
    float lastFrame = 0.0f;
    Text* text = new Text("assets/gl/text.vs", "assets/gl/text.fs");


	//Main Loop
	//---------
	while(!glfwWindowShouldClose(window))
	{
        // calculate dt
        float currentFrame = glfwGetTime();
        dt = currentFrame - lastFrame;
        lastFrame = currentFrame;

        switch(state)
        {
        case GameState::PLAYING:
            gameplay->update(dt);
        break;
        case GameState::SETTINGS:    
            settings->update(dt);
        break;
        case GameState::MATCH:
            std::cout << "Matchmaking" << std::endl;
        break;
        }

   
		//Handle events
		glfwSwapBuffers(window);
		glfwPollEvents();    
        
		//poll inputs
		pollInput(window, dt);
	}
	
	#ifdef DEBUG 
        std::cerr << "Closing Window" << std::endl;
    #endif
    glfwDestroyWindow(window);
	glfwTerminate();
    delete settings;
    delete match;
    #ifdef DEBUG
        std::cerr << "Successfully destroyed window" << std::endl;
    #endif
}

std::ostream& operator<<(std::ostream& os, const glm::vec3& vec)
{
    os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
    return os;
} 
