#pragma once
#include "PhysicsObject.h"

const float MIN_LINEAR_THRESHOLD = 0.01f;
const float MIN_ROTATION_THRESHOLD = 0.1f;

class RigidBody : public PhysicsObject
{
public:
	RigidBody(ShapeType shapeID, glm::vec2 position,
			glm::vec2 velocity, float rotation, float mass, float linearDrag, float angularDrag, float elasticity);
	
	RigidBody(ShapeType shapeID, glm::vec2 position, float angularVelocity,
		glm::vec2 velocity, float rotation, float mass, float linearDrag, float angularDrag, float elasticity);
	
	~RigidBody();

	virtual void fixedUpdate(glm::vec2 gravity, float timeStep);
	virtual void debug();
	void applyForce(glm::vec2 force);
	void applyForceToActor(RigidBody* actor2, glm::vec2 force);

	void resolveCollision(RigidBody* actor2, glm::vec2 colNorm);

	glm::vec2 getPosition() { return m_position; }
	float getRotation() { return m_rotation; }
	glm::vec2 getVelocity() { return m_velocity; }
	float getMass() { return m_mass; }
	float getElasticity() { return m_elasticity; }
	float getLinearDrag() { return m_linearDrag; }
	float getAngularDrag() { return m_angularDrag; }

	void setVelocity(glm::vec2 newVelocity) { m_velocity = newVelocity; }
	void setPosition(glm::vec2 newPos) { m_position = newPos; }
	void setLinearDrag(float drag) { m_linearDrag = drag; }
	void setAngularDrag(float drag) { m_angularDrag = drag; }

	void changeMass(float amount);

protected:
	glm::vec2 m_position;
	glm::vec2 m_velocity;
	float m_angularVelocity = 0;
	float m_mass;
	float m_rotation;
	float m_elasticity = 1;

	float m_linearDrag;
	float m_angularDrag;
};

