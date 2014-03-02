#include "SupportSpotCalculator.h"
#include "PitchRegion.h"
#include "Team.h"
#include "SoccerMatch.h"

SupportSpotCalculator::SupportSpotCalculator(int numX, int numY, Team* team)
	:bestSupportSpot(NULL),
	team(team),
	frameCounter(0),
	noOfFramesBeforeUpdate(100)
{
	const PitchRegion* PlayingField = team->match->pitch->PitchArea();

	//calculate the positions of each sweet spot, create them and 
	//store them in m_Spots
	double HeightOfSSRegion = PlayingField->rect.h * 1;
	double WidthOfSSRegion  = PlayingField->rect.w * 0.5;
	double SliceX = WidthOfSSRegion / numX ;
	double SliceY = HeightOfSSRegion / numY;

	double left  = PlayingField->rect.x + (PlayingField->rect.w-WidthOfSSRegion)/2.0 + SliceX/2.0;
	double right = (PlayingField->rect.x + PlayingField->rect.w) - (PlayingField->rect.w-WidthOfSSRegion)/2.0 - SliceX/2.0;
	double top   = PlayingField->rect.y + (PlayingField->rect.h-HeightOfSSRegion)/2.0 + SliceY/2.0;
	double bottom = (PlayingField->rect.y + PlayingField->rect.h) - (PlayingField->rect.h - HeightOfSSRegion) / 2.0 - SliceY/2.0;

	for (int y=0; y< (numY / 2) -1; ++y)
	{
		for (int x=0; x< numX; ++x)
		{
			if (team->Home())
			{
				spots.push_back(SupportSpot(b2Vec2(left + x * SliceX, top + y * SliceY), 0.0));
			}

			else
			{
				spots.push_back(SupportSpot(b2Vec2(left + x * SliceX, bottom - y * SliceY), 0.0));
			}
		}
	}

	color = b2Color(1, 0, 0);
}


SupportSpotCalculator::~SupportSpotCalculator(void)
{
}

//--------------------------- DetermineBestSupportingPosition -----------------
//
//  see header or book for description
//-----------------------------------------------------------------------------
b2Vec2 SupportSpotCalculator::DetermineBestSupportingPosition()
{
	//only update the spots every few frames                              

	frameCounter++;

	if((frameCounter < noOfFramesBeforeUpdate) && bestSupportSpot)
	{
		return bestSupportSpot->position;
	}

	frameCounter = 0; // reset the counter

	//reset the best supporting spot
	bestSupportSpot = NULL;

	double BestScoreSoFar = 0.0;

	std::vector<SupportSpot>::iterator curSpot;

	for (curSpot = spots.begin(); curSpot != spots.end(); ++curSpot)
	{
		//first remove any previous score. (the score is set to one so that
		//the viewer can see the positions of all the spots if he has the 
		//aids turned on)
		curSpot->score = 1.0;

		//Test 1. is it possible to make a safe pass from the ball's position 
		//to this position?
		if(team->isPassSafeFromAllOpponents(team->ControllingPlayer()->GetPositionInPixels(),
			curSpot->position,
			NULL,
			50)) //Prm.MaxPassingForce))
		{
			curSpot->score += 2.0; //Prm.Spot_PassSafeScore;
		}


		//Test 2. Determine if a goal can be scored from this position.  
		if( team->CanShoot(curSpot->position,            
			30)) //Prm.MaxPassingForce))
		{
			curSpot->score +=  1.0; //Prm.Spot_CanScoreFromPositionScore;
		}   


		//Test 3. calculate how far this spot is away from the controlling
		//player. The further away, the higher the score. Any distances further
		//away than OptimalDistance pixels do not receive a score.
		if (team->SupportingPlayer())
		{
			const double OptimalDistance = 40000.0;

			double dist = DistanceSq(team->ControllingPlayer()->GetPositionInPixels(),
				curSpot->position);

			double temp = fabs(OptimalDistance - dist);

			if (temp < OptimalDistance)
			{

				//normalize the distance and add it to the score
				curSpot->score += 2.0 * //Prm.Spot_DistFromControllingPlayerScore *
					(OptimalDistance-temp)/OptimalDistance;  
			}
		}

		//check to see if this spot has the highest score so far
		if (curSpot->score > BestScoreSoFar)
		{
			BestScoreSoFar = curSpot->score;

			bestSupportSpot = &(*curSpot);
		}    

	}

	return bestSupportSpot->position;
}

//------------------------------- GetBestSupportingSpot -----------------------
//-----------------------------------------------------------------------------
b2Vec2 SupportSpotCalculator::GetBestSupportingSpot()
{
	if (bestSupportSpot)
	{
		return bestSupportSpot->position;
	}

	else
	{ 
		return DetermineBestSupportingPosition();
	}
}

void SupportSpotCalculator::Draw(DebugDraw* debugDraw)
{
	std::vector<SupportSpot>::iterator iter = spots.begin();

	while(iter != spots.end())
	{
		debugDraw->DrawCircle(iter->position, 10, color);
		++iter;
	}
}