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

	m_angularDrag = 0.3f;
	m_linearDrag = 0.3f;
	m_angularVelocity = 1.0f;
}

RigidBody::~RigidBody()
{

}

void RigidBody::fixedUpdate(glm::vec2 gravity, float timeStep)
{

	//if (m_isKinematic)
		//return;

	//Newtons Second Law of Motion
	m_velocity += gravity * timeStep;
	m_velocity += m_acceleration * timeStep;

	m_position += m_velocity * timeStep;									//position increments via velocity being times by TimeStep

	//m_velocity -= m_velocity * m_linearDrag * timeStep;						//We multiply the velocty by the drag value so that vel will slightly decrease each frame	
	m_rotation += m_angularVelocity * timeStep;
	//m_angularVelocity -= m_angularVelocity * m_angularDrag * timeStep;		//Threshold value which the object stops moving or rotating

	if (length(m_velocity) < MIN_LINEAR_THRESHOLD)
	{
		//m_velocity = glm::vec2(0, 0);
	}
	if (abs(m_angularVelocity) < MIN_ROTATION_THRESHOLD)
	{
		//m_angularDrag = 0;
	}

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

void RigidBody::resolveCollision(RigidBody * actor2, glm::vec2 contact, glm::vec2* collisionNormal)
{
	glm::vec2 normal = collisionNormal ? *collisionNormal : glm::normalize(actor2->m_position - m_position);								  //Calculates the collision normal (find the vector between their centres, 
																																			  //or use the provided direction of force)
	glm::vec2 relativeVelocity = actor2->getVelocity() - m_velocity;																		  //Calculates the relativeVelocity

	glm::vec2 perp(normal.y, -normal.x);																									  //Get the vector perpendicular to the collision normal

	//Determine the total velocity of the contact points for the two objects,
	//For both Linear and Rotational

	float r1 = glm::dot(contact - m_position, -perp);
	float r2 = glm::dot(contact - actor2->m_position, perp);

	float v1 = glm::dot(m_velocity, normal) - r1 * m_rotation;																				  //Velocity of the contact point on this object
	float v2 = glm::dot(actor2->m_velocity, normal) + r2 * actor2->m_rotation;																  //Velocity of contact point on actor 2

	if (v1 > v2)	//They are moving closer
	{
		//Calculate the effective mass at contact point for each object
		float mass1 = 1.0f / (1.0f / m_mass + (r1*r1) / m_moment);
		float mass2 = 1.0f / (1.0f / actor2->m_mass + (r2*r2) / actor2->m_moment);

		float elasticity = (m_elasticity + actor2->getElasticity()) / 2.0f;																		  //takes the average of the two object's elasticity values
		glm::vec2 force = (1.0f + elasticity)*mass1*mass2 / (mass1 + mass2)*(v1 - v2)*normal;

		//Apply equal force and opposite forces
		applyForce(-force, contact - m_position);
		actor2->applyForce(force, contact - actor2->getPosition());
	}																							  
}

void RigidBody::applyForce(glm::vec2 force, glm::vec2 pos)
{
	//Calculates acceleration by divinding force by mass
	m_acceleration = force / m_mass;
	m_angularVelocity += (force.y * pos.x - force.x * pos.y) / (m_moment);

}

bool RigidBody::checkCollision(PhysicsObject * pOther)
{
	return true;
}


