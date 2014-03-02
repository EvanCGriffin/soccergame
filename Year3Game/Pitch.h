#pragma once

#include <Year3Engine\Sprite.h>
#include <Year3Engine\GameScreen.h>
#include <Year3Engine\DebugDraw.h>
#include "PitchBoundary.h"
#include "Goal.h"
#include "PitchRegion.h"

// Draws the pitch and sets up the boundaries and goals
class Pitch : public Sprite
{
public:
	Pitch();
	~Pitch(void);

	void LoadContent();
	void Draw(DebugDraw* debugDraw);

	shared_ptr<PitchBoundary> leftBoundary;
	shared_ptr<PitchBoundary> rightBoundary;
	shared_ptr<PitchBoundary> topLeftBoundary;
	shared_ptr<PitchBoundary> topRightBoundary;
	shared_ptr<PitchBoundary> bottomLeftBoundary;
	shared_ptr<PitchBoundary> bottomRightBoundary;

	shared_ptr<Goal> topGoal;
	shared_ptr<Goal> bottomGoal;

	const PitchRegion* PitchArea() const { return pitchArea; }

	const PitchRegion* const GetRegionFromIndex(int idx)                                
	{
		assert ( (idx >= 0) && (idx < (int)regions.size()) );

		return regions[idx];
	}

private:
	//this instantiates the regions the players utilize to  position
	//themselves
	void CreateRegions(double width, double height);

	PitchRegion* pitchArea;

	std::vector<PitchRegion*> regions;
	std::vector<PitchRegion*>::iterator regionIter;
};

