#include "engine/graphics/shader.h"

namespace graphics {

	int Shader::createShader(const std::string path) {
		
		// Used for debug printings, to know which shader it is
		m_debugName = path;

		// Read shader file
		std::string source = utils::readFile(path.c_str(),true);

		// Try to split file content into fragment and vertex shader by token "#fragment"
		auto pos = source.find(SHADER_SPLIT_TOKEN);
    	if (pos == std::string::npos) {
			std::string msg = "(" + m_debugName + ") cannot split shader source codes";
			logger.err("SHADER",msg.c_str());
			return 1;
		}
		
		// Get strings of vertex, fragment source codes
        std::string vertexSTR = source.substr(0, pos);
        std::string fragmentSTR = source.substr(pos + std::string(SHADER_SPLIT_TOKEN).length());

		// Get the char*, because gl only accepts that
		const char* vertexSRC = vertexSTR.c_str();
        const char* fragmentSRC = fragmentSTR.c_str(); 

		// ID's of the shaders
		// Since they're deleted after linking the program, they're only defined in this function
		unsigned int m_vertex = 0;
		unsigned int m_fragment = 0;

		// -------------------- //
		// Create VERTEX shader //

		int success; // compile exit code
		char log[512]; // compile message

		m_vertex = glCreateShader(GL_VERTEX_SHADER); // gl ID
		glShaderSource(m_vertex, 1, &vertexSRC, NULL);
		glCompileShader(m_vertex);
		glGetShaderiv(m_vertex, GL_COMPILE_STATUS, &success);
		if(!success) {
			glGetShaderInfoLog(m_vertex, 512, NULL, log);
			logger.err("SHADER", "Failed to compile vertex shader!");
			logger.err("SHADER", m_debugName.c_str());
			logger.err("SHADER", (const char*)log);
			return 2;
		}
		
		// ---------------------- //
		// Create FRAGMENT shader //

		// success and log will be reused;
		m_fragment = glCreateShader(GL_FRAGMENT_SHADER); // gl ID
		glShaderSource(m_fragment, 1, &fragmentSRC, NULL);
		glCompileShader(m_fragment);
		glGetShaderiv(m_fragment, GL_COMPILE_STATUS, &success);
		if(!success) {
			glGetShaderInfoLog(m_fragment, 512, NULL, log);
			logger.err("SHADER", "Failed to compile fragment shader!");
			logger.err("SHADER", m_debugName.c_str());
			logger.err("SHADER", (const char*)log);
			return 3;
		}


		// --------------------- //
		// Create shader PROGRAM //

		m_program = glCreateProgram();
		glAttachShader(m_program, m_vertex);
		glAttachShader(m_program, m_fragment);
		glLinkProgram(m_program);
		// print linking errors if any
		glGetProgramiv(m_program, GL_LINK_STATUS, &success);
		if(!success) {
			glGetProgramInfoLog(m_program, 512, NULL, log);
			logger.err("SHADER", "Failed to link shader program!");
			logger.err("SHADER", m_debugName.c_str());
			logger.err("SHADER", (const char*)log);
			return 4;
		}
		  
		// delete the shaders as they're linked into our program now and no longer necessary
		glDeleteShader(m_vertex);
		glDeleteShader(m_fragment);


		// Print creation of shader	
		logger.info("SHADER", m_debugName.c_str());
		return 0;
	}
	

	void Shader::use() {
		glUseProgram(m_program);
	}

	
	void Shader::destroy() {
		logger.detail("SHADER", "Deleted program");
		glUseProgram(0);
		glDeleteProgram(m_program);	
	}

	Shader::~Shader() {
		logger.warn("SHADER", "Deleted program from destructor. Forgot to call .destroy()?");
		this->destroy();
	}

}
