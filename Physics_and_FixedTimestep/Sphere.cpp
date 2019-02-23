#include "Sphere.h"
#include <Gizmos.h>


Sphere::Sphere(glm::vec2 position, glm::vec2 velocity,
	float mass, float radius, glm::vec4 colour, int sides, float linearDrag, float angularDrag, float elasticity) :
	RigidBody(SPHERE, position, velocity, 0, mass, linearDrag, angularDrag, elasticity)
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
	//aie::Gizmos::add2DLine(m_position, m_position + (glm::vec2(1, 0) * m_radius), glm::vec4(0, 0, 0, 1));
}





