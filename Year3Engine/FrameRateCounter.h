#pragma once

#include "GameComponent.h"
#include "EngineFont.h"
#include <sstream>

class FrameRateCounter : public GameComponent
{
public:
	FrameRateCounter(void);
	~FrameRateCounter(void);

	void LoadContent();
	void Update(float deltaTime);
	void Draw();

private:
	EngineFont font;
	int frameRate;
	int frameCounter;
	float elapsedTime;
	std::stringstream stringStream;
};

