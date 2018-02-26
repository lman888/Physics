#include "PhysicsScene.h"
#include <algorithm>
#include <cassert>
#include "Sphere.h"
#include "Box.h"
#include <glm\glm.hpp>
#include <glm\ext.hpp>
#include "Plane.h"

typedef PhysicsScene::CollisionData(*collisionFnc)(PhysicsObject*, PhysicsObject*);

//Function pointer array for doing our collisions
//typedef bool(*collisionFnc)(PhysicsObject*, PhysicsObject*);

static collisionFnc collisionFunctionArray[] =
{
	nullptr					  ,	PhysicsScene::Plane2Sphere,
	PhysicsScene::Plane2Box,    PhysicsScene::Sphere2Plane,	PhysicsScene::Sphere2Sphere,
	PhysicsScene::Sphere2Box,   PhysicsScene::Box2Plane,    PhysicsScene::Box2Sphere,
	PhysicsScene::Box2Box
};

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
		checkForCollision();
		//Check for collision (ideally you'd want to have some sort of
		//scene management in place)
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

void PhysicsScene::handleCollision(PhysicsObject * obj1, PhysicsObject * obj2, CollisionData & collision)
{
	auto rb1 = dynamic_cast<RigidBody*>(obj1);	//RigibBody 1
	auto rb2 = dynamic_cast<RigidBody*>(obj2);	//Rigidbody 2
	

	seperateCollisionObjects(rb1, rb2, collision);

	glm::vec2 relativeVelocity = { 0, 0 };
	if (rb1) relativeVelocity = rb1->getVelocity();
	if (rb2) relativeVelocity -= rb2->getVelocity();

	float elasticaty = 1;
	float invMass1 = (rb1) ? 1.0f / rb1->getMass() : 0.0f;
	float invMass2 = (rb2) ? 1.0f / rb2->getMass() : 0.0f;

	float jTop = -(1 + elasticaty) * glm::dot(relativeVelocity, collision.normal);
	float jBottom = glm::dot(collision.normal, collision.normal) * (invMass1 + invMass2);

	float j = jTop / jBottom;
	
	if (rb1)	//makes the object go in the opposite direction after the collision
	{
		glm::vec2 newVelocity = rb1->getVelocity() + (j / rb1->getMass()) * collision.normal;

		rb1->setVelocity(newVelocity);
		
	}

	if (rb2)	//makes the object go in the opposite direction after the collision
	{
		glm::vec2 newVelocity = rb2->getVelocity() - (j / rb2->getMass()) * collision.normal;

		rb2->setVelocity(newVelocity);
	}
	
}

void PhysicsScene::seperateCollisionObjects(RigidBody * rb1, RigidBody * rb2, PhysicsScene::CollisionData & collision)
{
	if (rb1 == nullptr && rb2 == nullptr)
	{
		return;
	}

	float object1MoveRatio = 0;
	float object2MoveRatio = 0;

	if (rb1 && rb2)
	{
		float totalMass = rb1->getMass() + rb2->getMass();
		object1MoveRatio = 1 - (rb1->getMass() / totalMass);
		object2MoveRatio = 1 - (rb2->getMass() / totalMass);
	}
	else if (rb1)
	{
		object1MoveRatio = 1.0f;																					//Move ratio set to 1
	}
	else if (rb2)
	{
		object2MoveRatio = 1.0f;																					//Move ratio set to 1
	}

	if (rb1)
	{
		rb1->setPosition(rb1->getPosition() + (object1MoveRatio * collision.overlap * collision.normal));			//Sets the position of the first Object
	}
	if (rb2)
	{
		rb2->setPosition(rb2->getPosition() - (object1MoveRatio * collision.overlap * collision.normal));			//Sets the position of the second Object
	}
}

