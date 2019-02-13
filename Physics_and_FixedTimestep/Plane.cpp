#include "Plane.h"
#include <Gizmos.h>

Plane::Plane() : PhysicsObject(PLANE)
{
	m_normal = glm::vec2(0, 1);
	m_distanceToOrigin = 0;
	m_colour = glm::vec4(1, 1, 1, 1);
}

Plane::Plane(const glm::vec2& p1, const glm::vec2& p2) : PhysicsObject(PLANE)
{
	glm::vec2 v = p2 - p1;
	glm::normalize(v);

	m_normal.x = -v.y;
	m_normal.y = v.x;

	m_distanceToOrigin = glm::dot(p1, m_normal);

	m_colour = glm::vec4(1, 1, 1, 1);
}

Plane::Plane(glm::vec2 normal, float distanceToOrigin, glm::vec4 colour) : PhysicsObject(PLANE), m_normal(normal), m_distanceToOrigin(distanceToOrigin), m_colour(colour)
{
}

Plane::~Plane()
{
}

void Plane::makeGizmo()
{
	float lineSegmentLength = 300.0f;
	glm::vec2 centerPoint = m_normal * -m_distanceToOrigin;

	glm::vec2 parallel(m_normal.y, -m_normal.x);
	glm::vec2 start = centerPoint + (parallel * lineSegmentLength);
	glm::vec2 end = centerPoint - (parallel * lineSegmentLength);
	aie::Gizmos::add2DLine(start, end, m_colour);
}


bool Plane::checkCollision(PhysicsObject * other)
{
	return false;
}
