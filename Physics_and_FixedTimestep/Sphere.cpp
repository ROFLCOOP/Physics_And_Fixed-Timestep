#include "Sphere.h"



Sphere::Sphere(glm::vec2 position, glm::vec2 velocity,
	float mass, float radius, glm::vec4 colour) :
	RigidBody(SPHERE, position, velocity, 0, mass)
{
	m_radius = radius;
	m_colour = colour;
}


Sphere::~Sphere()
{
	
}

void Sphere::makeGizmo()
{
	
}

bool Sphere::checkCollision(PhysicsObject * pOther)
{
	Sphere* other = (Sphere*)pOther;
	float minCollision = m_radius + other->getRadius();
	float distance = glm::distance(m_position, other->getPosition());

	if (distance <= minCollision)
		return true;
	else
		return false;
}

