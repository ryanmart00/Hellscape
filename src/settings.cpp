#include "settings.hpp"

void Settings::mouseCallback(GLFWwindow*, double xpos, double ypos)
{
    
}
void Settings::pollInput(GLFWwindow* window, float dt)
{
    std::cout << "In Settings" << std::endl;
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
        state_ = GameState::PLAYING;
	}
    
}
