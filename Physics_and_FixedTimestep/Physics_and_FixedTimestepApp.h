#pragma once

#include "Application.h"
#include "Renderer2D.h"
#include "PhysicsScene.h"
#include "Sphere.h"
#include "AABB.h"
#include "Plane.h"
#include "SAT.h"
#include <glm/ext.hpp>
#include <vector>

class Physics_and_FixedTimestepApp : public aie::Application {
public:

	Physics_and_FixedTimestepApp();
	virtual ~Physics_and_FixedTimestepApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

protected:

	aie::Renderer2D*	m_2dRenderer;
	aie::Font*			m_font;

	PhysicsScene* m_physicsScene;

	//float m_timer = 0;

	const int TOTAL_BALLS = 10;

	Sphere* m_ball1;
	//Sphere* m_ball2;
};