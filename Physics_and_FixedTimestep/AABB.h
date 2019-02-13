#pragma once
#include "RigidBody.h"

class AABB : public RigidBody
{
public:
	AABB(glm::vec2 position, glm::vec2 velocity, float mass, glm::vec2 max, glm::vec4 colour);
	~AABB();

	void makeGizmo();

	bool checkCollision(PhysicsObject* other);

	const glm::vec2 getMinPos() const { return m_position + m_minPos; }
	const glm::vec2 getMaxPos() const { return m_position + m_maxPos; }

private:
	glm::vec2 m_minPos;
	glm::vec2 m_maxPos;
	glm::vec4 m_colour;
};

