#pragma once
#include <vector>
#include "RigidBody.h"

const float PI = 3.1415926f;

class SAT : public RigidBody
{
public:
	// This constructor is more "user friendly", it takes in how many sides there are and the "radius" or distance from the centre that each vertex should be (kinda similar to gizmo circles, I think)
	SAT(glm::vec2 position, glm::vec2 velocity, unsigned int sides, float radius,
		float rotation, float mass, float linearDrag, float angularDrag,
		float elasticity, glm::vec4 colour);

	// This constructor is for more crude shapes, and a lot easier to shoot yourself in the foot with, as you can accidentally make a concave shape (perhaps make a catch for this in future)
	SAT(glm::vec2 position, glm::vec2 velocity, std::vector<glm::vec2> vertices,
		float rotation, float mass, float linearDrag, float angularDrag,
		float elasticity, glm::vec4 colour);


	~SAT();

	virtual void makeGizmo();

	// Each vert position is stored as a position in reference to the centre point, and therefore needs to be added to the centre point before being used for drawing
	std::vector<glm::vec2> getVerts() const;

	std::vector<glm::vec2> getEdges() const;

private:
	std::vector<glm::vec2> m_verts;
	glm::vec4 m_colour = glm::vec4 (1.0f, 1.0f, 1.0f, 0.0f);

	void rotateAngle(glm::vec2& vec, float radians);
};

