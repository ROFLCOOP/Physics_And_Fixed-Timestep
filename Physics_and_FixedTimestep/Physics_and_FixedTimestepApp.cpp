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
	srand(time(NULL));


	aie::Gizmos::create(255U, 255U, 65535U, 65535U);

	m_2dRenderer = new aie::Renderer2D();

	// TODO: remember to change this when redistributing a build!
	// the following path would be used instead: "./font/consolas.ttf"
	m_font = new aie::Font("../bin/font/consolas.ttf", 32);

	
	m_physicsScene = new PhysicsScene();
	m_physicsScene->setGravity(glm::vec2(0, -100));
	m_physicsScene->setTimeStep(0.0001f);

	// create objects here
	
	float angle = 0.707f;
	const int ballCount = 10;

	Sphere* Buallz[ballCount];

	for (int i = 0; i < ballCount; i++)
	{
		Buallz[i] = new Sphere(glm::vec2((rand() % 180) - 90, (rand() % 140) - 70), glm::vec2((rand() % 500) - 250, (rand() % 500) - 250), 50, 5, glm::vec4((rand() % 100) * 0.05f, (rand() % 100) * 0.05f, (rand() % 100) * 0.05f, 1), 32, 0.3f, 0.3f, 0.8f);
	}


	
	Sphere* ball1 = new Sphere(glm::vec2(20, 0), glm::vec2(0, 0), 50, 5, glm::vec4(1, 0, 1, 1), 16, 0.3f, 0.3f, 0.8f);
	ball1->setKinematic(true);
	Sphere* ball2 = new Sphere(glm::vec2(20, 0), glm::vec2(-50, 0), 50, 5, glm::vec4(1, 0, 1, 0.5f), 16, 0.3f, 0.3f, 0.8f);

	AABB* box1 = new AABB(glm::vec2(50, 0), glm::vec2(-10, 0), 50, glm::vec2(5, 5), glm::vec4(1, 1, 0, 1), 0.3f, 0.3f, 0.8f);
	//box1->setKinematic(true);
	AABB* box2 = new AABB(glm::vec2(-50, 0), glm::vec2(50, 0), 50, glm::vec2(5, 5), glm::vec4(1, 1, 0, 1), 0.3f, 0.3f, 0.8f);

	Plane* top = new Plane(glm::vec2(0, 1), -55, glm::vec4(1, 1, 1, 1));
	Plane* bottom = new Plane(glm::vec2(0, -1), -55, glm::vec4(1, 1, 1, 1));
	Plane* left = new Plane(glm::vec2(-1, 0), -99, glm::vec4(1, 1, 1, 1));
	Plane* right = new Plane(glm::vec2(1, 0), -99, glm::vec4(1, 1, 1, 1));

	glm::vec2 diagAngle = glm::normalize(glm::vec2(55, 100));

	Plane* diag = new Plane(diagAngle, -50, glm::vec4(1, 1, 1, 1));

	//Plane* midSplit = new Plane(glm::vec2(1, 0), 0, glm::vec4(1, 1, 1, 1));

	//std::vector<glm::vec2> verts;
	//verts.push_back(glm::vec2(-10, 10));
	//verts.push_back(glm::vec2(0, 15));
	//verts.push_back(glm::vec2(8, 13));
	//verts.push_back(glm::vec2(12, 5));
	//verts.push_back(glm::vec2(8, -6));
	//verts.push_back(glm::vec2(-8, -4));

	SAT* sat1 = new SAT(glm::vec2(-20, 0), glm::vec2(10, 0), 3, 10, 0, 50, 0, 0, 0.8f, glm::vec4(1,1,1,1));
	SAT* sat2 = new SAT(glm::vec2(20, 0), glm::vec2(-10, 0), 4, 10, 45, 50, 0, 0, 0.8f, glm::vec4(1, 1, 1, 1));

	//add physics objects here
	m_physicsScene->addActor(sat1);
	//m_physicsScene->addActor(sat2);

	//for (int i = 0; i < ballCount; i++)
	//{
	//	m_physicsScene->addActor(Buallz[i]);
	//}
	//
	m_physicsScene->addActor(ball1);
	//m_physicsScene->addActor(ball2);

	//m_physicsScene->addActor(box1);
	//m_physicsScene->addActor(box2);

	m_physicsScene->addActor(top);
	m_physicsScene->addActor(bottom);
	m_physicsScene->addActor(left);
	m_physicsScene->addActor(right);
	m_physicsScene->addActor(diag);
	//m_physicsScene->addActor(midSplit);


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

	if (input->wasMouseButtonPressed(0))
	{
		m_physicsScene->attachShapeToMouse(getMousePos(input));
	}
	else if (input->isMouseButtonDown(0))
	{
		m_physicsScene->dragObject(getMousePos(input));
	}
	else if (input->wasMouseButtonReleased(0))
	{
		m_physicsScene->detachObject();
	}

	m_physicsScene->update(deltaTime);
	m_physicsScene->updateGizmos();

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

glm::vec2 Physics_and_FixedTimestepApp::getMousePos(aie::Input* input)
{
	glm::vec2 mousePos(input->getMouseX(), input->getMouseY());
	float percX = 200.0f / getWindowWidth();
	mousePos.x = (mousePos.x * percX) - 100;
	float percY = 110.0f / getWindowHeight();
	mousePos.y = (mousePos.y * percY) - 55;
	return mousePos;
}
