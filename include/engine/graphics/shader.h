#pragma once
#include "engine/utils.h"

#include <vendor/glad/glad.h>
#include <GLFW/glfw3.h>


namespace graphics {
	
	class Shader {
	private:
		Logger& logger = Logger::getInstance(); // Singleton logger instance
		
		// OpenGL ID of the program
		// 0 indicates that it's invalid
		unsigned int m_program = 0;

		// Used for safety guards 
#define SHADER_NOT_INITIALIZED 0
#define SHADER_COMPILED 1
#define SHADER_BOUND 2
		int m_state = SHADER_NOT_INITIALIZED;
		std::string m_debugName;

#define SHADER_SPLIT_TOKEN "#fragment"
		

	public:
		//// Reads source codes from given path, splits vertex and fragment codes, then compiles them
		// NOTE: Shader objects get deleted after this function, only the shader program survives
		int createShader(const std::string path);
		
		//// Used to "select" shader
		void use();

		//// Mark shader program deletable
		void destroy();

		~Shader();
	};
}
