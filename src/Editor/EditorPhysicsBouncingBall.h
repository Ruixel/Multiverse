#pragma once

#include "btBulletDynamicsCommon.h"
#include "../Renderer/Model.h"
#include "../Renderer/Renderer.h"

class EditorPhysicsBouncingBall
{
public:
	EditorPhysicsBouncingBall(btRigidBody* m_rbody) : m_rbody(m_rbody) {}

	void render(Renderer& renderer, glm::vec3 pos, float radius) {
		if (m_hasModel)
			m_model.destroyModel();

		m_model		= Primitives::createSphere(pos, radius, 1);
		m_hasModel	= true;

		renderer.draw(this->m_model);
	}

	btRigidBody* m_rbody;

private:
	bool  m_hasModel = true;
	Model m_model;
};

