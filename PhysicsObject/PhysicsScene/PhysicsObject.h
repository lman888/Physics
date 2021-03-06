#pragma once
#include <glm\glm.hpp>
#include <glm\ext.hpp>

enum ShapeType
{
	PLANE,
	SPHERE,
	BOX,
	SHAPE_COUNT
};

class PhysicsObject
{
public:

protected:

	//Creates and deletes the default constructor
	PhysicsObject() = delete;
	//Overloaded Constructor 
	PhysicsObject(ShapeType a_shapeID) : m_shapeID(a_shapeID) {}

public:
	//Initializing functions
	virtual void fixedUpdate(glm::vec2 gravity, float timeStep) = 0;
	virtual void debug() = 0;
	virtual void makeGizmo() = 0;
	virtual void resetPosition() {};

	//Gets ShapeID
	ShapeType getShapeID() { return m_shapeID; }
	void setShapeID(ShapeType a_ShapeID) { m_shapeID = a_ShapeID; }


protected:
	ShapeType m_shapeID;

	
};

