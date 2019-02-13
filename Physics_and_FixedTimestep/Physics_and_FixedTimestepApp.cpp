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

	// create objects here
	
	float angle = 0.707f;

	//Sphere* ball1 = new Sphere(glm::vec2(-50, 50), glm::vec2(angle * 50, angle * -50), 20, 5, glm::vec4(1, 0, 1, 1), 16);
	//Sphere* ball2 = new Sphere(glm::vec2(50, -50), glm::vec2(angle * -30, angle * 30), 20, 5, glm::vec4(1, 0, 1, 1), 16);

	Sphere* ball1 = new Sphere(glm::vec2(-20, 0), glm::vec2(11.11f, 0), 0.170f, 5, glm::vec4(1, 0, 1, 1), 16);
	Sphere* ball2 = new Sphere(glm::vec2(20, 0), glm::vec2(0, 0), 0.160f, 5, glm::vec4(1, 0, 1, 1), 16);

	AABB* box1 = new AABB(glm::vec2(50, 50), glm::vec2(angle * -10, angle * -10), 20, glm::vec2(5, 5), glm::vec4(1, 1, 0, 1));
	AABB* box2 = new AABB(glm::vec2(-50, -50), glm::vec2(angle * 10, angle * 10), 20, glm::vec2(5, 5), glm::vec4(1, 1, 0, 1));

	Plane* top = new Plane(glm::vec2(0, -1), 55, glm::vec4(1, 1, 1, 1));
	Plane* bottom = new Plane(glm::vec2(0, 1), 55, glm::vec4(1, 1, 1, 1));
	Plane* left = new Plane(glm::vec2(1, 0), 99, glm::vec4(1, 1, 1, 1));
	Plane* right = new Plane(glm::vec2(-1, 0), 99, glm::vec4(1, 1, 1, 1));
	

	//add physics objects here
	m_physicsScene->addActor(ball1);
	m_physicsScene->addActor(ball2);
	//m_physicsScene->addActor(box1);
	//m_physicsScene->addActor(box2);

	m_physicsScene->addActor(top);
	m_physicsScene->addActor(bottom);
	m_physicsScene->addActor(left);
	m_physicsScene->addActor(right);

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

	m_physicsScene->checkForCollision();


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