#pragma once
#include "RigidBody.h"

class Box : public RigidBody
{
public:
	Box(glm::vec2 position, glm::vec2 velocity, float mass, glm::vec2 size, glm::vec4 colour);
	~Box();

	virtual void fixedUpdate(glm::vec2 gravity, float timeStep);
	virtual void makeGizmo();
	bool checkCollision();

	float getWidth() { return m_size.x; }
	float getHeight() { return m_size.y; }
	glm::vec2 getExtents() { return m_extents; }
	glm::vec2 getLocalX() { return m_localX; }
	glm::vec2 getLocalY() { return m_localY; }

protected:

	glm::vec2 m_extents;	//Halfdge lengths
	glm::vec4 m_colour;
	glm::vec2 m_size;

	//Store the local x, y axes of the box based on its angle of rotation
	glm::vec2 m_localX;
	glm::vec2 m_localY;
};

