#include "PhysicsSceneFixedTimeStepApp.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include <iostream>
#include "Sphere.h"


PhysicsSceneFixedTimeStepApp::PhysicsSceneFixedTimeStepApp() 
{

}

PhysicsSceneFixedTimeStepApp::~PhysicsSceneFixedTimeStepApp() 
{

}

bool PhysicsSceneFixedTimeStepApp::startup()
{
	//Increase the 2D line count to maximize the number of objects we can draw
	aie::Gizmos::create(255U, 255U, 65535U, 6555535U);

	m_2dRenderer = new aie::Renderer2D();

	// TODO: remember to change this when redistributing a build!
	// the following path would be used instead: "./font/consolas.ttf"
	m_font = new aie::Font("../bin/font/consolas.ttf", 32);

	//Creates physicsScene on the Heap
	m_physicsScene = new PhysicsScene();

	//Gravity control
	m_physicsScene->setGravity(glm::vec2(10, -90));
	m_physicsScene->setTimesStep(0.01f);

	//Creates the ball and all of the variables inside (pos, vel, mass/ radius, colour)
	Sphere* ball = new Sphere(glm::vec2(440, 0), glm::vec2(0, 0), 4.0f, 10, glm::vec4(1, 0, 1, 1));
	Sphere* ball2 = new Sphere(glm::vec2(20, 0), glm::vec2(0, 0), 4.0f, 24, glm::vec4(1, 1, 1, 1));
	Sphere* ball3 = new Sphere(glm::vec2(-150, 10), glm::vec2(0, 0), 4.0f, 14, glm::vec4(0, 1, 1, 0));

	//Adds the actor to the scene
	m_physicsScene->addActor(ball);
	m_physicsScene->addActor(ball2);
	m_physicsScene->addActor(ball3);

	//Collides with the other actor and produces force on the actor
	//Applies Force
	//ball2->applyForce(glm::vec2(0, 0));
	//ball3->applyForce(glm::vec2(0, 0));
	//ball2->applyForceToActor(ball, glm::vec2(7, 0));

	return true;
}

void PhysicsSceneFixedTimeStepApp::shutdown() 
{
	delete m_font;
	delete m_2dRenderer;
}

void PhysicsSceneFixedTimeStepApp::update(float deltaTime) 
{
	// input example
	aie::Input* input = aie::Input::getInstance();
		//Clears Gizmos per frame
	aie::Gizmos::clear();
	
	m_physicsScene->update(deltaTime);
	m_physicsScene->updateGizmos();


	Sphere* ball4 = new Sphere(glm::vec2(100, 200), glm::vec2(0, 0), 4.0f, 4, glm::vec4(1, 1, 0, 0));

	if (input->isKeyDown(aie::INPUT_KEY_LEFT))
	{
		m_physicsScene->addActor(ball4);
	}

	//Applies force to ball4
	ball4->applyForce(glm::vec2(20, 40));


	//physicsScene Updates
	m_physicsScene->update(deltaTime);
	m_physicsScene->updateGizmos();


	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void PhysicsSceneFixedTimeStepApp::draw()
{

	// wipe the screen to the background colour
	clearScreen();

	// begin drawing sprites
	m_2dRenderer->begin();

	// draw your stuff here!
	//Creates aspectRatio
	static float aspectRatio = 16 / 9.f;

	//Draws the 2D Object using Gizmos
	aie::Gizmos::draw2D(glm::ortho<float>(-640, 640, -640 / aspectRatio, 640 / aspectRatio, -1.0f, 1.0f));
	// output some text, uses the last used colour
	m_2dRenderer->drawText(m_font, "Press ESC to quit", 0, 0);
	m_2dRenderer->drawText(m_font, "Fixed Time Step", 500, 500);

	// done drawing sprites
	m_2dRenderer->end();
}