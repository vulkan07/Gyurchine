#pragma once
#include <vendor/glad/glad.h>
#include <GLFW/glfw3.h>

#include "engine/logger.h"


class Window {
private:
	int m_width, m_height;	//Window size
	int m_state = 0;		//State [0 Not initialized, 1 Opened, 2 Closed]	
	bool m_fullscreen = false;
	GLFWwindow* m_window;	//Window pointer
	Logger& logger = Logger::getInstance();
public:
	void resize(int w, int h);
	void close();

	void setResizeable(bool b);
	void setDecorated(bool b);
	void setFullscreen(bool b);
	
	bool isFullscreen() const;
	int getWidth() const;
	int getHeight() const;

	GLFWwindow* getPtr() const;
	Window(int w, int h, const char* title); //Initialize GLFW, and make context current
	~Window(); 				//Calls close()	
};
