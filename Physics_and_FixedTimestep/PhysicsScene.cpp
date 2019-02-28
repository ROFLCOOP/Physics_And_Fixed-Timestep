#include "PhysicsScene.h"
#include <list>
#include <iostream>
#include <Gizmos.h>

typedef bool(*fn)(PhysicsObject*, PhysicsObject*);

static fn collisionFunctionArray[] =
{
	PhysicsScene::plane2Plane,	PhysicsScene::plane2Sphere,		PhysicsScene::plane2Box,	PhysicsScene::plane2Sat,
	PhysicsScene::sphere2Plane,	PhysicsScene::sphere2Sphere,	PhysicsScene::sphere2Box,	PhysicsScene::sphere2Sat,
	PhysicsScene::box2Plane,	PhysicsScene::box2Sphere,		PhysicsScene::box2Box,		PhysicsScene::box2Sat,
	PhysicsScene::sat2Plane,	PhysicsScene::sat2Sphere,		PhysicsScene::sat2Box,		PhysicsScene::sat2Sat
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

void PhysicsScene::attachShapeToMouse(glm::vec2 mousePos)
{
	for (PhysicsObject* actor : m_actors)
	{
		if (actor->getShapeID() == SPHERE)
		{
			Sphere* sphere = dynamic_cast<Sphere*>(actor);
			assert(sphere != nullptr);
			if (glm::distance(mousePos, sphere->getPosition()) < sphere->getRadius())
			{
				m_attachedObject = sphere;
				m_attachedIsKinematic = sphere->isKinematic();
			}
		}
		else if (actor->getShapeID() == BOX)
		{
			AABB* box = dynamic_cast<AABB*>(actor);
			assert(box != nullptr);
			if (mousePos.x < box->getMaxPos().x && mousePos.y < box->getMaxPos().y
				&& mousePos.x > box->getMinPos().x && mousePos.y > box->getMinPos().y)
			{
				m_attachedObject = box;
				m_attachedIsKinematic = box->isKinematic();
			}
		}
	}
}

void PhysicsScene::dragObject(glm::vec2 mousePos)
{
	if (m_attachedObject != nullptr)
	{
		RigidBody* object = dynamic_cast<RigidBody*>(m_attachedObject);
		assert(object != nullptr);

		if (!m_attachedIsKinematic)
		{
			aie::Gizmos::add2DLine(object->getPosition(), mousePos, glm::vec4(1, 1, 1, 1));
			object->applyForce((mousePos - object->getPosition()) * object->getMass());
		}
		else
		{
			object->setPosition(mousePos);
		}
	}
}

void PhysicsScene::detachObject()
{
	RigidBody* object = dynamic_cast<RigidBody*>(m_attachedObject);
	if (object != nullptr)
	{
		if (m_attachedIsKinematic)
		{
			object->setKinematic(m_attachedIsKinematic);
			object->setVelocity(glm::vec2(0, 0));
		}
	}
	m_attachedObject = nullptr;
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

	return box2Plane(obj2, obj1);

	//Plane *plane = dynamic_cast<Plane*>(obj1);
	//AABB  *box = dynamic_cast<AABB*>(obj2);
	//
	//if (plane != nullptr && box != nullptr)
	//{
	//	glm::vec2 planeNorm = plane->getNormal();
	//	glm::vec2 vec1, vec2;
	//	if (planeNorm.x >= 0)
	//	{
	//		vec1.x = box->getMinPos().x;
	//		vec2.x = box->getMaxPos().x;
	//	}
	//	else
	//	{
	//		vec1.x = box->getMaxPos().x;
	//		vec2.x = box->getMinPos().x;
	//	}
	//
	//	if (planeNorm.y >= 0)
	//	{
	//		vec1.y = box->getMinPos().y;
	//		vec2.y = box->getMaxPos().y;
	//	}
	//	else
	//	{
	//		vec1.y = box->getMaxPos().y;
	//		vec2.y = box->getMinPos().y;
	//	}
	//
	//	float posSide = (planeNorm.x * vec2.x) + (planeNorm.y * vec2.y) + plane->getDistance();
	//	//float negSide = (planeNorm.x * vec1.x) + (planeNorm.y * vec1.y) + plane->getDistance();
	//
	//	if (posSide > 0)
	//	{
	//		std::cout << "plane2Box Collision" << std::endl;
	//
	//
	//
	//		return true;
	//	}
	//}
	//
	//return false;
}

bool PhysicsScene::plane2Sat(PhysicsObject * obj1, PhysicsObject * obj2)
{
	return sat2Plane(obj2, obj1);
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
			float res1 = sphere1->getMass() / (sphere1->getMass() + sphere2->getMass());
			float res2 = sphere2->getMass() / (sphere1->getMass() + sphere2->getMass());

			float posScalar = (minCollision - distance); // minCollision - distance should give the amount of overlap between the circles, this is halved in order to apply one half to each circle
			glm::vec2 direction = sphere1->getPosition() - sphere2->getPosition();

			direction = glm::normalize(direction);

			if (!sphere1->isKinematic() && !sphere2->isKinematic())
			{
				sphere1->setPosition(sphere1->getPosition() + (direction * (posScalar * res1)));
				sphere2->setPosition(sphere2->getPosition() + (direction * (-posScalar * res2)));
			}
			else if (!sphere1->isKinematic())
			{
				sphere1->setPosition(sphere1->getPosition() + direction * posScalar);
			}
			else
			{
				sphere2->setPosition(sphere2->getPosition() + direction * posScalar);
			}

			
			

			sphere1->resolveCollision(sphere2, direction);
			return true;
		}
	}

	return false;
}

