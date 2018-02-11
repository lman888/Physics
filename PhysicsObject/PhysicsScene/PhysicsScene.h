#pragma once
#include "PhysicsObject.h"
#include <Gizmos.h>
#include <glm\ext.hpp>
#include <glm\glm.hpp>
#include <vector>
#include <Gizmos.h>
#include <list>
#include <iostream>




class PhysicsScene
{
public:

	 
	PhysicsScene();
	~PhysicsScene();

	//Functions
	void addActor(PhysicsObject* actor);
	void removeActor(PhysicsObject* actor);
	void update(float deltaTime);
	void updateGizmos();
	void debugScene();

	void setupContinuousDemo(glm::vec2 startPos, float inclination,
							float speed, float gravity);

	//Set and Get Gravity
	glm::vec2 getGravity() const { return m_gravity; }
	void setGravity(const glm::vec2 gravity) { m_gravity = gravity;  }

	//Set and Get TimeStep
	float getTimeStep() const { return m_timeStep; }
	void setTimesStep(const float timeStep) { m_timeStep = timeStep; }

protected:
	//Intialise values
	glm::vec2 m_gravity;
	float m_timeStep;

	//Creates a vector array
	std::vector<PhysicsObject*> m_actors;
};

