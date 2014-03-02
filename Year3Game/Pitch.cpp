#include "Pitch.h"

const int NumRegionsHorizontal = 3; 
const int NumRegionsVertical   = 6;

/*		-538
	|			|
	|			|
	|			|	
-362|			|362
	|			|
	|			|
		538
*/

Pitch::Pitch()
	: Sprite("Content//Sprites//pitch_new.png", "Pitch"),
	leftBoundary(new PitchBoundary(-362, 0, 0.5, 22)),
	rightBoundary(new PitchBoundary(362, 0, 0.5, 22)),
	topLeftBoundary(new PitchBoundary(-202, -538, 5.9, 0.5)),
	topRightBoundary(new PitchBoundary(202, -538, 5.9, 0.5)),
	bottomLeftBoundary(new PitchBoundary(-202, 538, 5.9, 0.5)),
	bottomRightBoundary(new PitchBoundary(202, 538, 5.9, 0.5)),
	topGoal(new Goal(0, -535, 2.2, 0.4, "topGoal")),
	bottomGoal(new Goal(0, 535, 2.2, 0.4, "bottomGoal")),
	regions(NumRegionsHorizontal*NumRegionsVertical)
{
	rotation = 90;

	pitchArea = new PitchRegion(-362, -538, 724, 1076, -1);

	CreateRegions(pitchArea->rect.w / (double)NumRegionsHorizontal,
		pitchArea->rect.h / (double)NumRegionsVertical);
}


Pitch::~Pitch(void)
{
	delete pitchArea;
	
	regionIter = regions.begin();

	while(regionIter != regions.end())
	{
		delete *regionIter;
		regionIter = regions.erase(regionIter);
	}
}

void Pitch::LoadContent()
{
	Sprite::LoadContent();

	parent->AddComponent(leftBoundary);
	parent->AddComponent(rightBoundary);
	parent->AddComponent(topLeftBoundary);
	parent->AddComponent(topRightBoundary);
	parent->AddComponent(bottomLeftBoundary);
	parent->AddComponent(bottomRightBoundary);
	parent->AddComponent(topGoal);
	parent->AddComponent(bottomGoal);
}

void Pitch::Draw(DebugDraw* debugDraw)
{
	for(regionIter=regions.begin(); regionIter != regions.end(); ++regionIter)
	{
		(*regionIter)->Draw(debugDraw);
	}
	
}

void Pitch::CreateRegions(double width, double height)
{
	//index into the vector
  int idx = regions.size()-1;
    
  for (int col=0; col<NumRegionsHorizontal; ++col)
  {
    for (int row=0; row<NumRegionsVertical; ++row)
    {
      regions[idx--] = new PitchRegion(-362 + col * width,
                                   -538 + row * height,
                                   width,
                                   height,
                                   idx);
    }
  }
}