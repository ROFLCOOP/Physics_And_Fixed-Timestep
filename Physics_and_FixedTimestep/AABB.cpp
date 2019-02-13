#include "AABB.h"
#include <Gizmos.h>


AABB::AABB(glm::vec2 position, glm::vec2 velocity, float mass,
	glm::vec2 max, glm::vec4 colour) : RigidBody(BOX, position, velocity, 0, mass), m_minPos(-max), m_maxPos(max), m_colour(colour)
{
}

AABB::~AABB()
{
}

void AABB::makeGizmo()
{
	aie::Gizmos::add2DAABBFilled(m_position, m_maxPos, m_colour);
}

bool AABB::checkCollision(PhysicsObject * other)
{
	return false;
}
