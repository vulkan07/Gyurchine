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
int Game::exitFlag = 0;
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
	m_window.setClearColor(1.f,.5f,0.f);

	// Get working directory (for file IO)
	char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
			Game::m_gameDir = std::string(cwd)+"/assets/";
        } else {
			// In case it fails, exit
			logger.err("GAME","Cannot get working directory! ?? WTF? ??");
			Game::flagExit(69);
		}
	logger.detail("GAME",Game::getGameDir().c_str());

//// TEMPORARY ////

	graphics::Shader sh;
	sh.createShader("shaders/default.glsl");

	uint32_t v;
	glGenBuffers(1, &v);
	glBindBuffer(GL_ARRAY_BUFFER, v);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*3, (void*)0);
	glEnableVertexAttribArray(0);
//// TEMPORARY ////

	//Enter game loop
	loop();
}

//// Runs until the window is closed or the Game's exit flag is set
//// FPS and UPS updates occur independently and are currently defined in 'game.h'
void Game::loop() {
	// Compute the frame and update times
    const std::chrono::nanoseconds FRAME_TIME{ std::chrono::seconds{ 1 } / m_targ_fps };
    const std::chrono::nanoseconds UPDATE_TIME{ std::chrono::seconds{ 1 } / m_targ_ups };

    // Initialize the last frame and update times
    auto last_frame_time = std::chrono::steady_clock::now();
    auto last_update_time = std::chrono::steady_clock::now();
    auto last_print_time = std::chrono::steady_clock::now();
	auto current_time = std::chrono::steady_clock::now();
	auto frame_time_elapsed = current_time - last_frame_time;
    auto update_time_elapsed = current_time - last_update_time;
	auto elapsed_time = current_time - last_print_time;

	// Initialize FPS and UPS counters
	int fps = 0, ups = 0;

    // Game loop
	while(!glfwWindowShouldClose(m_window.getPtr()) && !Game::exitFlag) {
        // Compute the elapsed time since the last frame and update times
        current_time = std::chrono::steady_clock::now();
        frame_time_elapsed = current_time - last_frame_time;
        update_time_elapsed = current_time - last_update_time;

		glfwPollEvents();
		
		// Print FPS and UPS every second
		elapsed_time = current_time - last_print_time;
		if (elapsed_time >= std::chrono::seconds{ 2 }) {
			std::cout << CON_GRAY <<  fps/2 << " FPS    " << ups/2 << " UPS"<< std::endl;
			fps = 0;
			ups = 0;
			last_print_time = current_time;
		}

        // Render the frame if it's time
        if (frame_time_elapsed >= FRAME_TIME) {
            render(1);
			last_frame_time = current_time;
			fps++;
        }

        // Update the game state if it's time
        if (update_time_elapsed >= UPDATE_TIME) {
			tick(1);
            last_update_time = current_time;
			ups++;
        }
		
        
		// Sleep to avoid busy waiting
        const auto sleep_time = std::min(FRAME_TIME - frame_time_elapsed, UPDATE_TIME - update_time_elapsed);
        if (sleep_time > std::chrono::nanoseconds::zero()) {
            std::this_thread::sleep_for(sleep_time);
        }

		
    }
	exitGame();
}


//// Start of render pipeline
// NOTE: Currently it renders a primitive triangle whilst wiggling randomly its verices 
void Game::render(float dt) {
	//Once there will be a background/everything will be re-rendered,
	//can be removed for performance
	glClear(GL_COLOR_BUFFER_BIT);
	
	float t[] ={
		-.5f, -.5f, 0.f,
		0.f, .5f, 0.f,
		.5f, -.5f, 0.f};
	t[rand()%8]+=((rand()%4)-2)*0.01f; //Wiggling vertices

	glBufferData(GL_ARRAY_BUFFER, sizeof(t)*sizeof(t)/sizeof(t[0]), &t[0], GL_DYNAMIC_DRAW);
//	glDrawArrays(GL_TRIANGLES, 0, 3);

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
