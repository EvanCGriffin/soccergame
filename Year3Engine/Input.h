#pragma once

#include <map>
#include <SDL\SDL.h>
#include "Keys.h"

// Helpful input handler
class Input
{
public:
	Input(void);
	~Input(void);

	bool Update(SDL_Surface* screen);

	// returns true if key is currently pressed
	bool IsKeyDown(Keys::keys Key);

	// returns true if key is currently released
	bool IsKeyUp(Keys::keys Key);

	// return true if key is now pressed
	bool IsNewKeyPress(Keys::keys key);

private:
	std::map<int, bool> currentKeysState;
	std::map<int, bool> previousKeysState;
	int currentMouseX;
	int currentMouseY;
	int previousMouseX;
	int previousMouseY;
};

