#pragma once

#include "EditorView.h"

class GridEditorView : public EditorView {
public:
    GridEditorView(const Transform& cam_transform) : EditorView(cam_transform) {}

    virtual void input(const Controller& controller);
    virtual void update(float dt, DebugLogGUI& d_gui, u8 floor, GeoOctree& octr);

    virtual CameraType getType() { return CameraType::GRID; }

private:
    // Mouse Drag
	bool m_grid_m2Down = false;
	sf::Vector2i m_grid_startPos;
	glm::vec3 m_grid_dragStartVec;

	// Mouse Rotation
	bool m_grid_m3Down = false;
};


