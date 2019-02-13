#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "PhysicsObject.h"
#include "RigidBody.h"
#include "Sphere.h"
#include "Plane.h"
#include "AABB.h"



class PhysicsScene
{
public:
	PhysicsScene();
	~PhysicsScene();


	void addActor(PhysicsObject* actor); //add an actor to the m_actors vector
	void removeActor(PhysicsObject* actor);	//remove an actor from the m_actors vector
	void update(float dt);	//update function for calling all the update functions in physics items and handling collision
	void updateGizmos();	//for drawing actors

	void setGravity(const glm::vec2 gravity) { m_gravity = gravity; }
	glm::vec2 getGravity() const { return m_gravity; }

	void setTimeStep(const float timeStep) { m_timeStep = timeStep; }
	float getTimeStep() const { return m_timeStep; }

	void activateRocket(float dt, Sphere* Rocket);

	void checkForCollision();

	void debugScene();

	static bool plane2Plane(PhysicsObject* obj1, PhysicsObject* obj2) { return false; }
	static bool plane2Sphere(PhysicsObject* obj1, PhysicsObject* obj2);
	static bool plane2Box(PhysicsObject* obj1, PhysicsObject* obj2);
	static bool sphere2Plane(PhysicsObject* obj1, PhysicsObject* obj2);
	static bool sphere2Sphere(PhysicsObject* obj1, PhysicsObject* obj2);
	static bool sphere2Box(PhysicsObject* obj1, PhysicsObject* obj2);
	static bool box2Plane(PhysicsObject* obj1, PhysicsObject* obj2);
	static bool box2Sphere(PhysicsObject* obj1, PhysicsObject* obj2);
	static bool box2Box(PhysicsObject* obj1, PhysicsObject* obj2);


protected:
	glm::vec2 m_gravity;
	float m_timeStep;

	std::vector<PhysicsObject*> m_actors;

	const int SHAPE_COUNT = 3;
};

