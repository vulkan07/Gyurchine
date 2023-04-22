#include "engine/window.h"

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

void Window::setResizeable(bool b) {glfwWindowHint(GLFW_RESIZABLE, b?GLFW_TRUE:GLFW_FALSE);}

void Window::setDecorated(bool b) {glfwWindowHint(GLFW_DECORATED, b?GLFW_TRUE:GLFW_FALSE);}

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

bool Window::isFullscreen() const {
	return m_fullscreen;
}

int Window::getWidth() const {return m_width;}
int Window::getHeight() const {return m_height;}
GLFWwindow* Window::getPtr() const {return m_window;}

Window::Window(int w, int h, const char* title): m_width(w), m_height(h) {
	glfwInit();
	m_window = glfwCreateWindow(w, h, title, NULL, NULL);
	glfwMakeContextCurrent(m_window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glViewport(0,0,w,h);
	std::string msg = "Window created - ";
	msg += std::to_string(w);
	msg += "x";
	msg += std::to_string(h);
	logger.info("WINDOW", msg.c_str());
	m_state = 1;
}

Window::~Window() {
	close();
}

void Window::close() {
	if (m_state == 2) return;
	m_state = 2;
	glfwTerminate();
	logger.info("WINDOW","Window closed.");	
}
