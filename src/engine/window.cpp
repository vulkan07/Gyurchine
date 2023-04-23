#include "engine/window.h"

//// Resizes window and updates OpenGL viewport
//// Safety-checks whether window is open
void Window::resize(int w, int h) {
	if (m_state != 1) {
		logger.warn("WINDOW", "Attempted to resize window while it's not initialized!");
		return;
	}
	m_width = w;
	m_height= h;
	glfwSetWindowSize(m_window, w,h);
	glViewport(0,0,w,h);
	logger.detail("WINDOW", "resized"); 
}


//// Sets window's background color
//// Only works, if the render pipeline issues a clear
//// Safety-checks whether window is open
void Window::setClearColor(float r, float g, float b) {
	if (m_state != 1) {
		logger.warn("WINDOW", "Attempted to set window clear color while it's not initialized!");
		return;
	}
	glClearColor(r, g, b, 0.0f);
}

void Window::setResizeable(bool b) {glfwWindowHint(GLFW_RESIZABLE, b?GLFW_TRUE:GLFW_FALSE);}
void Window::setDecorated(bool b) {glfwWindowHint(GLFW_DECORATED, b?GLFW_TRUE:GLFW_FALSE);}

//// Fullscreens window to THE DEFAULT DISPLAY
//// Also removing it's borders
void Window::setFullscreen(bool b) {
	if (b == m_fullscreen) return;
	m_fullscreen = b;

	std::string msg = "Fullscreen ";
	msg += b?"on":"off";
	logger.detail("WINDOW", msg.c_str());

	if (b) {
		setDecorated(false);
		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);
		glfwSetWindowMonitor(m_window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
	} else {
		setDecorated(true);
		glfwSetWindowMonitor(m_window, nullptr, 100, 100, m_width, m_height, 0);
	}
}

// Getters
bool Window::isFullscreen() const {return m_fullscreen;}
int Window::getWidth() const {return m_width;}
int Window::getHeight() const {return m_height;}
//// Return actual GLFW window pointer
GLFWwindow* Window::getPtr() const {return m_window;}


//// Must be called once from the windows' owner
//// Creates GLFW window, sets params like multisampling, alpha blending,
//// And creates OpenGL context
//// In the end, sets m_state to 1 (open)
void Window::init(int w, int h, const char* title) {
	m_width  = w;
	m_height = h;

	//Create window
	glfwInit();
	m_window = glfwCreateWindow(w, h, title, NULL, NULL);
	glfwMakeContextCurrent(m_window);

	//Load OpenGL
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress); //Must be here, otherwise segfault
	glViewport(0,0,w,h);

	//Set multisampling
	glfwWindowHint(GLFW_SAMPLES, 16);
	glEnable(GL_MULTISAMPLE);  
	
	//Set alpha blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Logger message
	std::string msg = "Window created - ";
	msg += std::to_string(w);
	msg += "x";
	msg += std::to_string(h);
	logger.info("WINDOW", msg.c_str());

	//Set state to 'opened'
	m_state = 1;
}

Window::Window() {}

Window::~Window() {close();}

void Window::close() {
	if (m_state != 1) return;
	m_state = 2;
	glfwTerminate();
	logger.info("WINDOW","Window closed.");	

}
