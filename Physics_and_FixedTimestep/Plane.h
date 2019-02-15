#pragma once
#include "PhysicsObject.h"
#include "RigidBody.h"

class Plane : public PhysicsObject
{
public:
	Plane();
	Plane(const glm::vec2& p1, const glm::vec2& p2);
	Plane(glm::vec2 normal, float distanceToOrigin, glm::vec4 colour);
	~Plane();

	virtual void fixedUpdate(glm::vec2 gravity, float timestep) {}
	virtual void debug() {}
	virtual void resetPosition() { m_normal = glm::vec2(0, 1); m_distanceToOrigin = 0; }
	virtual void makeGizmo();


	bool checkCollision(PhysicsObject* other);

	glm::vec2 getNormal() { return m_normal; }
	float getDistance() { return m_distanceToOrigin; }

	void resolveCollision(RigidBody* other, glm::vec2 collisionNorm);

private:
	glm::vec2 m_normal;
	float m_distanceToOrigin;
	glm::vec4 m_colour;
};