bool PhysicsScene::sphere2Box(PhysicsObject * obj1, PhysicsObject * obj2)
{
	return box2Sphere(obj2, obj1);
	//Sphere *sphere = dynamic_cast<Sphere*>(obj1);
	//AABB *box = dynamic_cast<AABB*>(obj2);
	//
	//if (sphere != nullptr && box != nullptr)
	//{
	//	glm::vec2 clampPoint = glm::clamp(sphere->getPosition(), box->getMinPos(), box->getMaxPos());
	//	float distance = glm::distance(sphere->getPosition(), clampPoint);
	//
	//	if (distance < sphere->getRadius())
	//	{
	//		//std::cout << "sphere2Box Collision" << std::endl;
	//		return true;
	//	}
	//
	//}
	//
	//return false;
}

bool PhysicsScene::sphere2Sat(PhysicsObject * obj1, PhysicsObject * obj2)
{
	return sat2Sphere(obj2, obj1);
}

bool PhysicsScene::box2Plane(PhysicsObject * obj1, PhysicsObject * obj2)
{
	AABB *box = dynamic_cast<AABB*>(obj1);
	Plane *plane = dynamic_cast<Plane*>(obj2);

	if (box != nullptr && plane != nullptr)
	{
		//glm::vec2 planeNorm = -plane->getNormal();
		//glm::vec2 vec1, vec2;
		//if (planeNorm.x >= 0)
		//{
		//	vec2.x = box->getMaxPos().x;
		//}
		//else
		//{
		//	vec2.x = box->getMinPos().x;
		//}
		//
		//if (planeNorm.y >= 0)
		//{
		//	vec2.y = box->getMaxPos().y;
		//}
		//else
		//{
		//	vec2.y = box->getMinPos().y;
		//}
		//
		//float posSide = (planeNorm.x * vec2.x) + (planeNorm.y * vec2.y) + plane->getDistance();
		//
		//if (posSide > 0)
		//{
		std::vector<glm::vec2> verts =
		{
			glm::vec2(box->getMinPos().x, box->getMaxPos().y), box->getMaxPos(),
			glm::vec2(box->getMaxPos().x, box->getMinPos().y), box->getMinPos()
		};
		
		float minDist = INFINITY;
		for (glm::vec2 vert : verts)
		{
			float distance = glm::dot(vert, plane->getNormal()) - plane->getDistance();
			if (distance < minDist)
			{
				minDist = distance;
			}
		}

		if(minDist <= 0)
		{
			box->setPosition(box->getPosition() - (plane->getNormal() * minDist));

			plane->resolveCollision(box, plane->getNormal());

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
			float boxRes = box->getMass() / (box->getMass() + sphere->getMass());
			float sphereRes = sphere->getMass() / (box->getMass() + sphere->getMass());

			float overlap = sphere->getRadius() - distance;

			glm::vec2 colNorm;

			if (sphere->getPosition() == clampPoint)
			{
				//std::cout << "colNorm Error" << std::endl; // the sphere position is on the clamp point, the colnorm will not work
				if (sphere->getVelocity() != glm::vec2(0, 0))
					colNorm = -glm::normalize(sphere->getVelocity());
				else
					colNorm = -glm::normalize(box->getVelocity());
			}
			else
				colNorm = glm::normalize(sphere->getPosition() - clampPoint);

			if (!box->isKinematic() && !sphere->isKinematic())
			{
				box->setPosition(box->getPosition() + (-overlap * colNorm) * boxRes);
				sphere->setPosition(sphere->getPosition() + (overlap * colNorm) * sphereRes);
			}
			else if (!box->isKinematic())
			{
				box->setPosition(box->getPosition() - (overlap * colNorm));
			}
			else
			{
				sphere->setPosition(sphere->getPosition() + (overlap * colNorm));
			}

			box->resolveCollision(sphere, colNorm);

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

			float res1 = box1->getMass() / (box1->getMass() + box2->getMass());
			float res2 = box2->getMass() / (box1->getMass() + box2->getMass());

			if (!box1->isKinematic() && !box2->isKinematic())
			{
				box1->setPosition(box1->getPosition() - (faces[faceCol] * min) * res1);
				box2->setPosition(box2->getPosition() + (faces[faceCol] * min) * res2);
			}
			else if (!box1->isKinematic())
			{
				box1->setPosition(box1->getPosition() + (faces[faceCol] * min));
			}
			else
			{
				box2->setPosition(box2->getPosition() + (faces[faceCol] * min));
			}
			box1->resolveCollision(box2, faces[faceCol]);

			return true;
		}
	}
	return false;
}

