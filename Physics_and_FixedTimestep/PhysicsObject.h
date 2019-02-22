#pragma once
#include <glm/ext.hpp>

enum ShapeType
{
	PLANE = 0,
	SPHERE = 1,
	BOX = 2,
	POLYGON = 3 // or SAT
};

class PhysicsObject
{
protected:
	PhysicsObject(ShapeType a_shapeID) : m_shapeID(a_shapeID) {}

public:
	virtual void fixedUpdate(glm::vec2 gravity, float timeStep) = 0;
	virtual void debug() = 0;
	virtual void makeGizmo() = 0;
	virtual void resetPosition() {};

	const ShapeType getShapeID();

protected:
	ShapeType m_shapeID;
};