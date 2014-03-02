#pragma once

#include <Box2D\Common\b2Math.h>
#include <vector>
#include <Year3Engine\DebugDraw.h>

class Team;

class SupportSpotCalculator
{
public:
	SupportSpotCalculator(int numX, int numY, Team* team);
	~SupportSpotCalculator(void);

	//this method iterates through each possible spot and calculates its
	//score.
	b2Vec2  DetermineBestSupportingPosition();

	//returns the best supporting spot if there is one. If one hasn't been
	//calculated yet, this method calls DetermineBestSupportingPosition and
	//returns the result.
	b2Vec2  GetBestSupportingSpot();

	void Draw(DebugDraw* debugDraw);

private:

	struct SupportSpot
	{
		b2Vec2 position;
		double score;

		SupportSpot(b2Vec2 position, double score)
			: position(position), score(score)
		{}
	};

	Team* team;
	std::vector<SupportSpot> spots;

	//a pointer to the highest valued spot from the last update
	SupportSpot* bestSupportSpot;

	b2Color color;

	int frameCounter;
	int noOfFramesBeforeUpdate;
};

