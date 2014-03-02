#include "Input.h"


Input::Input(void)
{
}


Input::~Input(void)
{
}

bool Input::Update(SDL_Surface* screen)
{
	SDL_Event event;

	previousKeysState = currentKeysState;
	previousMouseX = currentMouseX;
	previousMouseY = currentMouseY;

	while (SDL_PollEvent(&event))
	{
		switch(event.type)
		{
		case SDL_QUIT:
			return true;
			break;
		case SDL_KEYDOWN:
			currentKeysState[event.key.keysym.sym] = true;
			break;
		case SDL_KEYUP:
			currentKeysState[event.key.keysym.sym] = false;
			break;
		case SDL_MOUSEMOTION:
			currentMouseX = event.motion.x;
			currentMouseY = screen->h - event.motion.y;
			break;
		case SDL_MOUSEBUTTONUP:
			//do input core stuff here
			break;
		case SDL_MOUSEBUTTONDOWN:
			//do input core stuff here
			break;
		default:
			break;
		}
	}

	return false;
}

// returns true if key is currently pressed
bool Input::IsKeyDown(Keys::keys key)
{
	return currentKeysState[key];
}

// returns true if key is currently released
bool Input::IsKeyUp(Keys::keys key)
{
	return !currentKeysState[key];
}

// return true if key is now pressed
bool Input::IsNewKeyPress(Keys::keys key)
{
	return currentKeysState[key] && !previousKeysState[key];
}