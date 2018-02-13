#include "PhysicsScene.h"
#include "RigidBody.h"
#include <algorithm>
#include <cassert>
#include "Sphere.h"
#include <glm\glm.hpp>
#include <glm\ext.hpp>
#include "Plane.h"


PhysicsScene::PhysicsScene() : m_timeStep(0.01f), m_gravity(glm::vec2(0, 0))
{

}

PhysicsScene::~PhysicsScene()
{
	//pActor becomes the same type as m_actors
	for (auto pActor : m_actors)
	{
		delete pActor;
	}
}

void PhysicsScene::addActor(PhysicsObject * actor)
{
	//Pushes in the Actor into the std::vector array
	m_actors.push_back(actor);
}

void PhysicsScene::removeActor(PhysicsObject * actor)
{
	//Iter gets set to a vector array due to the auto
	for (auto iter = m_actors.begin(); iter != m_actors.end(); iter++)
	{
		//Dereferences iter since it is pretending
		//To be a PhysicsObject*
		if (*iter == actor)
		{
			//Erasses actor at position iter
			m_actors.erase(iter);
			//Jumps out the loop
			break;
		}
	}
}

void PhysicsScene::update(float deltaTime)
{
	//Update Physics at a fixed Time Step
	static std::list<PhysicsObject*> dirty;

	//Creates a static accumalatedTime and sets it to 0.0f
	static float accumulatedTime = 0.0f;
	accumulatedTime += deltaTime;

	while (accumulatedTime >= m_timeStep)
	{
		//auto (way to declare a variable that has a complicated type)
		for (auto pActor : m_actors)
		{
			pActor->fixedUpdate(m_gravity, m_timeStep);
		}


		accumulatedTime -= m_timeStep;

		//Check for collision (ideally you'd want to have some sort of
		//scene management in place)

		for (auto pActor : m_actors)
		{
			for (auto pOther : m_actors)
			{
				if (pActor == pOther)
					continue;
				if (std::find(dirty.begin(), dirty.end(), pActor/*constant*/) != dirty.end() &&
					std::find(dirty.begin(), dirty.end(), pOther) != dirty.end())
					continue;

				//Makes pActor appear as a Rigidbody
				//Dynamic_cast casts pActor to be a RigidBody
				RigidBody* pRigid = dynamic_cast<RigidBody*> (pActor);

				if (pRigid && pRigid->checkCollision(pOther) == true)
				{
					//Calls apply force to the actor(Object)
					pRigid->applyForceToActor
						//casts pOther to a RigidBody
						(dynamic_cast<RigidBody*>(pOther),
							//Gets the pRigid's velocity and * it by the mass
							pRigid->getVelocity() * pRigid->getMass());
					//Pushes both into the array
					dirty.push_back(pRigid);
					dirty.push_back(pOther);
				}
			}
		}
		//Clears the list
		dirty.clear();
	}

}

void PhysicsScene::updateGizmos()
{
	for (auto pActor : m_actors)			//Creates pActor as m_actors
	{
		pActor->makeGizmo();
	}
}

void PhysicsScene::debugScene()
{
	int count = 0;															
	for (auto pActor : m_actors)
	{
		std::cout << count << " : ";
		pActor->debug();
		count++;
	}
}

void PhysicsScene::setupContinuousDemo(glm::vec2 startPos, float inclination, float speed, float gravity)
{
	float t = 0;
	float tStep = 0.5f;																//tStep is the time difference between each frame
	float radius = 5.0f;
	int segments = 12;
	glm::vec4 colour = glm::vec4(0, 1, 1, 0);

	float posX;																		//Creates posX
	float posY;																		//Creates posY
	float degreesToRadiants = glm::pi<float>() / 180;								//Calculations for the inclinations (takes)
	inclination *= degreesToRadiants;												//Times equals the inclination by the degreesToRadiants


	glm::vec2 m_velocity(sin(inclination), cos(inclination) * speed);				//Velocty
	
	
	while (t <= 10)
	{
		startPos += speed * tStep /*- gravity*/;									//Calculate the x, y position of the projectile at time t

		posX = startPos.x + m_velocity.x * t;										//Position X velocity at time T
		posY = startPos.y + m_velocity.y * t + 0.5f * gravity * t * t;				//Position Y velocty at time T (squares time t * t)

		aie::Gizmos::add2DCircle(glm::vec2(posX, posY), radius, segments, colour);	//Draws the circle and colours it
		t += tStep;																	//increments t by tStep
	}
}

