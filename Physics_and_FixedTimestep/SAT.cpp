#include "SAT.h"
#include <Gizmos.h>


SAT::SAT(glm::vec2 position, glm::vec2 velocity, int sides, float radius, float rotation, float mass, float linearDrag, float angularDrag, float elasticity, glm::vec4 colour):
	RigidBody(POLYGON, position, velocity, rotation, mass, linearDrag, angularDrag, elasticity)
{
	glm::vec2 dir(0, 1);
	for (int i = 0; i < sides; i++)
	{
		m_verts.push_back(dir * radius);
	}
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

		aie::Gizmos::add2DCircle(verts[i], 0.5f, 16, glm::vec4(1, 1, 1, 1));
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
