#include "Plane.h"
#include <glm\glm.hpp>
#include <glm\ext.hpp>
#include <iostream>
#include <Gizmos.h>




Plane::Plane() : PhysicsObject(ShapeType::PLANE)
{
	m_distanceToOrigin = 0;											   //Sets distanceToOrigin to 0
	m_normal = glm::vec2(0, 1);										   //Sets the X and Y of m_normal
}


Plane::~Plane()
{

}

void Plane::fixedUpdate(glm::vec2 gravity, float timeStep)
{

}

void Plane::makeGizmo() const
{
	const float lineSegmentLength = 300;								//Sets lineseegmentLength to 300
	glm::vec2 centrePoint = m_normal * m_distanceToOrigin;				//Creates centrePoint via timesng m_normal by m_distanceToOrigin
	glm::vec4 colour(1, 1, 1, 1);										//Sets the colour
	glm::vec2 parrallel(m_normal.y, -m_normal.x);						//Creates start
	glm::vec2 start = centrePoint + parrallel * lineSegmentLength;		//
	glm::vec2 end = centrePoint - parrallel * lineSegmentLength;		//
	aie::Gizmos::add2DLine(start, end, colour);							//Draws the Line
}

void Plane::resetPosition()
{

}
