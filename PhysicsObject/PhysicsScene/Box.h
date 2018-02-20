#pragma once
#include "RigidBody.h"

class Box : public RigidBody
{
public:
	Box(glm::vec2 position, glm::vec2 velocity, float mass, glm::vec2 extents, glm::vec4 colour);
	~Box();

	virtual void fixedUpdate(glm::vec2 gravity, float timeStep);
	virtual void makeGizmo();
	virtual bool checkCollision();


protected:

	glm::vec2 m_extents;	//Halfdge lengths
	glm::vec4 m_colour;

	//Store the local x, y axes of the box based on its angle of rotation
	glm::vec2 m_localX;
	glm::vec2 m_localY;
};

