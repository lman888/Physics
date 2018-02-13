#include "Plane.h"
#include <glm\glm.hpp>
#include <glm\ext.hpp>
#include <iostream>
#include <Gizmos.h>




Plane::Plane(glm::vec2 normal, float distance) : PhysicsObject(ShapeType::PLANE)
{
	m_distanceToOrigin = distance;											  
	m_normal = glm::normalize(normal);														  
}


Plane::~Plane()
{

}

void Plane::fixedUpdate(glm::vec2 gravity, float timeStep)
{

}

void Plane::makeGizmo()
{
	float lineSegmentLength = 1000;										//Sets the length of the Plane to 1000
	glm::vec2 centrePoint = m_normal * -m_distanceToOrigin;				//Creates centrePoint via timesng m_normal by m_distanceToOrigin
	glm::vec4 colour(1, 1, 1, 1);										//Sets the colour
	glm::vec2 parrallel(m_normal.y, -m_normal.x);						//Creates parralel
	glm::vec2 start = centrePoint + (parrallel * lineSegmentLength);	//Start of the line
	glm::vec2 end = centrePoint - (parrallel * lineSegmentLength);		//End of the line
	aie::Gizmos::add2DLine(start, end, colour);							//Draws the Line
}

void Plane::resetPosition()
{

}
