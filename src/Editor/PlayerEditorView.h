#pragma once

#include "EditorView.h"

class PlayerEditorView : public EditorView {
public:
    PlayerEditorView(const Transform& cam_transform) : EditorView(cam_transform) {}

    virtual void input(const Controller &controller);
    virtual void update(float dt, DebugLogGUI &d_gui, u8 floor, GeoOctree &octr);

    virtual CameraType getType() { return CameraType::PLAYER; };

private:
    // Player Movement
    PlayerMovement playerCollider;
};
