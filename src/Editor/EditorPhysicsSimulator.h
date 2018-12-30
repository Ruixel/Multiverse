#pragma once

#include "btBulletDynamicsCommon.h"
#include "../Renderer/Primitives.h"
#include "../Renderer/Renderer.h"
#include <vector>

#define BT_USE_DOUBLE_PRECISION

/*
 * EditorPhysicsSimulator Class
 * This class takes care of the physics simulation within the editor
 * It is only used when in player mode (for player movement & basic interaction)
 */

class EditorPhysicsSimulator {
public:
    EditorPhysicsSimulator();
    ~EditorPhysicsSimulator();

    void update(float dt);
    void renderTestScene(Renderer & renderer);

private:
    btDiscreteDynamicsWorld* dynamicsWorld;

    btDefaultCollisionConfiguration* collisionConfiguration;
    btCollisionDispatcher* dispatcher;
    btBroadphaseInterface* overlappingPairCache;
    btSequentialImpulseConstraintSolver* seqSolver;

    btAlignedObjectArray<btCollisionShape*> m_collisionShapes;
    std::vector<btRigidBody*> m_rigidBodies;

    btRigidBody* sphereRigidBody;
};
