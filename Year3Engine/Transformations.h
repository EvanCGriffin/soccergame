#ifndef TRANSFORMATIONS_H
#define TRANSFORMATIONS_H
//------------------------------------------------------------------------
//
//  Name:   Transformations.h
//
//  Desc:   Functions for converting 2D vectors between World and Local
//          space.
//
//  Author: Mat Buckland (fup@ai-junkie.com)
//
//------------------------------------------------------------------------
#include <vector>

#include <Box2D\Common\b2Math.h>
#include "C2DMatrix.h"
#include "Transformations.h"






//--------------------------- WorldTransform -----------------------------
//
//  given a std::vector of 2D vectors, a position, orientation and scale,
//  this function transforms the 2D vectors into the object's world space
//------------------------------------------------------------------------
inline std::vector<b2Vec2> WorldTransform(std::vector<b2Vec2> &points,
                                            const b2Vec2   &pos,
                                            const b2Vec2   &forward,
                                            const b2Vec2   &side,
                                            const b2Vec2   &scale)
{
	//copy the original vertices into the buffer about to be transformed
  std::vector<b2Vec2> Tranb2Vec2s = points;
  
  //create a transformation matrix
	C2DMatrix matTransform;
	
	//scale
  if ( (scale.x != 1.0) || (scale.y != 1.0) )
  {
	  matTransform.Scale(scale.x, scale.y);
  }

	//rotate
	matTransform.Rotate(forward, side);

	//and translate
	matTransform.Translate(pos.x, pos.y);
	
  //now transform the object's vertices
  matTransform.Transformb2Vec2s(Tranb2Vec2s);

  return Tranb2Vec2s;
}

//--------------------------- WorldTransform -----------------------------
//
//  given a std::vector of 2D vectors, a position and  orientation
//  this function transforms the 2D vectors into the object's world space
//------------------------------------------------------------------------
inline std::vector<b2Vec2> WorldTransform(std::vector<b2Vec2> &points,
                                 const b2Vec2   &pos,
                                 const b2Vec2   &forward,
                                 const b2Vec2   &side)
{
	//copy the original vertices into the buffer about to be transformed
  std::vector<b2Vec2> Tranb2Vec2s = points;
  
  //create a transformation matrix
	C2DMatrix matTransform;

	//rotate
	matTransform.Rotate(forward, side);

	//and translate
	matTransform.Translate(pos.x, pos.y);
	
  //now transform the object's vertices
  matTransform.Transformb2Vec2s(Tranb2Vec2s);

  return Tranb2Vec2s;
}

//--------------------- PointToWorldSpace --------------------------------
//
//  Transforms a point from the agent's local space into world space
//------------------------------------------------------------------------
inline b2Vec2 PointToWorldSpace(const b2Vec2 &point,
                                    const b2Vec2 &AgentHeading,
                                    const b2Vec2 &AgentSide,
                                    const b2Vec2 &AgentPosition)
{
	//make a copy of the point
  b2Vec2 TransPoint = point;
  
  //create a transformation matrix
	C2DMatrix matTransform;

	//rotate
	matTransform.Rotate(AgentHeading, AgentSide);

	//and translate
	matTransform.Translate(AgentPosition.x, AgentPosition.y);
	
  //now transform the vertices
  matTransform.Transformb2Vec2s(TransPoint);

  return TransPoint;
}

//--------------------- VectorToWorldSpace --------------------------------
//
//  Transforms a vector from the agent's local space into world space
//------------------------------------------------------------------------
inline b2Vec2 VectorToWorldSpace(const b2Vec2 &vec,
                                     const b2Vec2 &AgentHeading,
                                     const b2Vec2 &AgentSide)
{
	//make a copy of the point
  b2Vec2 TransVec = vec;
  
  //create a transformation matrix
	C2DMatrix matTransform;

	//rotate
	matTransform.Rotate(AgentHeading, AgentSide);

  //now transform the vertices
  matTransform.Transformb2Vec2s(TransVec);

  return TransVec;
}


//--------------------- PointToLocalSpace --------------------------------
//
//------------------------------------------------------------------------
inline b2Vec2 PointToLocalSpace(const b2Vec2 &point,
                             b2Vec2 &AgentHeading,
                             b2Vec2 &AgentSide,
                              b2Vec2 &AgentPosition)
{

	//make a copy of the point
  b2Vec2 TransPoint = point;
  
  //create a transformation matrix
	C2DMatrix matTransform;

  double Tx = - b2Dot(AgentPosition, AgentHeading);
  double Ty = - b2Dot(AgentPosition, AgentSide);

  //create the transformation matrix
  matTransform._11(AgentHeading.x); matTransform._12(AgentSide.x);
  matTransform._21(AgentHeading.y); matTransform._22(AgentSide.y);
  matTransform._31(Tx);           matTransform._32(Ty);
	
  //now transform the vertices
  matTransform.Transformb2Vec2s(TransPoint);

  return TransPoint;
}

//--------------------- VectorToLocalSpace --------------------------------
//
//------------------------------------------------------------------------
inline b2Vec2 VectorToLocalSpace(const b2Vec2 &vec,
                             const b2Vec2 &AgentHeading,
                             const b2Vec2 &AgentSide)
{ 

	//make a copy of the point
  b2Vec2 TransPoint = vec;
  
  //create a transformation matrix
	C2DMatrix matTransform;

  //create the transformation matrix
  matTransform._11(AgentHeading.x); matTransform._12(AgentSide.x);
  matTransform._21(AgentHeading.y); matTransform._22(AgentSide.y);
	
  //now transform the vertices
  matTransform.Transformb2Vec2s(TransPoint);

  return TransPoint;
}

//-------------------------- Vec2DRotateAroundOrigin --------------------------
//
//  rotates a vector ang rads around the origin
//-----------------------------------------------------------------------------
inline void Vec2DRotateAroundOrigin(b2Vec2& v, double ang)
{
  //create a transformation matrix
  C2DMatrix mat;

  //rotate
  mat.Rotate(ang);
	
  //now transform the object's vertices
  mat.Transformb2Vec2s(v);
}

//------------------------ CreateWhiskers ------------------------------------
//
//  given an origin, a facing direction, a 'field of view' describing the 
//  limit of the outer whiskers, a whisker length and the number of whiskers
//  this method returns a vector containing the end positions of a series
//  of whiskers radiating away from the origin and with equal distance between
//  them. (like the spokes of a wheel clipped to a specific segment size)
//----------------------------------------------------------------------------
inline std::vector<b2Vec2> CreateWhiskers(unsigned int  NumWhiskers,
                                            double        WhiskerLength,
                                            double        fov,
                                            b2Vec2      facing,
                                            b2Vec2      origin)
{
  //this is the magnitude of the angle separating each whisker
  double SectorSize = fov/(double)(NumWhiskers-1);

  std::vector<b2Vec2> whiskers;
  b2Vec2 temp;
  double angle = -fov*0.5; 

  for (unsigned int w=0; w<NumWhiskers; ++w)
  {
    //create the whisker extending outwards at this angle
    temp = facing;
    Vec2DRotateAroundOrigin(temp, angle);
    whiskers.push_back(origin + WhiskerLength * temp);

    angle+=SectorSize;
  }

  return whiskers;
}


#endif