void PhysicsScene::setupContinuousDemo(glm::vec2 startPos, float inclination, float speed, float gravity)
{
	float t = 0;
	float tStep = 0.5f;																				//tStep is the time difference between each frame
	float radius = 5.0f;
	int segments = 12;
	glm::vec4 colour = glm::vec4(0, 1, 1, 0);

	float posX;																						//Creates posX
	float posY;																						//Creates posY
	float degreesToRadiants = glm::pi<float>() / 180;												//Calculations for the inclinations (takes)
	inclination *= degreesToRadiants;																//Times equals the inclination by the degreesToRadiants


	glm::vec2 m_velocity(sin(inclination), cos(inclination) * speed);								//Velocty
	
	
	while (t <= 10)
	{
		startPos += speed * tStep /*- gravity*/;													//Calculate the x, y position of the projectile at time t

		posX = startPos.x + m_velocity.x * t;														//Position X velocity at time T
		posY = startPos.y + m_velocity.y * t + 0.5f * gravity * t * t;								//Position Y velocty at time T (squares time t * t)

		aie::Gizmos::add2DCircle(glm::vec2(posX, posY), radius, segments, colour);					//Draws the circle and colours it
		t += tStep;																				    //increments t by tStep
	}
}

void PhysicsScene::checkForCollision()
{
	int actorCount = (int)m_actors.size();															//Creates actorCount and pushes m_actor's size in as an int

	//need to check for collisions against all objects except this one.
	for (int outer = 0; outer != actorCount - 1; outer++)
	{
		for (int inner = outer + 1; inner != actorCount; inner++)
		{
			auto object1 = m_actors[outer];															//Casts object1 as a PhysicsObject
			auto object2 = m_actors[inner];															//Casts object2 as a PhysicsObject
			int shapeId1 = (int)object1->getShapeID();												//Gets the shapeID for object1
			int shapeId2 = (int)object2->getShapeID();												//Gets the shapeID for object2

			//Using function pointers
			int functionIdx = (shapeId1 * SHAPE_COUNT + shapeId2);								    //
			collisionFnc collisionFunctionPtr = collisionFunctionArray[functionIdx];			    //Pushes in the collisionArray[functionIdx] into collisionFunctionPtr
			
			if (collisionFunctionPtr != nullptr)
			{
				CollisionData collisionData = collisionFunctionPtr(object1, object2);				//Creates collisionData and pushes in both the objects in the array
				if (collisionData.wasCollision)
				{
					handleCollision(object1, object2, collisionData);								//Did it collide?
				}
			}
		}
	}
}

PhysicsScene::CollisionData PhysicsScene::Plane2Sphere(PhysicsObject * obj1, PhysicsObject * obj2)
{
	return Sphere2Plane(obj2, obj1);
}

PhysicsScene::CollisionData PhysicsScene::Sphere2Sphere(PhysicsObject * obj1, PhysicsObject * obj2)
{
	//Try to cast objects to sphere and sphere
	Sphere* sphere1 = dynamic_cast<Sphere*>(obj1);
	Sphere* sphere2 = dynamic_cast<Sphere*>(obj2);


	CollisionData collData;

																									  //If we are successful then test for collision
	if (sphere1 != nullptr && sphere2 != nullptr)
	{
		//This is where the collision detection happens
		//You need code which sets velocity of the two spheres to zero
		//If they are overlapping

		float distanceBetween = glm::length(sphere2->getPosition() - sphere1->getPosition());
		float minDistBetween = sphere1->getRadius() + sphere2->getRadius();

		//Get distance between 2 spheres
		//If distance is less than the combined radius of
		//both spheres, then a collision occurred so set the
		//velocity of both spheres to 0 (we'll add collision resolution later)
		if (distanceBetween < minDistBetween)
		{
			collData.wasCollision = true;																//Sets collision to true
			collData.normal = glm::normalize(sphere2->getPosition() - sphere1->getPosition());			//Normalizes both spheres positions
			collData.overlap = distanceBetween - minDistBetween;									//Pushes in both distances into the overlap

			return collData;
		}
	}
	return collData;
}

PhysicsScene::CollisionData PhysicsScene::Sphere2Plane(PhysicsObject * obj1, PhysicsObject * obj2)
{
	Sphere* sphere = dynamic_cast<Sphere*>(obj1);
	Plane* plane = dynamic_cast<Plane*>(obj2);

	CollisionData collData;
	

	//If we are successful on the test for collision
	if (sphere != nullptr && plane != nullptr)
	{
		glm::vec2 collisionNormal = plane->getNormal();

		glm::vec2 contact = sphere->getPosition() + (collisionNormal * -sphere->getRadius());

		float sphereToPlane = glm::dot(sphere->getPosition(), plane->getNormal()) -
							  plane->getDistance() - sphere->getRadius();

		if (sphereToPlane < 0)
		{
			collData.wasCollision = true;
			collData.normal = plane->getNormal();
			collData.overlap = -1 * sphereToPlane;
		}
	}
	return collData;
}

