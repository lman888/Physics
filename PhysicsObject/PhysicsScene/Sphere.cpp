#include "Sphere.h"
#include <Gizmos.h>



Sphere::Sphere(glm::vec2 position, glm::vec2 velocity, float mass, float radius, 
glm::vec4 colour) :
	RigidBody(SPHERE, position, velocity, 0, mass)
{
	m_radius = radius;
	m_colour = colour;
	m_moment = 0.5f * m_mass * m_radius;
}

Sphere::~Sphere()
{
	
}

void Sphere::makeGizmo()
{
	//Makes and adds the Circle
	glm::vec2 end = glm::vec2(std::cos(m_rotation), std::sin(m_rotation)) * m_radius;

	//Adds in the values that have bene pre-set
	aie::Gizmos::add2DCircle(m_position, m_radius, 32, m_colour);
	aie::Gizmos::add2DLine(m_position, m_position + end, glm::vec4(1, 1, 1, 1));
}

bool Sphere::checkCollision(PhysicsObject * pOther)
{
//	////Dynamically casts pOther into a Sphere
//	////Pushes pO5ther* into sOther*
//	//Sphere* sOther = dynamic_cast<Sphere*>(pOther);
//
//	//if (sOther != nullptr)
//	//{
//	//	float distance = glm::distance(getPosition(), sOther->getPosition());
//	//	{
//	//		if (distance < getRadius() + sOther->getRadius())
//	//		{
//	//			return true;
//	//		}
//	//		else
//	//		{
//	//			return false;
//	//		}
//	//	}
//	//}
	return true;
}