bool PhysicsScene::box2Sat(PhysicsObject * obj1, PhysicsObject * obj2)
{
	return sat2Box(obj2, obj1);
}

bool PhysicsScene::sat2Plane(PhysicsObject * obj1, PhysicsObject * obj2)
{
	SAT* sat = dynamic_cast<SAT*>(obj1);
	Plane* plane = dynamic_cast<Plane*>(obj2);
	if (sat != nullptr && plane != nullptr)
	{
		std::vector<glm::vec2> verts = sat->getVerts();
		float minDist = INFINITY;

		for (glm::vec2 vert : verts)
		{
			float distance = glm::dot(vert, plane->getNormal()) - plane->getDistance();
			if (distance < minDist)
			{
				minDist = distance;
			}
		}

		if (minDist <= 0)
		{
			std::cout << "sat2Plane Collision" << std::endl;

			sat->setPosition(sat->getPosition() - plane->getNormal() * minDist);

			plane->resolveCollision(sat, plane->getNormal());

			return true;
		}
	}
	return false;
}

bool PhysicsScene::sat2Sphere(PhysicsObject * obj1, PhysicsObject * obj2)
{
	SAT* sat = dynamic_cast<SAT*>(obj1);
	Sphere* sphere = dynamic_cast<Sphere*>(obj2);
	if (sat != nullptr && sphere != nullptr)
	{
		float overlap = INFINITY;
		glm::vec2 smallest;
		std::vector<glm::vec2> axes;

		for (auto vec : sat->getEdges())
		{
			glm::vec2 perp(vec.y, -vec.x);
			axes.push_back(glm::normalize(perp));
		}

		for (int i = 0; i < axes.size(); i++)
		{
			glm::vec2 axis = axes[i];

			glm::vec2 p1 = sat->project(axis);

			float minSphere = glm::dot(sphere->getPosition() + (-axis * sphere->getRadius()), axis); // get the "min and max" points of a circle on the axis
			float maxSphere = glm::dot(sphere->getPosition() + (axis * sphere->getRadius()), axis);
			glm::vec2 p2 = glm::vec2(minSphere, maxSphere);
			

			if (p1.x < p2.y && p2.x > p1.y)
			{
				return false;
			}
			else
			{
				float o;
				if (p1.y - p2.x < p2.y - p1.x)
					o = p1.y - p2.x;
				else
					o = p2.y - p1.x;

				if (o < overlap)
				{
					overlap = o;
					smallest = axis;
				}
			}
		}
		std::cout << "sat2Sphere collision" << std::endl;

		float res1 = sat->getMass() / (sat->getMass() + sphere->getMass());
		float res2 = sphere->getMass() / (sat->getMass() + sphere->getMass());


		if (!sat->isKinematic() && !sphere->isKinematic())
		{
			sat->setPosition(sat->getPosition() - smallest * (overlap * res1));
			sphere->setPosition(sphere->getPosition() + smallest * (overlap * res2));
		}
		else if (!sat->isKinematic())
		{
			sat->setPosition(sat->getPosition() - smallest * overlap);
		}
		else
		{
			sphere->setPosition(sphere->getPosition() + smallest * overlap);
		}

		sat->resolveCollision(sphere, -smallest);

		return true;
	}
	return false;
}

