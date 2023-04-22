#pragma one
#include <vendor/glad/glad.h>
#include <GLFW/glfw3.h>
#include "engine/logger.h"
#include "engine/window.h"

class Game {
private:
	int m_targ_fps = 60, m_targ_ups = 30;
	Logger& logger = Logger::getInstance();
	Window m_window;

public:
	Game(int loglevel, bool fullscreen);
	void loop();
	void render(float dt);
	void tick(float dt);
	void exit();
};
