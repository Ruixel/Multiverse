#include "GridEditorView.h"

void GridEditorView::input(const Controller &controller)
{
	m_velocity = glm::vec3(0, 0, 0);
	float speed = 0.01f;
	if (controller.forwardPressed()) {
		m_velocity.x += -glm::cos(glm::radians(m_transform.rotation.y + 90)) * speed;
		m_velocity.z += -glm::sin(glm::radians(m_transform.rotation.y + 90)) * speed;
	}
	if (controller.backPressed()) {
		m_velocity.x += glm::cos(glm::radians(m_transform.rotation.y + 90)) * speed;
		m_velocity.z += glm::sin(glm::radians(m_transform.rotation.y + 90)) * speed;
	}
	if (controller.leftPressed()) {
		m_velocity.x += -glm::cos(glm::radians(m_transform.rotation.y)) * speed;
		m_velocity.z += -glm::sin(glm::radians(m_transform.rotation.y)) * speed;
	}
	if (controller.rightPressed()) {
		m_velocity.x += glm::cos(glm::radians(m_transform.rotation.y)) * speed;
		m_velocity.z += glm::sin(glm::radians(m_transform.rotation.y)) * speed;
	}

	// Mouse Drag
	if (controller.mouse2Down())
	{
		float mouseDragSpeed = 600.f;

		// Set variables to compare to (when mouse2 is pressed)
		if (m_grid_m2Down == false)
		{
			m_grid_m2Down = true;
			m_grid_startPos = controller.getMousePositionRelativeToWindow();
			m_grid_dragStartVec = m_transform.position;
		}

		// Apply velocity to the startPos for WASD to have an effect
		m_grid_dragStartVec += m_velocity;
		m_velocity = glm::vec3(0, 0, 0);

		// Get the difference in mouse position
		sf::Vector2i mouseDiff = m_grid_startPos - controller.getMousePositionRelativeToWindow();
		glm::vec3 newPos = m_grid_dragStartVec;

		// Apply difference in 3D
		newPos.x += glm::cos(glm::radians(m_transform.rotation.y)) * mouseDiff.x / mouseDragSpeed;
		newPos.x += glm::cos(glm::radians(m_transform.rotation.y + 90)) * mouseDiff.y / mouseDragSpeed;

		newPos.z += glm::sin(glm::radians(m_transform.rotation.y)) * mouseDiff.x / mouseDragSpeed;
		newPos.z += glm::sin(glm::radians(m_transform.rotation.y + 90)) * mouseDiff.y / mouseDragSpeed;

		m_transform.position = newPos;
	}
	else {
		m_grid_m2Down = false;
	}

	// Mouse Rotation
	if (controller.mouse3Down())
	{
		if (m_grid_m3Down == false)
		{
			m_grid_m3Down = true;
			//m_grid_startRay = mRay;

		}

	}
	else {
		m_grid_m3Down = false;
	}

}

void GridEditorView::update(float dt, DebugLogGUI &d_gui, u8 floor, GeoOctree &octr)
{
	glm::vec3 pos = m_transform.position;
	m_transform.position = Maths::lerp(pos, glm::vec3(pos.x, ((float)floor + 3.5f) * WORLD_HEIGHT / WORLD_SIZE, pos.z), 0.4f);

    m_viewMatrix = makeViewMatrix(*this);
}

