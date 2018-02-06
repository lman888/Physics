#include "Sphere.h"
#include <Gizmos.h>



Sphere::Sphere(glm::vec2 position, glm::vec2 velocity, float mass, float radius, 
glm::vec4 colour) :
	RigidBody(SPHERE, position, velocity, 0, mass)
{
	m_radius = radius;
	m_colour = colour;
}

Sphere::~Sphere()
{
	
}

void Sphere::makeGizmo()
{
	//Makes and adds the Circle
	//Adds in the values that have bene pre-set
	aie::Gizmos::add2DCircle(m_position, m_radius, 32, m_colour);
}

bool Sphere::checkCollision(PhysicsObject * pOther)
{
	//Dynamically casts pOther into a Sphere
	//Pushes pOther* into sOther*
	Sphere* sOther = dynamic_cast<Sphere*>(pOther);

	if (sOther != nullptr)
	{
		float distance = glm::distance(getPosition(), sOther->getPosition());
		{
			if (distance < getRadius() + sOther->getRadius())
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}
}
