#include "GameScreen.h"
#include "Engine.h"

GameScreen::GameScreen()
	:paused(false)
{
}


GameScreen::~GameScreen(void)
{
	components.clear();
}

void GameScreen::LoadContent()
{
}

void GameScreen::HandleInput(Input input)
{
	for (compIter = components.begin(); compIter != components.end(); compIter++)
		(*compIter)->HandleInput(input);
}

void GameScreen::Update(float deltaTime)
{
	for (compIter = components.begin(); compIter != components.end(); compIter++)
		(*compIter)->Update(deltaTime);
}

void GameScreen::Draw()
{
	for (compIter = components.begin(); compIter != components.end(); compIter++)
		(*compIter)->Draw();
}

void GameScreen::AddComponent(shared_ptr<GameComponent> component)
{
	component->parent = this;

	component->LoadContent();

	components.push_back(component);
}