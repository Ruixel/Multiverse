#include "Collision.h"

// Note: Most of this code comes from Kasper Fauerby's tutorial on collision detection
// It's very well written and I highly recommend it 
// Link: http://www.peroxide.dk/papers/collision/collision.pdf

// I've modified the code so it works with glm & the game's environment

glm::vec3 Collision::eSpace::collideWithWorld(const std::vector<std::shared_ptr<CYGeneric>>& obj_list,
	P_CollisionPacket* pkg, const glm::vec3 & pos, const glm::vec3 & vel, int depth)
{
	// Determine how close we want to get to a polygon
	float unitScale = unitsPerMeter / 100.0f;
	float veryCloseDistance = 0.005f * unitScale;

	// Avoid recursing too deep
	if (depth > 5)
		return pos;

	// Set the packet data
	pkg->e_velocity		 = vel;
	pkg->e_velocityN	 = glm::normalize(vel);
	pkg->e_origin		 = pos;
	pkg->foundCollision  = false;
	pkg->nearestDistance = INFINITY;

	// Iterate through every collision mesh 
	for (auto & obj : obj_list)
		for (auto & c_tri : obj->getCollisionMesh())
			Collision::eSpace::checkCollision(pkg, c_tri);

	// If there's no collision
	if (!pkg->foundCollision)
	{
		if (depth > 2)
			pkg->foundCollision = true;

		return glm::vec3(pos + vel);
	}

	// If there is a collision detected
	glm::vec3 destination = pos + vel;
	glm::vec3 newPoint = pos;

	if (pkg->nearestDistance >= veryCloseDistance)
	{
		glm::vec3 newV = (float)(pkg->nearestDistance - veryCloseDistance) * pkg->e_velocityN;
		newPoint = pkg->e_origin + newV;

		newV = glm::normalize(newV);
		pkg->collisionPoint -= veryCloseDistance * newV;
	}

	// Determine the sliding plane
	glm::vec3 slidePlaneOrigin = pkg->collisionPoint;
	glm::vec3 slidePlaneNormal = newPoint - pkg->collisionPoint;
	slidePlaneNormal = glm::normalize(slidePlaneNormal);

	double eq = -(slidePlaneNormal.x*slidePlaneOrigin.x
		+ slidePlaneNormal.y*slidePlaneOrigin.y
		+ slidePlaneNormal.z*slidePlaneOrigin.z);
	double distanceFromDestinationToPlane = glm::dot(destination, slidePlaneNormal) + eq;

	glm::vec3 newDestination = destination - ((float)distanceFromDestinationToPlane * slidePlaneNormal);

	glm::vec3 newVelocityVector = newDestination - pkg->collisionPoint;

	// Recursion
	if (glm::length(newVelocityVector) < veryCloseDistance) {
		return newPoint;
	}

	return collideWithWorld(obj_list, pkg, newPoint, newVelocityVector, depth + 1);
}

glm::vec3 Collision::eSpace::covertToESpace(const glm::vec3 & vec, const glm::vec3& pRadius)
{
	return glm::vec3(vec.x / pRadius.x, vec.y / pRadius.y, vec.z / pRadius.z);
}

bool Collision::eSpace::getLowestRoot(float a, float b, float c, float maxR, float* newT)
{
	float determinant = b * b - 4 * a*c;

	// Check for any roots
	if (determinant < 0)
		return false;

	// If the determinant is 0, then there's only 1 root
	if (fabs(determinant) < 0.0001f)
	{
		float x = -b / (2 * a);
		if (x > 0 && x < maxR)
		{
			*newT = x;
			return true;
		}
	}

	float sDeterminant = sqrt(determinant);

	// Determine whether A is positive so we don't need to swap the values
	int aPositive = (0 < a) - (a < 0);
	float x1 = (-b + -aPositive * sDeterminant) / (2 * a);

	if (x1 > 0 && x1 < maxR)
	{
		*newT = x1;
		return true;
	}

	// If x1 is less than 0, then we'll take the larger root
	float x2 = (-b + aPositive * sDeterminant) / (2 * a);
	if (x2 > 0 && x2 < maxR)
	{
		*newT = x2;
		return true;
	}

	return false;
}