PhysicsScene::CollisionData PhysicsScene::Sphere2Box(PhysicsObject * obj1, PhysicsObject * obj2)
{
	return Box2Sphere(obj2, obj1);
}

PhysicsScene::CollisionData PhysicsScene::Box2Plane(PhysicsObject * obj1, PhysicsObject * obj2)
{
	/*Box* box = dynamic_cast<Box*>(obj1);
	Plane* plane = dynamic_cast<Plane*>(obj2);

	//If we are successful then test for collsion
	if (box != nullptr && plane != nullptr)
	{
		int numCounts = 0;
		glm::vec2 contact(0, 0);
		float contactV = 0;
		float radius = 0.5f * std::fminf(box->getWidth(), box->getHeight());

		//Which side is the centre of mass on?
		glm::vec2 planeOrigin = plane->getNormal() * plane->getDistance();
		float comFromPlane = glm::dot(box->getPosition() - planeOrigin,
									  plane->getNormal());

		//Check all four corners to see if we have hit the plane
		for (float x = -box->getExtents().y; x < box->getWidth(); x+= box->getWidth())
		{
			for (float y = -box->getExtents().y; y < box->getHeight(); y+= box->getHeight())
			{
				//Get the position of the corner in world space
				glm::vec2 p = box->getPosition() + x * box->getLocalX() +
					          y * box->getLocalY();

				float distFromPlane = glm::dot(p - planeOrigin, plane->getNormal());

				//This is the velocity of the point
				float velocityIntoPlane = glm::dot(box->getVelocity() + box->getRotation() *
												  (-y * box->getLocalX() + x * box->getLocalY()), plane->getNormal());

				//If this corner is on the opposite side from the COM.
				and moving further in, we need to resolve the collsion
				if ((distFromPlane > 0 && comFromPlane < 0 && velocityIntoPlane > 0) ||
					(distFromPlane < 0 && comFromPlane > 0 && velocityIntoPlane < 0))
				{
					numCounts++;
					contact += p;
					contactV += velocityIntoPlane;
				}
			}
		}

		//We have had a hit - typically only two corners can contact
		if (numCounts > 0)
		{
			//Get the average collision velocity into the plane
			//(covers linear and rotational velocity of all corners involved)
			float CollisionV = contactV / (float)numCounts;

			//Get the acceleration required to stop (restitution = 0) or reverse
			//(restitution = 1) the average velocity of all corners involved
			glm::vec2 acceleration = -plane->getNormal() * ((1.0f + box->getElasticity()) * CollisionV);

			//And the average position at which we will apply the force
			//(corner or edge centre)
			glm::vec2 localContact = (contact / (float)numCounts) - box->getPosition();

			//This is the perpendicular distance we apply the force at relative to the COM, so torque = F*r
			float r = glm::dot(localContact, glm::vec2(plane->getNormal().y,
													   plane->getNormal().x));

			//Work out the "effective mass" - this is a combination of moment of 
			//inertia and mass, and tells us how much the contact point velocity
			//will change with the force we are applying
			float mass0 = 1.0f / (1.0f / box->getMass() + (r*r) / box->getMoment());

			//And apply force
			box->applyForce(acceleration * mass0, localContact);
		}
	}*/
	Box* box = dynamic_cast<Box*>(obj1);
	Plane* plane = dynamic_cast<Plane*>(obj2);

	CollisionData colldata;

	//If we are successful then test for collision
	if (box != nullptr && plane != nullptr)
	{
		//The distance between the box and the plane
		float distanceBetween = glm::dot(box->getPosition(), plane->getNormal()) - plane->getDistance();

		float d = (distanceBetween - glm::length(box->getExtents() * plane->getNormal()));

		//If check for collision
		if (d < 0)
		{
			colldata.wasCollision = true;
			colldata.normal = plane->getNormal();
			colldata.overlap = d;
		}
	}

	return colldata;
}

