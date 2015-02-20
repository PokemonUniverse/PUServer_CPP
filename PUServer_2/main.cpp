#include "definitions.h"

#include "configmanager.h"
#include "game.h"
#include "server.h"
#include "database.h"

ConfigManager g_config;
Game g_game;

void pause()
{
	printf("*** Press 'Enter' to continue... ***");

	while (getchar() != '\n');
}

int main()
{
	// Load config file
	if(!g_config.loadFile("data/server.conf")) {
		std::cerr << ":: Failed to read config file ::" << std::endl;

		pause();
		return EXIT_FAILURE;
	}

	Database* db = Database::getFactory();
	if(!db || !db->isConnected()) {
		std::cerr << ":: Failed to startup database connection ::" << std::endl;

		pause();
		return EXIT_FAILURE;
	}
	
	if(!g_game.loadGame()) {
		std::cerr << ":: Failed to startup the game ::" << std::endl;

		pause();
		return EXIT_FAILURE;
	}

	// Running the server
	Server::instance()->startServer();
	
	pause();
	return EXIT_SUCCESS;
}