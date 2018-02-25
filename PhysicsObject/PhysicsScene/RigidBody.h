#pragma once
#include "PhysicsObject.h"
#include <glm\glm.hpp>
#include <glm\ext.hpp>
#define MIN_LINEAR_THRESHOLD 0.1
#define MIN_ROTATION_THRESHOLD 0.01


class RigidBody : public PhysicsObject
{
public:
	RigidBody(ShapeType shapeID, glm::vec2 position,
			glm::vec2 velocity, float rotation, float mass);
	~RigidBody();

	//Initializes Update and degub
	virtual void fixedUpdate(glm::vec2 gravity, float timeStep);
	virtual void debug();
	void resolveCollision(RigidBody* actor2, glm::vec2 contact, glm::vec2* collisionNormal = nullptr);

	//Applying Forces
	void applyForce(glm::vec2 force, glm::vec2 pos);

	//Checks Collision
	bool checkCollision(PhysicsObject* pOther) ;

	float getMass() { return m_mass; }

	glm::vec2 getVelocity() { return m_velocity; }
	void setVelocity(glm::vec2 a_velocity) { m_velocity = a_velocity; }

	glm::vec2 getPosition() { return m_position; }
	void setPosition(glm::vec2 a_position) { m_position = a_position; }
	
	float getLinearDrag() { return m_linearDrag; }
	void setLinearDrag(float a_linearDrag) { m_linearDrag = a_linearDrag; }
	
	float getAngularDrag() { return m_angularDrag; }
	void setAngularDrag(float a_AngularDrag) { m_angularDrag = a_AngularDrag; }

	float getElasticity() { return m_elasticity; }
	void setElasticity(float a_elasiticity) { m_elasticity = a_elasiticity; }

	float getRotation() { return m_rotation; }
	void setRotation(float a_rotation) { m_rotation = a_rotation; }

	float getMoment() { return m_moment; }
	void setMoment(float a_moment) { m_moment = a_moment; }

	float getAngularVel() { return m_angularVelocity; }
	void setAngularVel(float a_angularVel) { m_angularVelocity = a_angularVel; }




protected:
	float m_rotation;				//How much the object is rotated in radians
	float m_moment;					//represents the moment of inertia
	float m_angularVelocity;
	float m_linearDrag;
	float m_angularDrag;

	float m_elasticity;
	glm::vec2 m_position;
	glm::vec2 m_velocity;
	float m_mass;
	//float m_rotation; //2D so we only need a single float to represent our rotation
	glm::vec2 m_acceleration;
};

