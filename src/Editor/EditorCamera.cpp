#include "EditorCamera.h"

EditorCamera::EditorCamera()
{
    // Default camera
    Transform start_cam;
    m_camera = std::make_unique<FreeroamEditorView>(start_cam);
}

std::shared_ptr<EditorView> EditorCamera::get()
{
    return m_camera;
}

void EditorCamera::changeCamera(CameraType type)
{
    switch(type)
    {
        case CameraType::GRID:
            m_camera = std::make_unique<GridEditorView>(m_camera->getTransformation());
            break;
        case CameraType::FREEROAM:
            m_camera = std::make_unique<FreeroamEditorView>(m_camera->getTransformation());
            break;
        case CameraType::PLAYER:
            m_camera = std::make_unique<PlayerEditorView>(m_camera->getTransformation());
            break;
    }
}