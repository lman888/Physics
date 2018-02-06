#include "PhysicsScene.h"
#include "RigidBody.h"


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

				if (pRigid->checkCollision(pOther) == true)
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
	for (auto pActor : m_actors)
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
	float tStep = 0.5f;
	float radius = 1.0f;
	int segments = 12;
	glm::vec4 colour = glm::vec4(1, 0, 1, 1);

	while (t <= 5)
	{
		//Calculate the x, y position of the projectile at time t
		startPos += speed * tStep;

		aie::Gizmos::add2DCircle(glm::vec2(startPos.x, startPos.y), radius, segments, colour);
		t += tStep;
	}
}
