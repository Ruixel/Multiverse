#pragma once

#include <SFML/System.hpp>

#include <iostream>
#include "../Renderer/Camera.h"
#include "../Input/Controller.h"
#include "../Game/WorldConstants.h"
#include "../Util/MouseRay.h"
#include "../Editor/GeoOctree.h"
#include "../GUI/DebugLogGUI.h"

#include "../Game/PlayerMovement.h"

enum class CameraType { FREEROAM, GRID, PLAYER, NONE };

class EditorView : public Camera
{
public:
    EditorView(const Transform& cam_transform)
    {
        this->m_transform = cam_transform;
    }

	virtual void input(const Controller& controller) = 0;
	virtual void update(float dt, DebugLogGUI& d_gui, u8 floor, GeoOctree& octr) = 0;

    virtual CameraType getType() { return CameraType::NONE; }
};
