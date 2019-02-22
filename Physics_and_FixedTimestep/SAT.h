#pragma once
#include <vector>
#include "RigidBody.h"
class SAT : public RigidBody
{
public:
	SAT(glm::vec2 position, glm::vec2 velocity, int sides, float radius, float rotation, float mass, float linearDrag, float angularDrag, float elasticity, glm::vec4 colour);
	SAT(glm::vec2 position, glm::vec2 velocity, std::vector<glm::vec2> vertices, float rotation, float mass, float linearDrag, float angularDrag, float elasticity, glm::vec4 colour);
	~SAT();

	virtual void makeGizmo();

	std::vector<glm::vec2> getVerts() const;

private:
	std::vector<glm::vec2> m_verts;
	glm::vec4 m_colour = glm::vec4 (1.0f, 1.0f, 1.0f, 0.0f);

	
};

