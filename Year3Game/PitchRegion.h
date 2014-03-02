#pragma once

#include <SDL\SDL.h>
#include <Box2D\Common\b2Math.h>
#include <Year3Engine\DebugDraw.h>

class PitchRegion
{
public:
	enum region_modifier{halfsize, normal};

	PitchRegion(double x, double y, double width, double height, int id);
	~PitchRegion(void);

	void Draw(DebugDraw* debugDraw);

	inline bool Inside(b2Vec2 position, region_modifier r=normal) const;
	inline b2Vec2 GetRandomPosition();

	SDL_Rect rect;
	b2Vec2 center;
	int id;
private:
	b2Color color;
};

inline bool PitchRegion::Inside(b2Vec2 position, region_modifier r) const
{
	if(r== normal)
	{
		return ((position.x > rect.x) && (position.x < (rect.x + rect.w)) &&
			(position.y > rect.y) && (position.y < (rect.y + rect.h)));
	}
	else
	{
		const double marginX = rect.w * 0.25;
		const double marginY = rect.h * 0.25;

		return ((position.x > (rect.x + marginX)) && (position.x < ((rect.x + rect.w)-marginX)) &&
			(position.y > (rect.y+marginY)) && (position.y < ((rect.y + rect.h)-marginY)));
	}
}

