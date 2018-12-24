#include "PlayerEditorView.h"

// Player Mode
void PlayerEditorView::input(const Controller &controller)
{
	playerCollider.input(controller, m_transform);
	m_transform.rotation += controller.getLookChange();
}

void PlayerEditorView::update(float dt, DebugLogGUI &d_gui, u8 floor, GeoOctree &octr)
{
	m_velocity = playerCollider.movePlayer(this->m_transform.position, dt, d_gui, octr);
    m_viewMatrix = makeViewMatrix(*this);
}
