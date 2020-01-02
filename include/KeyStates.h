#ifndef KEYSTATES_H
#define KEYSTATES_H

#include <map>
#include <GLFW/glfw3.h>

const int KEY_JUST_PRESSED = 1 << 0;
const int KEY_PRESSED = 1 << 1;
const int KEY_JUST_RELEASED = 1 << 2;

class KeyStates
{
private:
	std::map<int, int>* keyStates;

public:
	KeyStates()
	{
		keyStates = new std::map<int, int>();
	}
	~KeyStates()
	{
		delete keyStates;
	}
	
	void update(GLFWwindow* window)
	{
		std::map<int,int>::iterator it = keyStates->begin();
		for(; it != keyStates->end(); ++it)
		{
			it->second &= ~(KEY_JUST_PRESSED | KEY_JUST_RELEASED);
			if(glfwGetKey(window, it->first) == GLFW_PRESS)
			{
				if((it->second | ~KEY_PRESSED) != 0)
				{
					it->second |= KEY_JUST_PRESSED;
				}
				it->second |= KEY_PRESSED;
			}
			else
			{
				if((it->second & KEY_PRESSED) == 0)
				{
					
				}
				it->second &= ~KEY_PRESSED;
			}
		}
	}

	void add(int key)
	{
		keyStates->insert(std::pair<int, int>(key, 0));
	}

	bool isKeyDown(int key)
	{
		return (keyStates->at(key) & KEY_PRESSED) != 0;
	}

	bool isKeyReleased(int key)
	{
		return (keyStates->at(key) & KEY_PRESSED) == 0;
	}

	bool isKeyJustDown(int key)
	{
		return (keyStates->at(key) & KEY_JUST_PRESSED) != 0;
	}

	bool isKeyJustReleased(int key)
	{
		return (keyStates->at(key) & KEY_JUST_RELEASED) != 0;
	}


	
};

#endif
