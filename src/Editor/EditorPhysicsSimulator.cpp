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
    /*btTransform t;      // Where to put the rigidBody
    t.setIdentity();
    t.setOrigin(btVector3(0,-50,0));
    btMotionState* motion = new btDefaultMotionState(t);

    btBoxShape* tempGround = new btBoxShape(btVector3(btScalar(50.), btScalar(50.), btScalar(50.)));
    m_collisionShapes.push_back(tempGround);

    btRigidBody::btRigidBodyConstructionInfo info(0.0, motion, tempGround);
    btRigidBody* groundRB = new btRigidBody(info);

    m_rigidBodies.push_back(groundRB);


    // Temp: Create Test dynamic RigidBody sphere
    btTransform tS;      // Where to put the rigidBody
    tS.setIdentity();
    tS.setOrigin(btVector3(1,20,1));
    btMotionState* motionS = new btDefaultMotionState(tS);

    btCollisionShape* tempSphereShape = new btSphereShape(btScalar (.5f));
    m_collisionShapes.push_back(tempSphereShape);

    btVector3 inertia(btVector3(0, 1, 1));
    tempSphereShape->calculateLocalInertia(btScalar(1.2f), inertia);

    btRigidBody::btRigidBodyConstructionInfo infoS(1.0f, motionS, tempSphereShape, inertia);
    sphereRigidBody = new btRigidBody(infoS);

    m_rigidBodies.push_back(sphereRigidBody);*/
}

void EditorPhysicsSimulator::renderTestScene(Renderer & renderer)
{
    btTransform sphereT = sphereRigidBody->getWorldTransform();

    btVector3 pos       = sphereT.getOrigin();
    glm::vec3 glm_pos   = glm::vec3(pos.getX(), pos.getY(), pos.getZ());
    float radius        = ((btSphereShape*)sphereRigidBody->getCollisionShape())->getRadius();
    Model sphereObj     = Primitives::createSphere(glm_pos, radius, 1);

    renderer.draw(sphereObj);
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