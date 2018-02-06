#include "RigidBody.h"






RigidBody::RigidBody(ShapeType shapeID, glm::vec2 position, glm::vec2 velocity, float rotation, float mass)
	: PhysicsObject(shapeID)
{
	m_position = position;
	m_velocity = velocity;
	m_rotation = rotation;
	m_mass = mass;
	m_shapeID = shapeID;
	m_acceleration = {0, 0};
}

RigidBody::~RigidBody()
{

}

void RigidBody::fixedUpdate(glm::vec2 gravity, float deltaTime)
{
	//Newtons Second Law of Motion
	m_velocity += gravity * deltaTime;
	m_velocity += m_acceleration * deltaTime;
	//position increments via velocity being times by TimeStep
	m_position += m_velocity * deltaTime;

	static float aspectRatio = (16.0f / 9.0f);

	if (m_position.x < -640)
	{
		m_position.x = 640 - aspectRatio;
	}
	if (m_position.x > 640)
	{
		m_position.x = -640 + aspectRatio;
	}
	if (m_position.y < -640 / aspectRatio)
	{
		m_position.y = 640 / aspectRatio;
	}
	if (m_position.y > 640 / aspectRatio)
	{
		m_position.y = -640 / aspectRatio;
	}



	//Resets acceleration to 0
	//m_acceleration = { 0, 0 };
}

void RigidBody::debug()
{

}

void RigidBody::applyForce(glm::vec2 force)
{
	//Calculates acceleration by divinding force by mass
	m_acceleration = force / m_mass;

}

void RigidBody::applyForceToActor(RigidBody * actor2, glm::vec2 force)
{
	//applyForce(force);
	//actor2->applyForce(-force);
}
