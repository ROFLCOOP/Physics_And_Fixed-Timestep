#include "Sphere.h"
#include <Gizmos.h>


Sphere::Sphere(glm::vec2 position, glm::vec2 velocity,
	float mass, float radius, glm::vec4 colour, int sides) :
	RigidBody(SPHERE, position, velocity, 0, mass)
{
	m_radius = radius;
	m_colour = colour;
	m_sides = sides;
}


Sphere::~Sphere()
{
	
}

void Sphere::makeGizmo()
{
	aie::Gizmos::add2DCircle(m_position, m_radius, m_sides, m_colour);
}

bool Sphere::checkCollision(PhysicsObject * pOther)
{
	return false;
}



