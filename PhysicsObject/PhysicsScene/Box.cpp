#include "Box.h"
#include <Gizmos.h>



Box::Box(glm::vec2 position, glm::vec2 velocity, float mass, glm::vec2 extents, glm::vec4 colour)
	    : RigidBody(BOX, position, velocity, 0, mass)
{
	m_position = position;
	m_velocity = velocity;
	m_colour = colour;
	m_mass = mass;
	m_extents = extents;
}

Box::~Box()
{
}

void Box::fixedUpdate(glm::vec2 gravity, float timeStep)
{
	RigidBody::fixedUpdate(gravity, timeStep);

	//m_rotation is using the angular velocity during the call to the base class implementation of fixedUpdte()
	float cs = cosf(m_rotation);
	float sn = sinf(m_rotation);
	m_localX = glm::normalize(glm::vec2(cs, sn));
	m_localY = glm::normalize(glm::vec2(-sn, cs));
}

void Box::makeGizmo()
{
	//If only using rotation
	//glm::mat4 transform = glm::rotate(m_rotation, glm::vec3(0, 0, 1));
	//aie::Gizmos::add2DAABBFilled(getCentre(),
	//							   m_extents, m_colour, &transform);

	//Draw using local axes
	//glm::vec2 p1 = m_position - m_localX * m_extents.x - m_localY * m_extents.y;
	//glm::vec2 p2 = m_position + m_localX * m_extents.x - m_localY * m_extents.y;
	//glm::vec2 p3 = m_position - m_localX * m_extents.x + m_localY * m_extents.y;
	//glm::vec2 p4 = m_position + m_localX * m_extents.x + m_localY * m_extents.y;
	//aie::Gizmos::add2DTri(p1, p2, p4, m_colour);
	//aie::Gizmos::add2DTri(p1, p4, p3, m_colour);
	aie::Gizmos::add2DAABBFilled(m_position, m_extents, m_colour);
}

bool Box::checkCollision()
{
	return false;
}