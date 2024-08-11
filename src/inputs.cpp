#include "inputs.hpp"


Settings::Settings(GameState& state, Input** inputs)
    : Input{state, inputs}
{
    text_ = new Text("assets/gl/text.vs", "assets/gl/text.fs");
}
void Settings::mouseCallback(GLFWwindow*, double xpos, double ypos)
{
    
}
void Settings::mouseButtonCallback(GLFWwindow*, int, int, int)
{
    
}
void Settings::pollInput(GLFWwindow* window, float dt)
{
	if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
        contextSwitch(GameState::PLAYING);
	}
}

void Settings::characterCallback(GLFWwindow*, unsigned int point)
{
    std::cout << point << std::endl;
}

void Settings::hardInit()
{   
}   

void Settings::stop()
{
}

void Settings::update(float dt)
{
    //Clear
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //Reset Viewport Size and clear again
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glViewport(0,0, SCR_WIDTH, SCR_HEIGHT);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //Show FPS
    text_->use();
    text_->setVec4("color", glm::vec4{0,0,0,1});
    float sx = 2.0 / SCR_WIDTH;
    float sy = 2.0 / SCR_HEIGHT;

    text_->renderText((std::string("FPS: ")
                + std::to_string((int)(1/dt))).c_str(),
                -1 + 8 * sx, 1 - 50 * sy, sx, sy);

}

Matchmaking::Matchmaking(GameState& state, Input** inputs) : Input{state, inputs}
{
    ip = "";
}

void Matchmaking::mouseCallback(GLFWwindow*, double xpos, double ypos)
{
    
}
void Matchmaking::mouseButtonCallback(GLFWwindow*, int, int, int)
{
    
}
void Matchmaking::pollInput(GLFWwindow* window, float dt)
{
	if(glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
	{
        //Try to connect to the IP inputted 
        ip = "";
	}
}

void Matchmaking::characterCallback(GLFWwindow*, unsigned int point)
{
    /*
    icu::UnicodeString uni_str((UChar32)point);
    std::string str;
    uni_str.toUTF8String(str);
    ip.append(str);
    std::cout << ip << std::endl;
    */
}

void Matchmaking::hardInit()
{
}

void Matchmaking::stop()
{
}

void Matchmaking::update(float)
{
}

