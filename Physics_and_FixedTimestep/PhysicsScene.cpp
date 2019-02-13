#include "PhysicsScene.h"
#include <list>
#include <iostream>

typedef bool(*fn)(PhysicsObject*, PhysicsObject*);

static fn collisionFunctionArray[] =
{
	PhysicsScene::plane2Plane,	PhysicsScene::plane2Sphere,		PhysicsScene::plane2Box,
	PhysicsScene::sphere2Plane,	PhysicsScene::sphere2Sphere,	PhysicsScene::sphere2Box,	
	PhysicsScene::box2Plane,	PhysicsScene::box2Sphere,		PhysicsScene::box2Box
};


PhysicsScene::PhysicsScene() : m_timeStep(0.01f), m_gravity(glm::vec2(0, 0))
{
}


PhysicsScene::~PhysicsScene()
{
	for (auto pActor : m_actors)
	{
		delete pActor;
	}
}

void PhysicsScene::addActor(PhysicsObject * actor)
{
	m_actors.push_back(actor);
}

void PhysicsScene::removeActor(PhysicsObject * actor)
{
	for (auto it = m_actors.begin(); it != m_actors.end(); it++)
	{
		if (*it == actor)
		{
			delete *it;
			m_actors.erase(it);
			return;
		}
	}

}

void PhysicsScene::update(float dt)
{
	static std::list<PhysicsObject*> dirty;

	static float accumulatedTime = 0.0f;
	accumulatedTime += dt;

	while (accumulatedTime >= m_timeStep)
	{
		for (auto pActor : m_actors)
		{
			pActor->fixedUpdate(m_gravity, m_timeStep);
		}

		accumulatedTime -= m_timeStep;

		//for (auto pActor : m_actors)
		//{
		//	for (auto pOther : m_actors)
		//	{
		//		if (pActor == pOther)
		//			continue;
		//		if (std::find(dirty.begin(), dirty.end(), pActor) != dirty.end() &&
		//			std::find(dirty.begin(), dirty.end(), pOther) != dirty.end())
		//			continue;
		//
		//		RigidBody* pRigid = dynamic_cast<RigidBody*>(pActor);
		//		if (pRigid->checkCollision(pOther) == true)
		//		{
		//			pRigid->applyForceToActor(dynamic_cast<RigidBody*>(pOther),
		//				pRigid->getVelocity() * pRigid->getMass());
		//			dirty.push_back(pRigid);
		//			dirty.push_back(pOther);
		//		}
		//	}
		//}
		//dirty.clear();

		
		
	}

}

void PhysicsScene::updateGizmos()
{
	for (auto pActor : m_actors)
	{
		pActor->makeGizmo();
	}
}

void PhysicsScene::activateRocket(float dt, Sphere* rocket)
{
	static float accumulatedTime = 0.0f;
	accumulatedTime += dt;

	while (accumulatedTime >= m_timeStep)
	{
		if (rocket->getMass() > 0.1f)
		{
			rocket->changeMass(-0.1f);
			glm::vec2 fuelPos = rocket->getPosition();

			fuelPos -= glm::vec2(0, rocket->getRadius());

			Sphere* fuelBall = new Sphere(fuelPos, glm::vec2(0, 0), 0.1f, 1, glm::vec4(0, 1, 0, 1), 16);
			addActor(fuelBall);
			rocket->applyForceToActor(fuelBall, glm::vec2(0, -5));
		}
		accumulatedTime -= m_timeStep;
	}
}

void PhysicsScene::checkForCollision()
{
	
	int actorCount = m_actors.size();

	for (int outer = 0; outer < actorCount; outer++)
	{
		for (int inner = outer + 1; inner < actorCount; inner++)
		{
			PhysicsObject* object1 = m_actors[outer];
			PhysicsObject* object2 = m_actors[inner];
			int shapeID1 = object1->getShapeID();
			int shapeID2 = object2->getShapeID();

			//the following determines what collision function should be used based on what shapes are being checked
			int functionIdx = (shapeID1 * SHAPE_COUNT) + shapeID2;
			fn collisionFunctionPtr = collisionFunctionArray[functionIdx];
			if (collisionFunctionPtr != nullptr)
			{
				collisionFunctionPtr(object1, object2);
			}
		}
	}
}

void PhysicsScene::debugScene()
{
	int count = 0;
	for (auto pActor : m_actors)
	{
		std::cout << count << " : ";
		pActor->debug();
		count++;
	}
}

bool PhysicsScene::plane2Sphere(PhysicsObject * obj1, PhysicsObject * obj2)
{
	Plane *plane = dynamic_cast<Plane*>(obj1);
	Sphere *sphere = dynamic_cast<Sphere*>(obj2);

	if (plane != nullptr && sphere != nullptr)
	{
		glm::vec2 collisionNormal = plane->getNormal();
		float sphereToPlane = glm::dot(
			sphere->getPosition(),
			plane->getNormal()) - plane->getDistance();

		if (sphereToPlane < 0)
		{
			collisionNormal *= -1;
			sphereToPlane *= -1;
		}

		float intersection = sphere->getRadius() - sphereToPlane;
		if (intersection > 0)
		{
			std::cout << "plane2Sphere Collision" << std::endl;
			return true;
		}
	}

	return false;
}

