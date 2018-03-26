#pragma once
#include "PhysicsObject.h"
#include <Gizmos.h>
#include "RigidBody.h"
#include <glm\ext.hpp>
#include <glm\glm.hpp>
#include <vector>
#include <Gizmos.h>
#include <list>
#include <iostream>




class PhysicsScene
{
public:

	struct CollisionData
	{
		bool wasCollision = false;		//sets collision to false
		glm::vec2 normal = { 0, 0 };	
		float overlap = 0.0f;
	};
	
public:
	PhysicsScene();
	~PhysicsScene();

	//Functions
	void addActor(PhysicsObject* actor);
	void removeActor(PhysicsObject* actor);
	void update(float deltaTime);
	void updateGizmos();
	void debugScene();
	void handleCollision(PhysicsObject* obj1, PhysicsObject* obj2, CollisionData& collision);
	void seperateCollisionObjects(RigidBody* rb1, RigidBody* rb2, PhysicsScene::CollisionData& collision);
	void setupContinuousDemo(glm::vec2 startPos, float inclination,
							float speed, float gravity);

	//Set and Get Gravity
	glm::vec2 getGravity() const { return m_gravity; }
	void setGravity(const glm::vec2 gravity) { m_gravity = gravity;  }

	//Set and Get TimeStep
	float getTimeStep() const { return m_timeStep; }
	void setTimesStep(const float timeStep) { m_timeStep = timeStep; }

	void checkForCollision();

	static CollisionData Plane2Sphere(PhysicsObject* obj1, 
							          PhysicsObject* obj2);

	static CollisionData Sphere2Sphere(PhysicsObject* obj1,
							           PhysicsObject* obj2);

	static CollisionData Sphere2Plane(PhysicsObject* obj1,
							          PhysicsObject* obj2);

	static CollisionData Sphere2Box(PhysicsObject* obj1,
									PhysicsObject* obj2);

	static CollisionData Box2Plane(PhysicsObject* obj1,
								   PhysicsObject* obj2);

	static CollisionData Box2Sphere(PhysicsObject* obj1,
									PhysicsObject* obj2);

	static CollisionData Box2Box(PhysicsObject* obj1,
								 PhysicsObject* obj2);
	  
	static CollisionData Plane2Box(PhysicsObject* obj1,
								   PhysicsObject* obj2);

protected:
	//Intialise values
	glm::vec2 m_gravity;
	float m_timeStep;

	//Creates a vector array
	std::vector<PhysicsObject*> m_actors;
};