PhysicsScene::CollisionData PhysicsScene::Box2Sphere(PhysicsObject * obj1, PhysicsObject * obj2)
{
	auto box = dynamic_cast<Box*>(obj1);
	auto sphere = dynamic_cast<Sphere*>(obj2);

	CollisionData collData;

	//If we are successful then test for collision
	if (box != nullptr && sphere != nullptr)
	{
		glm::vec2 offset = (sphere->getPosition() - box->getPosition());

		//Clamp the offset distance between the sphere and box
		//To find the closest point on the box to the sphere
		if (std::fabsf(offset.x) > box->getExtents().x)
		{
			offset.x = offset.x < 0 ? box->getExtents().x * -1 : box->getExtents().x;
		}
		if (std::fabsf(offset.y) > box->getExtents().y)
		{
			offset.y = offset.y < 0 ? box->getExtents().y * -1 : box->getExtents().y;
		}
		glm::vec2 closestPointOnBox = box->getPosition() + offset;

		//Find the distance between the sphere and the closest point on the box
		glm::vec2 actualOffSet = sphere->getPosition() - closestPointOnBox;

		if (glm::length(actualOffSet) < sphere->getRadius())
		{
			collData.wasCollision = true;
			collData.normal = glm::normalize(actualOffSet);
			collData.overlap = sphere->getRadius() - glm::length(actualOffSet);
		}
	}
	return collData;
}

PhysicsScene::CollisionData PhysicsScene::Box2Box(PhysicsObject * obj1, PhysicsObject * obj2)
{
	//Try to cast objects to box and box
	auto* box2 = dynamic_cast<Box*>(obj1);
	auto* box1 = dynamic_cast<Box*>(obj2);

	CollisionData collData;

	//If we are successful then test for collision
	if (box1 != nullptr && box2 != nullptr)
	{
		//The distance between the two boxs
		float distance = glm::length(box2->getPosition() - box1->getPosition());
		//The Min X of box 1
		float box1MinX = box1->getPosition().x - box1->getExtents().x;
		//The Min X of box 2
		float box2MinX = box2->getPosition().x - box2->getExtents().x;
		//The Min Y of box 1 
		float box1MinY = box1->getPosition().y - box1->getExtents().y;
		//The Min Y of box 2
		float box2MinY = box2->getPosition().y - box1->getExtents().y;
		//The Max X of box 1
		float box1MaxX = box1->getPosition().x + box1->getExtents().x;
		//The Max X of box 2
		float box2MaxX = box2->getPosition().x + box2->getExtents().x;
		//The Max Y of box 1
		float box1MaxY = box1->getPosition().y + box1->getExtents().y;
		//The Max Y of box 2
		float box2MaxY = box2->getPosition().y + box2->getExtents().y;

		//If check for collision
		if (box1MinX < box2MaxX && box1MaxX > box2MinX && box1MinY < box2MaxY && box1MaxY > box2MinY)
		{
			float xSeperationBox1Box2 = box1MinX - box2MaxX;
			float xSeperationBox2Box1 = box1MaxX - box2MinX;
			float ySeperationBox1Box2 = box1MinY - box2MaxY;
			float ySeperationBox2Box1 = box1MaxY - box2MinY;
			float overlapX = std::fabsf(xSeperationBox1Box2) < std::fabsf(xSeperationBox2Box1) ? xSeperationBox1Box2 : xSeperationBox2Box1;
			float overlapY = std::fabsf(ySeperationBox1Box2) < std::fabsf(ySeperationBox2Box1) ? ySeperationBox1Box2 : ySeperationBox2Box1;
			glm::vec2 overlap = std::fabsf(overlapX) < std::fabsf(overlapY) ? glm::vec2(overlapX, 0) : glm::vec2(0, overlapY);

			collData.wasCollision = true;
			collData.normal = glm::normalize(overlap);
			collData.overlap = glm::length(overlap);
		}
	}

	return collData;
}

PhysicsScene::CollisionData PhysicsScene::Plane2Box(PhysicsObject * obj1, PhysicsObject * obj2)
{
	return Box2Plane(obj2, obj1);
}
