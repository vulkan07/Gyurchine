#include <csignal>
#include "engine/game.h"


void signalHandler(int signum) {
	Game::getInstance().flagExit(signum);
	std::cout<<std::endl;
	Logger::getInstance().info("GAME","Termination Signal Received");
}

int main(int argc, char *argv[]) {
	std::cout << "\n+++[ Gyurchine started ]+++\n\n";
	
	signal(SIGTERM, signalHandler);
    signal(SIGINT, signalHandler);

	Game game = Game::getInstance();
	game.init(4,!false);
}

// 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
