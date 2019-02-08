#include "Physics_and_FixedTimestepApp.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include <Gizmos.h>

Physics_and_FixedTimestepApp::Physics_and_FixedTimestepApp() {

}

Physics_and_FixedTimestepApp::~Physics_and_FixedTimestepApp() {

}

bool Physics_and_FixedTimestepApp::startup() {
	
	aie::Gizmos::create(255U, 255U, 65535U, 65535U);

	m_2dRenderer = new aie::Renderer2D();

	// TODO: remember to change this when redistributing a build!
	// the following path would be used instead: "./font/consolas.ttf"
	m_font = new aie::Font("../bin/font/consolas.ttf", 32);

	

	m_physicsScene = new PhysicsScene();
	m_physicsScene->setGravity(glm::vec2(0, 0));
	m_physicsScene->setTimeStep(0.01f);

	//m_ball1 = new Sphere(glm::vec2(-20, 0), glm::vec2(10, 0), 4.0f, 4, glm::vec4(1, 0, 0, 1));
	//m_ball2 = new Sphere(glm::vec2(20, 0), glm::vec2(-10, 0), 4.0f, 4, glm::vec4(1, 0, 0, 1));
	//
	//m_physicsScene->addActor(m_ball1);
	//m_physicsScene->addActor(m_ball2);

	m_ball1 = new Sphere(glm::vec2(0, -20), glm::vec2(0, 0), 20.0f, 5, glm::vec4(1, 0, 0, 1), 3);
	
	m_physicsScene->addActor(m_ball1);

	return true;
}

void Physics_and_FixedTimestepApp::shutdown() {

	delete m_font;
	delete m_2dRenderer;
}

void Physics_and_FixedTimestepApp::update(float deltaTime) {

	// input example
	aie::Input* input = aie::Input::getInstance();

	aie::Gizmos::clear();

	m_physicsScene->update(deltaTime);
	m_physicsScene->updateGizmos();

	if (input->isKeyDown(aie::INPUT_KEY_SPACE))
	{
		m_physicsScene->activateRocket(deltaTime, m_ball1);
	}


	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void Physics_and_FixedTimestepApp::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// begin drawing sprites
	m_2dRenderer->begin();

	// draw your stuff here!
	static float aspectRatio = 16 / 9.0f;
	aie::Gizmos::draw2D(glm::ortho<float>(-100, 100,
		-100 / aspectRatio, 100 / aspectRatio, -1.0f, 1.0f));

	// output some text, uses the last used colour
	//m_2dRenderer->drawText(m_font, "Press ESC to quit", 0, 0);

	// done drawing sprites
	m_2dRenderer->end();
}