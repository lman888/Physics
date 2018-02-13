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

void RigidBody::resolveCollision(RigidBody * actor2)
{
	glm::vec2 normal = glm::normalize(actor2->getPosition - m_position);
	glm::vec2 relativeVelocity = actor2->getVelocity() - m_velocity;

	float elasticity = 1;
	float j = glm::dot(-(1 + elasticity) * (relativeVelocity), normal) /
		      glm::dot(normal, normal * ((1 / m_mass) + (1 / actor2->getMass()))); //J = -(1 + e) Vrel . n / Calculates the impulse magnitude
																			       //     n.n(1/Ma + 1/Mb)
	applyForceToActor(actor2, j * normal);										   //
}