bool PhysicsScene::plane2Box(PhysicsObject * obj1, PhysicsObject * obj2)
{
	Plane *plane = dynamic_cast<Plane*>(obj1);
	AABB  *box = dynamic_cast<AABB*>(obj2);

	if (plane != nullptr && box != nullptr)
	{
		glm::vec2 bottomLeft = box->getMinPos();
		glm::vec2 topRight = box->getMaxPos();
		glm::vec2 bottomRight = glm::vec2(box->getMaxPos().x, box->getMinPos().y);
		glm::vec2 topLeft = glm::vec2(box->getMinPos().x, box->getMaxPos().y);

		float bottomLeftDis = glm::dot(bottomLeft, plane->getNormal()) - plane->getDistance();
		float topRightDis = glm::dot(topRight, plane->getNormal()) - plane->getDistance();
		float bottomRightDis = glm::dot(bottomRight, plane->getNormal()) - plane->getDistance();
		float topLeftDis = glm::dot(topLeft, plane->getNormal()) - plane->getDistance();

		//the following results get inverted with the intention of using && instead of ||
		if (bottomLeftDis > 0 || topRightDis > 0
			|| bottomRightDis > 0 || topLeftDis > 0)
		{
			std::cout << "plane2Box Collision" << std::endl;
			return true;
		}
	}

	return false;
}

bool PhysicsScene::sphere2Plane(PhysicsObject * obj1, PhysicsObject * obj2)
{
	Sphere *sphere = dynamic_cast<Sphere*>(obj1);
	Plane  *plane = dynamic_cast<Plane*>(obj2);

	if (sphere != nullptr && plane != nullptr)
	{
		glm::vec2 collisionNormal = plane->getNormal();
		float sphereToPlane = glm::dot(
			sphere->getPosition(),
			plane->getNormal()) - plane->getDistance();

		if (sphereToPlane < 0) 
		{
			collisionNormal *= -1;
			sphereToPlane *= -1;
		}

		float intersection = sphere->getRadius() - sphereToPlane;
		if (intersection > 0)
		{
			std::cout << "sphere2Plane Collision" << std::endl;
			return true;
		}
	}

	return false;
}

bool PhysicsScene::sphere2Sphere(PhysicsObject * obj1, PhysicsObject * obj2)
{
	Sphere *sphere1 = dynamic_cast<Sphere*>(obj1);
	Sphere *sphere2 = dynamic_cast<Sphere*>(obj2);

	if (sphere1 != nullptr && sphere2 != nullptr)
	{
		//get the minimum distance the circles need to be for a collision to occur
		float minCollision = sphere1->getRadius() + sphere2->getRadius();

		//get the actual distance between the circles
		float distance = glm::distance(sphere1->getPosition(), sphere2->getPosition());

		//compare the distance and the minimum collision distance
		if (distance < minCollision)
		{
			std::cout << "sphere2Sphere Collision" << std::endl;
			return true;
		}
	}

	return false;
}

bool PhysicsScene::sphere2Box(PhysicsObject * obj1, PhysicsObject * obj2)
{
	Sphere *sphere = dynamic_cast<Sphere*>(obj1);
	AABB *box = dynamic_cast<AABB*>(obj2);

	if (sphere != nullptr && box != nullptr)
	{
		glm::vec2 clampPoint = glm::clamp(sphere->getPosition(), box->getMinPos(), box->getMaxPos());
		float distance = glm::distance(sphere->getPosition(), clampPoint);

		if (distance < sphere->getRadius())
		{
			std::cout << "sphere2Box Collision" << std::endl;
			return true;
		}

	}

	return false;
}

bool PhysicsScene::box2Plane(PhysicsObject * obj1, PhysicsObject * obj2)
{
	AABB *box = dynamic_cast<AABB*>(obj1);
	Plane *plane = dynamic_cast<Plane*>(obj2);

	if (box != nullptr && plane != nullptr)
	{
		glm::vec2 bottomLeft = box->getMinPos();
		glm::vec2 topRight = box->getMaxPos();
		glm::vec2 bottomRight = glm::vec2(box->getMaxPos().x, box->getMinPos().y);
		glm::vec2 topLeft = glm::vec2(box->getMinPos().x, box->getMaxPos().y);

		float bottomLeftDis = glm::dot(bottomLeft, plane->getNormal()) - plane->getDistance();
		float topRightDis = glm::dot(topRight, plane->getNormal()) - plane->getDistance();
		float bottomRightDis = glm::dot(bottomRight, plane->getNormal()) - plane->getDistance();
		float topLeftDis = glm::dot(topLeft, plane->getNormal()) - plane->getDistance();

		//the following results get inverted with the intention of using && instead of ||
		if (bottomLeftDis > 0 || topRightDis > 0
			|| bottomRightDis > 0 || topLeftDis > 0)
		{
			std::cout << "plane2Box Collision" << std::endl;
			return true;
		}
	}

	return false;
}

bool PhysicsScene::box2Sphere(PhysicsObject * obj1, PhysicsObject * obj2)
{
	AABB *box = dynamic_cast<AABB*>(obj1);
	Sphere *sphere = dynamic_cast<Sphere*>(obj2);

	if (box != nullptr && sphere != nullptr)
	{
		glm::vec2 clampPoint = glm::clamp(sphere->getPosition(), box->getMinPos(), box->getMaxPos());
		float distance = glm::distance(sphere->getPosition(), clampPoint);

		if (distance < sphere->getRadius())
		{
			std::cout << "sphere2Box Collision" << std::endl;
			return true;
		}

	}

	return false;
}

bool PhysicsScene::box2Box(PhysicsObject * obj1, PhysicsObject * obj2)
{
	AABB *box1 = dynamic_cast<AABB*>(obj1);
	AABB *box2 = dynamic_cast<AABB*>(obj2);

	if (box1 != nullptr && box2 != nullptr)
	{
		if (box1->getMinPos().x < box2->getMaxPos().x
			&& box1->getMaxPos().x > box2->getMinPos().x
			&& box1->getMinPos().y < box2->getMaxPos().y
			&& box1->getMaxPos().y > box2->getMinPos().y)
		{
			std::cout << "box2Box Collision" << std::endl;
			return true;
		}
	}

	return false;
}

