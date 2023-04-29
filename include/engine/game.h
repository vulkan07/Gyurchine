#pragma one

#include "engine/logger.h"
#include "engine/window.h"
#include "engine/utils.h"
#include "engine/graphics/shader.h"

#include <chrono>
#include <thread>

#ifdef __linux__
	#include <unistd.h>
#endif

class Game {
private:
	// Game's objects
	Logger& logger = Logger::getInstance(); // Singleton logger instance
	Window m_window;

	// Game's variables
	int m_targ_fps = 30;
	int m_targ_ups = 30;
	
	// Specials variables
	static std::string m_gameDir; // Fulls string path to the executable
	static int exitFlag; // Used for other threads to signal exit
	static Game& instance; // Singleton instance
	
	// Functions
	void exitGame(); // Private for thread safety, use flagExit() instead
	Game();	// Private, cause it's singleton
	

public:
	static void flagExit(int sig); // Called to signal an exit (e.g. by sigterm)
	static Game& getInstance(); // Singleton
	static const std::string getGameDir();
	void init(int loglevel, bool fullscreen);
	void loop();
	void render(float dt);
	void tick(float dt);
};
