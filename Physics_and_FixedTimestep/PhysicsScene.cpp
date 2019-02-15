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

			Sphere* fuelBall = new Sphere(fuelPos, glm::vec2(0, 0), 0.1f, 1, glm::vec4(0, 1, 0, 1), 16, 0.3f, 0.3f, 0.8f);
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

		/*if (sphereToPlane < 0)
		{
			collisionNormal *= -1;
			sphereToPlane *= -1;
		}*/

		float intersection = sphere->getRadius() - sphereToPlane;
		if (intersection > 0)
		{
			//std::cout << "plane2Sphere Collision" << std::endl;

			//glm::vec2 direction(glm::normalize(sphere->getPosition() - ))

			sphere->setPosition(sphere->getPosition() + (collisionNormal * intersection));

			plane->resolveCollision(sphere, collisionNormal);

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
		glm::vec2 planeNorm = plane->getNormal();
		glm::vec2 vec1, vec2;
		if (planeNorm.x >= 0)
		{
			vec1.x = box->getMinPos().x;
			vec2.x = box->getMaxPos().x;
		}
		else
		{
			vec1.x = box->getMaxPos().x;
			vec2.x = box->getMinPos().x;
		}

		if (planeNorm.y >= 0)
		{
			vec1.y = box->getMinPos().y;
			vec2.y = box->getMaxPos().y;
		}
		else
		{
			vec1.y = box->getMaxPos().y;
			vec2.y = box->getMinPos().y;
		}

		float posSide = (planeNorm.x * vec2.x) + (planeNorm.y * vec2.y) + plane->getDistance();
		//float negSide = (planeNorm.x * vec1.x) + (planeNorm.y * vec1.y) + plane->getDistance();

		if (posSide > 0)
		{
			std::cout << "plane2Box Collision" << std::endl;



			return true;
		}
	}

	return false;
}

bool PhysicsScene::sphere2Plane(PhysicsObject * obj1, PhysicsObject * obj2)
{
	return plane2Sphere(obj2, obj1);

	//Sphere *sphere = dynamic_cast<Sphere*>(obj1);
	//Plane  *plane = dynamic_cast<Plane*>(obj2);
	//
	//if (sphere != nullptr && plane != nullptr)
	//{
	//	glm::vec2 collisionNormal = plane->getNormal();
	//	float sphereToPlane = glm::dot(sphere->getPosition(),
	//		plane->getNormal()) - plane->getDistance();
	//
	//	if (sphereToPlane < 0) 
	//	{
	//		collisionNormal *= -1;
	//		sphereToPlane *= -1;
	//	}
	//
	//	float intersection = sphere->getRadius() - sphereToPlane;
	//	if (intersection > 0)
	//	{
	//		//std::cout << "sphere2Plane Collision" << std::endl;
	//
	//		sphere->setPosition(sphere->getPosition() + (collisionNormal * intersection));
	//
	//		plane->resolveCollision(sphere, collisionNormal);
	//
	//		return true;
	//	}
	//}
	//
	//return false;
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
			//std::cout << "sphere2Sphere Collision" << std::endl;

			//float scalar1 = glm::distance(glm::vec2(0, 0), sphere1->getVelocity());
			//float scalar2 = glm::distance(glm::vec2(0, 0), sphere2->getVelocity());
			//
			//float percentageConv = scalar1 + scalar2;
			//
			//percentageConv = (100 / percentageConv);
			//
			//scalar1 *= percentageConv;
			//scalar1 *= 0.01f;
			//scalar2 *= percentageConv;
			//scalar2 *= 0.01f;
			//
			float res1 = sphere1->getMass() / (sphere1->getMass() + sphere2->getMass());
			float res2 = sphere2->getMass() / (sphere1->getMass() + sphere2->getMass());

			float posScalar = (minCollision - distance); // minCollision - distance should give the amount of overlap between the circles, this is halved in order to apply one half to each circle
			glm::vec2 direction = sphere1->getPosition() - sphere2->getPosition();

			direction = glm::normalize(direction);

			glm::vec2 newPos1 = sphere1->getPosition() + (direction * (posScalar * res1));
			glm::vec2 newPos2 = sphere2->getPosition() + (direction * (-posScalar * res2));

			float newDistance = glm::distance(newPos1, newPos2);

			sphere1->setPosition(newPos1);
			sphere2->setPosition(newPos2);

			sphere1->resolveCollision(sphere2, direction);
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
			//std::cout << "sphere2Box Collision" << std::endl;
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
		glm::vec2 planeNorm = plane->getNormal();
		glm::vec2 vec1, vec2;
		if (planeNorm.x >= 0)
		{
			vec1.x = box->getMinPos().x;
			vec2.x = box->getMaxPos().x;
		}
		else
		{
			vec1.x = box->getMaxPos().x;
			vec2.x = box->getMinPos().x;
		}
		
		if (planeNorm.y >= 0)
		{
			vec1.y = box->getMinPos().y;
			vec2.y = box->getMaxPos().y;
		}
		else
		{
			vec1.y = box->getMaxPos().y;
			vec2.y = box->getMinPos().y;
		}
		
		float posSide = (planeNorm.x * vec2.x) + (planeNorm.y * vec2.y) + plane->getDistance();
		//float negSide = (planeNorm.x * vec1.x) + (planeNorm.y * vec1.y) + plane->getDistance();

		if (posSide > 0)
		{
			std::cout << "box2Plane Collision" << std::endl;



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
			//std::cout << "sphere2Box Collision" << std::endl;




			//box->resolveCollision(sphere, );

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
			//std::cout << "box2Box Collision" << std::endl;

			const glm::vec2 faces[4] =
			{
				glm::vec2(-1, 0), glm::vec2(1, 0),
				glm::vec2(0, -1), glm::vec2(0, 1)
			};

			float distance[4] =
			{
				(box2->getMaxPos().x - box1->getMinPos().x),
				(box1->getMaxPos().x - box2->getMinPos().x),
				(box2->getMaxPos().y - box1->getMinPos().y),
				(box1->getMaxPos().y - box2->getMinPos().y),
			};

			float min = INFINITY;
			int faceCol = 0;
			for (int i = 0; i < 4; i++)
			{
				if (distance[i] < min)
				{
					min = distance[i];
					faceCol = i;
				}
			}

			box1->setPosition(box1->getPosition() + (faces[faceCol] * -min));

			box1->resolveCollision(box2, faces[faceCol]);

			return true;
		}
	}

	return false;
}

