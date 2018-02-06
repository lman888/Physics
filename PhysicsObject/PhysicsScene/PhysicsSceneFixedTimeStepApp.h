#pragma once

#include "Application.h"
#include "Renderer2D.h"
#include "PhysicsSceneFixedTimeStepApp.h"
#include "PhysicsScene.h"

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

};