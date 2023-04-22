#include <iostream>
#include <vendor/glad/glad.h>
#include <GLFW/glfw3.h>
#include "engine/logger.h"

int main() {
	std::cout << "\n+++[ Gyurchine started ]+++\n\n";
	
	Logger& logger = Logger::getInstance();
	logger.setLogLevel(Logger::DETAIL);

	glfwInit();
	GLFWwindow* w = glfwCreateWindow(1920,1080, "Gyurchine", NULL, NULL);
	glfwMakeContextCurrent(w);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	float t[] =
	{
		-.5f, -.5f, 0.f,
		0.f, .5f, 0.f,
		.5f, -.5f, 0.f
	};
	uint32_t v;
	glGenBuffers(1, &v);
	glBindBuffer(GL_ARRAY_BUFFER, v);
	glBufferData(GL_ARRAY_BUFFER, sizeof(t)*sizeof(t)/sizeof(t[0]),
		&t[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*3,
		 (void*)0);
	glEnableVertexAttribArray(0);
	glViewport(0, 0, 600, 600);
	glClearColor(1.0f, 0.5f, 0.0f, 0.0f);
	glfwWindowHint(GLFW_SAMPLES, 2);
	glEnable(GL_MULTISAMPLE);  

	while(!glfwWindowShouldClose(w))
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glfwSwapBuffers(w);
		glfwPollEvents();
	}
	glfwTerminate();
	logger.info("GAME","Window closed");
}

