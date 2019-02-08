#pragma once

#include "Application.h"
#include "Renderer2D.h"
#include "PhysicsScene.h"
#include "Sphere.h"
#include <glm/ext.hpp>


class Projectile_SimulationApp : public aie::Application {
public:

	Projectile_SimulationApp();
	virtual ~Projectile_SimulationApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

	void setupContinuousDemo(glm::vec2 startPos, float inclination,
							float speed, float gravity);

protected:


	aie::Renderer2D*	m_2dRenderer;
	aie::Font*			m_font;

	PhysicsScene* m_physicsScene;
};