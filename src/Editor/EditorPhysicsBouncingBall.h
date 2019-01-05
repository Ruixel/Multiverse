#pragma once

#include "btBulletDynamicsCommon.h"
#include "../Renderer/Model.h"
#include "../Renderer/Renderer.h"

class EditorPhysicsBouncingBall
{
public:
	// Constructor
	EditorPhysicsBouncingBall(btRigidBody* m_rbody, float radius) : m_rbody(m_rbody) 
	{
		m_model = Primitives::createCube(glm::vec3(-.2f, -.2f, -.2f), glm::vec3(.2f, .2f, .2f));
	}

	// Copy Constructor (Model is non-copyable)
	EditorPhysicsBouncingBall(const EditorPhysicsBouncingBall & other)
	{
		m_rbody		= other.m_rbody;
		m_radius	= other.m_radius;
		m_model		= Primitives::createCube(glm::vec3(-.2f, -.2f, -.2f), glm::vec3(.2f, .2f, .2f));
	}

	// Move constructor
	EditorPhysicsBouncingBall(EditorPhysicsBouncingBall&& other)
	{
		m_rbody		= other.m_rbody;
		m_radius	= other.m_radius;
		m_model		= Primitives::createCube(glm::vec3(-.2f, -.2f, -.2f), glm::vec3(.2f, .2f, .2f));

		other.m_rbody = nullptr;
		other.m_model.destroyModel();
	}

	// Destructor 
	~EditorPhysicsBouncingBall() { m_model.destroyModel(); }

	void render(Renderer& renderer, glm::mat4 modelMat) {
		m_model.setModelMatrix(modelMat);

		renderer.draw(this->m_model);
	}

	btRigidBody* m_rbody;

private:
	bool  m_hasModel = true;
	Model m_model;

	float m_radius;
};