bool PhysicsScene::sat2Box(PhysicsObject * obj1, PhysicsObject * obj2)
{
	SAT* sat = dynamic_cast<SAT*>(obj1);
	AABB* box = dynamic_cast<AABB*>(obj2);
	if (sat != nullptr && box != nullptr)
	{
		float overlap = INFINITY;
		glm::vec2 smallest;
		std::vector<glm::vec2> axes2 = { glm::vec2(0, 1), glm::vec2(1, 0) };
		std::vector<glm::vec2> axes1;
		std::vector<glm::vec2> boxPoints = { box->getMinPos(), glm::vec2(box->getMinPos().x, box->getMaxPos().y),
											box->getMaxPos(), glm::vec2(box->getMaxPos().x, box->getMinPos().y) };

		for (auto vec : sat->getEdges())
		{
			glm::vec2 perp(vec.y, -vec.x);
			axes1.push_back(glm::normalize(perp));
		}
		for (int i = 0; i < axes1.size(); i++)
		{
			glm::vec2 axis = axes1[i];

			glm::vec2 p1 = sat->project(axis);
			
			float minVert = glm::dot(axis, boxPoints[0]);
			float maxVert = minVert;
			for (int i = 1; i < boxPoints.size(); i++)
			{
				float vert = glm::dot(axis, boxPoints[i]);
				if (vert < minVert)
					minVert = vert;
				else if (vert > maxVert)
					maxVert = vert;
			}
			glm::vec2 p2(minVert, maxVert);

			if (p1.x < p2.y && p2.x > p1.y)
			{
				return false;
			}
			else
			{
				float o;
				if (p1.y - p2.x < p2.y - p1.x)
					o = p1.y - p2.x;
				else
					o = p2.y - p1.x;

				if (o < overlap)
				{
					overlap = o;
					smallest = axis;
				}
			}
		}

		for (int i = 0; i < axes2.size(); i++)
		{
			glm::vec2 axis = axes2[i];

			glm::vec2 p1 = sat->project(axis);

			float minVert = glm::dot(axis, boxPoints[0]);
			float maxVert = minVert;
			for (int i = 1; i < boxPoints.size(); i++)
			{
				float vert = glm::dot(axis, boxPoints[i]);
				if (vert < minVert)
					minVert = vert;
				else if (vert > maxVert)
					maxVert = vert;
			}
			glm::vec2 p2(minVert, maxVert);

			if (p1.x < p2.y && p2.x > p1.y)
			{
				return false;
			}
			else
			{
				float o;
				if (p1.y - p2.x < p2.y - p1.x)
					o = p1.y - p2.x;
				else
					o = p2.y - p1.x;

				if (o < overlap)
				{
					overlap = o;
					smallest = axis;
				}
			}
		}
		std::cout << "sat2Box collision" << std::endl;

		float res1 = sat->getMass() / (sat->getMass() + box->getMass());
		float res2 = box->getMass() / (sat->getMass() + box->getMass());


		if (!sat->isKinematic() && !box->isKinematic())
		{
			sat->setPosition(sat->getPosition() - smallest * (overlap * res1));
			box->setPosition(box->getPosition() + smallest * (overlap * res2));
		}
		else if (!sat->isKinematic())
		{
			sat->setPosition(sat->getPosition() - smallest * overlap);
		}
		else
		{
			box->setPosition(box->getPosition() + smallest * overlap);
		}

		sat->resolveCollision(box, -smallest);

		return true;
	}
	return false;
}

