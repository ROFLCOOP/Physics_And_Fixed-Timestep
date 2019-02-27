#include "SAT.h"
#include <Gizmos.h>
#include <cmath>

SAT::SAT(glm::vec2 position, glm::vec2 velocity, unsigned int sides, float radius, float rotation, float mass, float linearDrag, float angularDrag, float elasticity, glm::vec4 colour):
	RigidBody(POLYGON, position, velocity, rotation, mass, linearDrag, angularDrag, elasticity)
{
	float angle = ((PI * 2) / sides);
	glm::vec2 dir(sin(rotation), cos(rotation));
	for (int i = 0; i < sides; i++)
	{
		rotateAngle(dir, -(angle));
		m_verts.push_back(dir * radius);

	}

	m_colour = colour;
}

SAT::SAT(glm::vec2 position, glm::vec2 velocity, std::vector<glm::vec2> vertices, float rotation, float mass, float linearDrag, float angularDrag, float elasticity, glm::vec4 colour):
	RigidBody(POLYGON, position, velocity, rotation, mass, linearDrag, angularDrag, elasticity)
{
	m_verts = vertices;
	m_colour = colour;
}


SAT::~SAT()
{
}

void SAT::makeGizmo()
{
	std::vector<glm::vec2> verts = getVerts();
	for (int i = 0; i < verts.size(); i++)
	{
		if (verts[i] == verts.back())
			aie::Gizmos::add2DLine(verts[i], verts[0], m_colour);
		else
			aie::Gizmos::add2DLine(verts[i], verts[i + 1], m_colour);

		//aie::Gizmos::add2DCircle(verts[i], 0.5f, 16, glm::vec4(1, 1, 1, 1));
	}
}

std::vector<glm::vec2> SAT::getVerts() const
{
	std::vector<glm::vec2> vertices;
	for (auto vert : m_verts)
	{
		vertices.push_back(vert + m_position);
	}
	return vertices;
}

std::vector<glm::vec2> SAT::getEdges() const
{
	std::vector<glm::vec2> edges;
	for (int i = 0; i < m_verts.size(); i++)
	{
		if (i != m_verts.size() - 1)
			edges.push_back(m_verts[i] - m_verts[i + 1]);
		else
			edges.push_back(m_verts[i] - m_verts[0]);
	}

	return edges;
}

glm::vec2 SAT::project(glm::vec2 axis)
{
	float min = glm::dot(axis, getVerts()[0]);
	float max = min;
	for (int i = 1; i < getVerts().size(); i++)
	{
		float vert = glm::dot(axis, getVerts()[i]);
		if (vert < min)
			min = vert;
		else if (vert > max)
			max = vert;
	}
	return glm::vec2(min, max);
}

void SAT::rotateAngle(glm::vec2 & vec, float radians)
{
	if (radians == 0)
		return;

	float cs = cos(radians);
	float sn = sin(radians);
	float x = vec.x;
	float y = vec.y;

	vec.x = x * cs - y * sn;
	vec.y = x * sn + y * cs;
	
	vec = glm::normalize(vec);
}


//void rotateAngle(glm::vec2& vec, float radians)
//{
//	if (radians == 0)
//		return;
//
//	vec.x *= cos(radians);
//	vec.y *= sin(radians);
//}