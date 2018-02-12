#pragma once
#include "RigidBody.h"


class Sphere : public RigidBody
{
public:
	Sphere(glm::vec2 position, glm::vec2 velocity,
			float mass, float radius, glm::vec4 colour);
	~Sphere();
	
	//Creates makeGizmo and checkCollision
	virtual void makeGizmo();
	virtual bool checkCollision(PhysicsObject* pOther);

	//Gets Radius and Colour
	float getRadius() { return m_radius; }
	glm::vec4 getColour() { return m_colour; }

	void setVelocity(glm::vec2 a_velocity) { m_velocity = a_velocity; }

protected:
	//Initializes variables
	float m_radius;
	glm::vec4 m_colour;
};