void Collision::eSpace::checkCollision(P_CollisionPacket* pkg, const CPolygon& c_tri)
{
	glm::vec3 p1, p2, p3;

	// Convert to Epsilon Space
	p1 = Collision::eSpace::covertToESpace(Coordinate::WorldToLevel(c_tri.vertex[0]), pkg->playerRadius);
	p2 = Collision::eSpace::covertToESpace(Coordinate::WorldToLevel(c_tri.vertex[1]), pkg->playerRadius);
	p3 = Collision::eSpace::covertToESpace(Coordinate::WorldToLevel(c_tri.vertex[2]), pkg->playerRadius);

	// Check if the velocity vector is facing the front of the triangle 
	if (glm::dot(c_tri.normal, pkg->e_velocityN) >= 0)
	{
		double t0, t1;
		bool embeddedInPlane = false;

		glm::vec3 planeNormal = c_tri.normal;
		//if ((pkg->R3Position.x * c_tri.normal.x + pkg->R3Position.y * c_tri.normal.y + pkg->R3Position.z * c_tri.normal.z + c_tri.d) < 0)
		//if (glm::dot(c_tri.normal, pkg->e_velocityN) < 0)
		//{
		//	planeNormal = -c_tri.normal;
		//}

		//if ((pkg->R3Position.x * c_tri.normal.x + pkg->R3Position.y * c_tri.normal.y + pkg->R3Position.z * c_tri.normal.z + c_tri.d) > 0)
		//		return;

		// Get the +ve distance from the origin of the camera to the triangle's plane
		double eq = -(planeNormal.x*p1.x + planeNormal.y*p1.y + planeNormal.z*p1.z);
		double distanceToPlane = glm::dot(pkg->e_origin, planeNormal) + eq;

		float normalDotVelocity = glm::dot(planeNormal,pkg->e_velocityN);

		// Check if the player is travelling parallel to the plane
		if (normalDotVelocity == 0.f)
		{
			// Check if the plane is not embedded
			if (fabs(distanceToPlane) >= 1.0f)
			{
				// No collision possible, we can skip checking
				return;
			}
			else {
				// The plane is actually embedded despite being parallel
				// Maybe stuck?
				embeddedInPlane = true;
				t0 = 0.0f; t1 = 1.0f;
			}
		}
		else {
			t0 = (-1.0f - distanceToPlane) / normalDotVelocity;
			t1 = (1.0f - distanceToPlane) / normalDotVelocity;
			//d_gui.addMessage("ndv: " + std::to_string(normalDotVelocity));

			// Assure that t0 is below t1
			if (t0 > t1)
			{
				// Swap
				double temp = t1;
				t1 = t0;
				t0 = t1;
			}

			// Make sure that at least one result lies between 0 and 1
			// If not then there is no possibility of collision
			if (t0 > 1.0f || t1 < 0.0f)
				return;

			// Clamp values to 0-1
			if (t0 < 0.f) t0 = 0.f;
			if (t0 > 1.f) t0 = 1.f;
			if (t1 < 0.f) t1 = 0.f;
			if (t1 > 1.f) t1 = 1.f;
		}

		// With t0 and t1 calculated, for any collision to occur, they must be between
		// these two time values
		glm::vec3 pointOfCollision;
		bool collided = false;
		float t = 1.0f;

		// First collision check - Collision inside the triangle
		// This must occur at t0 as that's when it rests on the front of the triangle
		if (!embeddedInPlane)
		{
			//std::cout << "Checking " << distanceToPlane << "\n";
			glm::vec3 planeIntersectionPoint = (pkg->e_origin - planeNormal)
				+ ((float)t0 * pkg->e_velocity);

			// Check if this point exists within the triangle
			if (Collision::eSpace::checkPointInTriangle(planeIntersectionPoint, p1, p2, p3))
			{
				//std::cout << "Collision detected " << distanceToPlane << "\n";

				collided = true;
				t = (float)t0;
				pointOfCollision = planeIntersectionPoint;


			}
		}

		// If no collision has been found yet, a swept sphere is checked against
		// every point and line of the triangle
		if (collided == false)
		{
			float velocityLengthSquared = glm::length2(pkg->e_velocity);
			float a, b, c;
			float newT;

			// Check every point
			a = velocityLengthSquared;

			auto checkPoint = [&](const glm::vec3& p)
			{
				b = 2.f * (glm::dot(pkg->e_velocityN, pkg->e_origin - p));
				c = glm::length2(p - pkg->e_origin) - 1.f;

				//std::cout << "a: " << a << ", b: " << b << ", c: " << c << "\n";
				if (Collision::eSpace::getLowestRoot(a, b, c, t, &newT))
				{
					t = newT;
					collided = true;
					pointOfCollision = p;
				}
			};

			// Point 1
			checkPoint(p1);
			checkPoint(p2);
			checkPoint(p3);

			// Check edges/lines of the triangle 
			glm::vec3 edge, baseToVertex;
			float edgeSquaredLength, edgeDotVelocity, edgeDotOriginToVertex;

			// p_1 -> p_2
			auto checkEdge = [&](const glm::vec3& p_1, const glm::vec3& p_2)
			{
				edge = p_2 - p_1;
				baseToVertex = p_1 - pkg->e_origin;
				edgeSquaredLength = glm::length2(edge);
				edgeDotVelocity = glm::dot(edge, pkg->e_velocity);
				edgeDotOriginToVertex = glm::dot(edge, baseToVertex);

				// Quadratic parameters
				// A = ||edge||^2 * -||vel||^2 + (edge . vel)^2
				a = (edgeSquaredLength * -velocityLengthSquared) +
					(edgeDotVelocity * edgeDotVelocity);
				// B = ||edge||^2 * 2(vel . baseToVertex) - 2((edge . vel)(edge . baseToVertex)
				b = (edgeSquaredLength * (2.f * glm::dot(pkg->e_velocity, baseToVertex))) -
					(2.f * edgeDotVelocity * edgeDotOriginToVertex);
				// C = ||edge||^2 * (1 - ||baseToVertex||^2) + (edge . baseToVertex)^2
				c = (edgeSquaredLength * (1.f - glm::length2(baseToVertex))) +
					(edgeDotOriginToVertex * edgeDotOriginToVertex);

				// Check if the sweep sphere collides with the infinite line created
				if (Collision::eSpace::getLowestRoot(a, b, c, t, &newT))
				{
					float f = (edgeDotVelocity * newT - edgeDotOriginToVertex) / edgeSquaredLength;
					if (f >= 0.f && f <= 1.f)
					{
						// New intersection
						t = newT;
						collided = true;
						pointOfCollision = p_1 + f * edge;
					}
				}
			};

			checkEdge(p1, p2);
			checkEdge(p2, p3);
			checkEdge(p3, p1);

		}

		if (collided) {
			float distanceToCollision = t * glm::length(pkg->e_velocity);

			if (distanceToCollision < pkg->nearestDistance)
			{
				pkg->foundCollision  = true;
				pkg->nearestDistance = distanceToCollision;
				pkg->collisionPoint = pointOfCollision;
			}
		}
	}
}

// Code written by Keidy from Mr-Gamemaker
bool Collision::eSpace::checkPointInTriangle(const glm::vec3 & point, const glm::vec3 & pa,
	const glm::vec3 & pb, const glm::vec3 & pc)
{
	glm::vec3 e10 = pb - pa;
	glm::vec3 e20 = pc - pa;
	float a = glm::dot(e10, e10);
	float b = glm::dot(e10, e20);
	float c = glm::dot(e20, e20);
	float ac_bb = (a*c) - (b*b);
	glm::vec3 vp(point.x - pa.x, point.y - pa.y, point.z - pa.z);
	float d = glm::dot(vp, e10);
	float e = glm::dot(vp, e20);
	float x = (d*c) - (e*b);
	float y = (e*a) - (d*b);
	float z = x + y - ac_bb;
	return ((collision_in(z)& ~(collision_in(x) | collision_in(y))) & 0x80000000);
}