//Function pointer array for doing our collisions
typedef bool(*collisionFnc)(PhysicsObject*, PhysicsObject*);

static collisionFnc collisionFunctionArray[] =
{
	nullptr					  ,	PhysicsScene::plane2Sphere,
	PhysicsScene::sphere2Plane,	PhysicsScene::sphere2Sphere,
};

void PhysicsScene::checkForCollision()
{
	int actorCount = (int)m_actors.size();												//Creates actorCount and pushes m_actor's size in as an int

	//need to check for collisions against all objects except this one.
	for (int outer = 0; outer != actorCount - 1; outer++)
	{
		for (int inner = outer + 1; inner != actorCount; inner++)
		{
			auto object1 = m_actors[outer];												//Casts object1 as a PhysicsObject
			auto object2 = m_actors[inner];												//Casts object2 as a PhysicsObject
			int shapeId1 = (int)object1->getShapeID();									//Gets the shapeID for object1
			int shapeId2 = (int)object2->getShapeID();									//Gets the shapeID for object2

			//Using function pointers
			int functionIdx = (shapeId1 * SHAPE_COUNT + shapeId2);						//
			collisionFnc collisionFunctionPtr = collisionFunctionArray[functionIdx];	//Pushes in the collisionArray[functionIdx] into collisionFunctionPtr

			if (collisionFunctionPtr != nullptr)
			{
				collisionFunctionPtr(object1, object2);									//Did it collide?
			}
			
		}
	}
}

bool PhysicsScene::plane2Sphere(PhysicsObject * obj1, PhysicsObject * obj2)
{
	return sphere2Plane(obj2, obj1);
}

bool PhysicsScene::sphere2Plane(PhysicsObject * obj1, PhysicsObject * obj2)
{
	Sphere* sphere = dynamic_cast<Sphere*>(obj1);
	Plane* plane = dynamic_cast<Plane*>(obj2);

	//If we are successful on the test for collision
	if (sphere != nullptr && plane != nullptr)
	{

		glm::vec2 collisionNormal = plane->getNormal();
		float sphereToPlane = glm::dot(sphere->getPosition(), plane->getNormal()) + 
							           plane->getDistance();

		//If we are behind the plane then we flip the normal!
		if (sphereToPlane < 0)
		{
			collisionNormal *= -1;
			sphereToPlane *= -1;
		}

		float intersection = sphere->getRadius() - sphereToPlane;

		if (intersection > 0)
		{
			//Set sphere velocity to zero here
			sphere->setVelocity(glm::vec2(plane->getNormal() * sphere->getVelocity()) /** -1.0f glm::vec2(0, 0)*/);
			return true;
		}
	}
	
	return false;
}

bool PhysicsScene::sphere2Sphere(PhysicsObject * obj1, PhysicsObject * obj2)
{
	//Try to cast objects to sphere and sphere
	Sphere* sphere1 = dynamic_cast<Sphere*>(obj1);
	Sphere* sphere2 = dynamic_cast<Sphere*>(obj2);

	glm::vec2 vecBetween = sphere2->getPosition() - sphere1->getPosition();			//End - Start

	float distance = glm::length(vecBetween);										//GLM MAGNITUDE(Gets the length of the Vector we are halfing)

	//If we are successful then test for collision
	if (sphere1 != nullptr && sphere2 != nullptr)
	{
		//This is where the collision detection happens
		//You need code which sets velocity of the two spheres to zero
		//If they are overlapping

		//Get distance between 2 spheres
		//If distance is less than the combined radius of
		//both spheres, then a collision occurred so set the
		//velocity of both spheres to 0 (we'll add collision resolution later)
		if ((sphere1->getRadius() + sphere2->getRadius()) > distance )
		{
			sphere1->setVelocity(sphere1->getVelocity() * -1.0f/*glm::vec2(-10, 0)*/);
			sphere2->setVelocity(sphere2->getVelocity() * -1.0f/*glm::vec2(0, 10)*/);
		}
	}
	return false;
}
