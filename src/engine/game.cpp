#include "engine/game.h"

Game::Game(int loglevel, bool fullscreen) {
	m_window.init(1920,1080,"Gyurchine");
	m_window.setFullscreen(fullscreen);
	m_window.setClearColor(1.f,0.5f,0.f);
	logger.setLogLevel(loglevel);

////
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
////

	loop();
}

void Game::loop() {

    float timePerFrame = 1.0 / m_targ_fps;
    float timePerUpdate = 1.0 / m_targ_ups;

    float lastFrameTime = glfwGetTime();
    float lastUpdateTime = lastFrameTime;
	float deltaTime = 0.0;

	logger.detail("GAME","Game loop started");
	while(!glfwWindowShouldClose(m_window.getPtr())) {
		
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
	exit();
}

void Game::render(float dt) {
	float t[] =
	{
		-.5f, -.5f, 0.f,
		0.f, .5f, 0.f,
		.5f, -.5f, 0.f
	};
	t[rand()%8]+=((rand()%4)-2)*0.001f;
	glBufferData(GL_ARRAY_BUFFER, sizeof(t)*sizeof(t)/sizeof(t[0]), &t[0], GL_DYNAMIC_DRAW);
	glClear(GL_COLOR_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glfwSwapBuffers(m_window.getPtr());
	glfwPollEvents();
}

void Game::tick(float dt) {
}

void Game::exit() {
	m_window.close();
}