bool PhysicsScene::sat2Sat(PhysicsObject * obj1, PhysicsObject * obj2)
{
	SAT* sat1 = dynamic_cast<SAT*>(obj1);
	SAT* sat2 = dynamic_cast<SAT*>(obj2);
	if (sat1 != nullptr && sat2 != nullptr)
	{
		float overlap = INFINITY;
		glm::vec2 smallest;
		std::vector<glm::vec2> axes1;
		std::vector<glm::vec2> axes2;
		//std::vector<glm::vec2> edges1 = sat1->getEdges();
		
		for (auto vec : sat1->getEdges())
		{
			glm::vec2 perp(vec.y, -vec.x);
			axes1.push_back(glm::normalize(perp));
		}
		for (auto vec : sat2->getEdges())
		{
			glm::vec2 perp(vec.y, -vec.x);
			axes2.push_back(glm::normalize(perp));
		}

		for (int i = 0; i < axes1.size(); i++)
		{
			glm::vec2 axis = axes1[i];

			glm::vec2 p1 = sat1->project(axis);
			glm::vec2 p2 = sat2->project(axis);

			if (p1.x < p2.y && p2.x > p1.y)
			{
				return false;
			}
			else
			{
				float o;
				if (p1.y - p2.x < p2.y - p1.x)
					o = p1.y - p2.x;
				else
					o = p2.y - p1.x;

				if (o < overlap)
				{
					overlap = o;
					smallest = axis;
				}
			}
		}

		for (int i = 0; i < axes2.size(); i++)
		{
			glm::vec2 axis = axes2[i];

			glm::vec2 p1 = sat1->project(axis);
			glm::vec2 p2 = sat2->project(axis);

			if (p1.x < p2.y && p2.x > p1.y)
			{
				return false;
			}
			else
			{
				float o;
				if (p1.y - p2.x < p2.y - p1.x)
					o = p1.y - p2.x;
				else
					o = p2.y - p1.x;

				if (o < overlap)
				{
					overlap = o;
					smallest = axis;
				}
			}
		}
		std::cout << "sat2Sat collision" << std::endl;

		float res1 = sat1->getMass() / (sat1->getMass() + sat2->getMass());
		float res2 = sat2->getMass() / (sat1->getMass() + sat2->getMass());

		if (!sat1->isKinematic() && !sat2->isKinematic())
		{
			sat1->setPosition(sat1->getPosition() - (smallest * (overlap * res1)));
			sat2->setPosition(sat2->getPosition() + (smallest * (overlap * res2)));
		}
		else if (!sat1->isKinematic())
		{
			sat1->setPosition(sat1->getPosition() - (smallest * (overlap * res1)));
		}
		else
		{
			sat2->setPosition(sat2->getPosition() + (smallest * (overlap * res2)));
		}

		sat1->resolveCollision(sat2, -smallest);

		return true;
	}
	return false;
}

