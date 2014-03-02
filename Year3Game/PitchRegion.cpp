#include "PitchRegion.h"
#include <Year3Engine\Utils.h>

PitchRegion::PitchRegion(double x, double y, double width, double height, int id)
	:id(id)
{
	rect = SDL_Rect();
	rect.x = x;
	rect.y = y;
	rect.w = width;
	rect.h = height;

	center = b2Vec2(x + (width * 0.5), y + (height * 0.5));

	color = b2Color(0, 0, 0);
}


PitchRegion::~PitchRegion(void)
{
}

void PitchRegion::Draw(DebugDraw* debugDraw)
{
	b2AABB aabb;
	aabb.lowerBound = b2Vec2(rect.x, rect.y + rect.h);
	aabb.upperBound = b2Vec2(rect.x + rect.w, rect.y);

	debugDraw->DrawCircle(center, 10, color);

	debugDraw->DrawAABB(&aabb, color);
}

inline b2Vec2 PitchRegion::GetRandomPosition()
{
	return b2Vec2(RandInRange(rect.x, (rect.x + rect.w)),
		RandInRange(rect.y, (rect.y + rect.h)));
}
