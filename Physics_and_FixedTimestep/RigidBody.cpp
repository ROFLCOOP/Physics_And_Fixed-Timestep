#include "RigidBody.h"

RigidBody::RigidBody(ShapeType shapeID, glm::vec2 position, glm::vec2 velocity, float rotation, float mass, float linearDrag, float angularDrag, float elasticity) : PhysicsObject(shapeID), m_position(position), m_velocity(velocity), m_rotation(rotation), m_mass(mass), m_linearDrag(linearDrag), m_angularDrag(angularDrag), m_elasticity(elasticity)
{
}

RigidBody::RigidBody(ShapeType shapeID, glm::vec2 position, float angularVelocity, glm::vec2 velocity, float rotation, float mass, float linearDrag, float angularDrag, float elasticity) : PhysicsObject(shapeID), m_position(position), m_velocity(velocity), m_rotation(rotation), m_mass(mass), m_linearDrag(linearDrag), m_angularDrag(angularDrag), m_elasticity(elasticity), m_angularVelocity(angularVelocity)
{
}

RigidBody::~RigidBody()
{
}

void RigidBody::fixedUpdate(glm::vec2 gravity, float timeStep)
{
	if (m_isKinematic) return;

	m_velocity -= m_velocity * m_linearDrag * timeStep;
	m_rotation -= m_rotation * m_angularDrag * timeStep;

	if (m_velocity.length() < MIN_LINEAR_THRESHOLD)
	{
		m_velocity = glm::vec2(0, 0);
	}
	if (m_rotation < MIN_ROTATION_THRESHOLD)
	{
		m_rotation = 0.0f;
	}

	applyForce(gravity * m_mass * timeStep);
	m_position += m_velocity * timeStep;
}

void RigidBody::debug()
{
}

void RigidBody::applyForce(glm::vec2 force)
{
	m_velocity += force / m_mass;
}

void RigidBody::applyForceToActor(RigidBody * actor2, glm::vec2 force)
{
	actor2->applyForce(force);
	applyForce(-force);
}

void RigidBody::resolveCollision(RigidBody * actor2, glm::vec2 colNorm)
{
	glm::vec2 normal = colNorm; //glm::normalize(actor2->getPosition() - m_position);
	glm::vec2 relativeVelocity = actor2->getVelocity() - m_velocity;

	float elasticity = (m_elasticity + actor2->getElasticity()) * 0.5;
	float j = glm::dot(-(1 + elasticity) * (relativeVelocity), normal) /
		glm::dot(normal, normal * ((1 / m_mass) + (1 / actor2->getMass())));

	glm::vec2 force = normal * j;

	applyForceToActor(actor2, force);
}

void RigidBody::changeMass(float amount)
{
	m_mass += amount;
}
