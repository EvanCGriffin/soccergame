#include <SDL\SDL_opengl.h>
#include "Game.h"

int main( int argc, char* args[] ) 
{ 
	b2Vec2 gravity(0, 0);

	// Create our game with screen 640 * 480 * 32
	Game game = Game(640, 480, 32, gravity);

	// If initialise failed, exit with error
	if(game.Intialise("Year 3 prototype") == -1)
		return -1;

	// If everything loaded correctly, run the game!
	game.Run();

	// Game and Engine cleaned up so just return 0
	return 0; 
}



