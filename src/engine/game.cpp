#include "engine/game.h"


//// Stuff to make Game singleton, so all classes can call stuff
//// like issuing exit
Game& Game::instance = Game::getInstance(); //Singleton shit
Game::Game() {} //Empty constructor (required for some reason)
Game& Game::getInstance() { //Singleton getter
	static Game instance;
	return instance;
}

//// Setup static exit flag
int Game::exitFlag;
void Game::flagExit(int sig) {
	exitFlag = sig;
}

//// Setup static game directory (used for file IO)
std::string Game::m_gameDir;
const std::string Game::getGameDir() {
	return m_gameDir;
}

//// Init shall be called once by the main function,
//// initializing the window and all other resources
//// todo: parameters of this should be launch arguments parsed by 'main.cpp'
void Game::init(int loglevel, bool fullscreen) {
	// Log level gets set from main function
	logger.setLogLevel(loglevel);

	// Create Window -- todo: remove baked in arguments
	m_window.init(1920,1080,"Gyurchine");
	m_window.setFullscreen(fullscreen);
	m_window.setClearColor(0.f,0.f,0.f);

	// Get working directory (for file IO)
	char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            m_gameDir = std::string(cwd);
        } else {
			// In case it fails, exit
			logger.err("GAME","Cannot get working directory! ?? WTF? ??");
			Game::flagExit();	
		}
	Game::m_gameDir = std::string(cwd)+"/";

//// TEMPORARY ////
// Basic OpenGL code for triangle
// To be removed once gl aspects get abstracted
	float t[] =
	{
		-.5f, -.5f, 0.f,
		0.f, .5f, 0.f,
		.5f, -.5f, 0.f
	};
	uint32_t v;
	glGenBuffers(1, &v);
	glBindBuffer(GL_ARRAY_BUFFER, v);
	glBufferData(GL_ARRAY_BUFFER, sizeof(t)*sizeof(t)/sizeof(t[0]), &t[0], GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*3, (void*)0);
	glEnableVertexAttribArray(0);
//// TEMPORARY ////

	//Enter game loop
	loop();
}

//// Runs until the window is closed or the Game's exit flag is set
//// FPS and UPS updates occur independently and are currently defined in 'game.h'
void Game::loop() {
    float timePerFrame = 1.0 / m_targ_fps;
    float timePerUpdate = 1.0 / m_targ_ups;

    float lastFrameTime = glfwGetTime();
    float lastUpdateTime = lastFrameTime;
	float deltaTime = 0.0;

	logger.detail("GAME","Game loop started");
	while(!glfwWindowShouldClose(m_window.getPtr())) {
		if (Game::exitFlag) exitGame();
		
		// Not polling events would make the window unresponsive,
		// Sot it'd called even when no updates are needed
		glfwPollEvents();
        
		float currentTime = glfwGetTime();
        float deltaFrameTime = currentTime - lastFrameTime;
        float deltaUpdateTime = currentTime - lastUpdateTime;

		if (deltaUpdateTime >= timePerUpdate) {
            lastUpdateTime = currentTime;
			tick(deltaUpdateTime);
        }

        if (deltaFrameTime >= timePerFrame) {
            lastFrameTime = currentTime;
			render(deltaUpdateTime);
        }
	}
	exitGame();
}


//// Start of render pipeline
// NOTE: Currently it renders a primitive triangle whilst wiggling randomly its verices 
void Game::render(float dt) {
	float t[] ={
		-.5f, -.5f, 0.f,
		0.f, .5f, 0.f,
		.5f, -.5f, 0.f};
	t[rand()%8]+=((rand()%4)-2)*0.001f; //Wiggling vertices

	glBufferData(GL_ARRAY_BUFFER, sizeof(t)*sizeof(t)/sizeof(t[0]), &t[0], GL_DYNAMIC_DRAW);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	//Once there will be a background/everything will be re-rendered,
	//can be removed for performance
	glClear(GL_COLOR_BUFFER_BIT);
	
	//V-sync
	glfwSwapBuffers(m_window.getPtr());
}

//// Update game world
// NOTE: this time, the UI will be updated independently from this
void Game::tick(float dt) {
}

//// Cleans up resources and exits with given exit code
//// Private function for thread safety
//// Gets called by the Game::loop() when exit flag is set or window gets closed
void Game::exitGame() {
	m_window.close();
	exit(Game::exitFlag);
}
