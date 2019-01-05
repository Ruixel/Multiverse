#include "EditorPhysicsSimulator.h"

// Physics world init
EditorPhysicsSimulator::EditorPhysicsSimulator()
{
    // Initialise the physics configuration & dependencies
    collisionConfiguration = new btDefaultCollisionConfiguration();
    dispatcher = new btCollisionDispatcher(collisionConfiguration);
    overlappingPairCache = new btDbvtBroadphase();
    seqSolver = new btSequentialImpulseConstraintSolver();
	
    // Create world
    dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,
            overlappingPairCache, seqSolver, collisionConfiguration);

    dynamicsWorld->setGravity(btVector3(0, -10, 0));

    // Temp: Create a ground collision mesh
    btTransform t;      // Where to put the rigidBody
    t.setIdentity();
    t.setOrigin(btVector3(0,-50,0));
    btMotionState* motion = new btDefaultMotionState(t);

    btBoxShape* tempGround = new btBoxShape(btVector3(btScalar(50.), btScalar(50.), btScalar(50.)));
    m_collisionShapes.push_back(tempGround);

    btRigidBody::btRigidBodyConstructionInfo info(0.0, motion, tempGround);
	info.m_restitution = 1.0f;
	info.m_friction = 1.5f;
    btRigidBody* groundRB = new btRigidBody(info);

    m_rigidBodies.push_back(groundRB);
	dynamicsWorld->addRigidBody(groundRB);
}

void EditorPhysicsSimulator::update(float dt)
{
	dynamicsWorld->stepSimulation(dt);
}

void EditorPhysicsSimulator::renderTestScene(Renderer & renderer)
{
	for (auto& ball : m_balls)
	{
		btTransform sphereT;
		ball.m_rbody->getMotionState()->getWorldTransform(sphereT);

		btVector3 pos = sphereT.getOrigin();
		glm::vec3 glm_pos = glm::vec3(pos.getX(), pos.getY(), pos.getZ());
		float radius = ((btSphereShape*)ball.m_rbody->getCollisionShape())->getRadius();
		ball.render(renderer, this->bulletToGlm(sphereT));

	}
}

void EditorPhysicsSimulator::launchBall(glm::vec3 ballPos, glm::vec3 ballDirection)
{
	std::cout << "Added ball\n";
	ballDirection = glm::normalize(ballDirection);

	// Convert glm::vec3 to btVector3
	btVector3 pos(ballPos.x, ballPos.y, ballPos.z);
	btVector3 dir(ballDirection.x, ballDirection.y, ballDirection.z);

	btTransform tS;    
	tS.setIdentity();
	tS.setOrigin(pos);
	btMotionState* motionS = new btDefaultMotionState(tS);

	float r = .2f;
	btCollisionShape* tempSphereShape = new btBoxShape(btVector3(.2f, .2f, .2f));
	m_collisionShapes.push_back(tempSphereShape);

	btVector3 inertia(btVector3(1, 1, 1));
	tempSphereShape->calculateLocalInertia(btScalar(1.2f), inertia);

	btRigidBody::btRigidBodyConstructionInfo info(1.2f, motionS, tempSphereShape, inertia);
	info.m_restitution = 0.3f;
	info.m_friction = 1.5f;
	btRigidBody* ballRB = new btRigidBody(info);

	dynamicsWorld->addRigidBody(ballRB);
	ballRB->setLinearVelocity(dir * 5);

	EditorPhysicsBouncingBall b(ballRB, 0.2f);
	m_balls.emplace_back(std::move(b));
	
}

glm::mat4 EditorPhysicsSimulator::bulletToGlm(const btTransform& t)
{
	btQuaternion rotation = t.getRotation();
	btVector3 transform = t.getOrigin();

	glm::mat4 mat;
	mat = glm::translate(mat, glm::vec3(transform.getX(), transform.getY(), transform.getZ()));
	mat = glm::rotate(mat, (float)rotation.getAngle(), glm::vec3(rotation.getAxis().getX(), rotation.getAxis().getY(), rotation.getAxis().getZ()));

	return mat;
}

// Destroy
EditorPhysicsSimulator::~EditorPhysicsSimulator()
{
    delete dynamicsWorld;
    delete seqSolver;
    delete overlappingPairCache;
    delete dispatcher;
    delete collisionConfiguration;
}