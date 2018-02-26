#pragma once

#include "Application.h"
#include "Renderer2D.h"
#include "PhysicsSceneFixedTimeStepApp.h"
#include "PhysicsScene.h"
#include "Sphere.h"
#include "Plane.h"
#include "Box.h"

//Virtual allows for overriding functions (google it)

class PhysicsSceneFixedTimeStepApp : public aie::Application {
public:

	PhysicsSceneFixedTimeStepApp();
	virtual ~PhysicsSceneFixedTimeStepApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

protected:

	aie::Renderer2D*	m_2dRenderer;
	aie::Font*			m_font;

	//Creates a physicsScene Pointer
	PhysicsScene* m_physicsScene;

	Sphere* colBall1;
	Sphere* colBall2;
	Sphere* colBall3;
	Box* box1;

	Plane* plane;
};