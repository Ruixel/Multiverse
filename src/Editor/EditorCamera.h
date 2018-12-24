#pragma once

#include "EditorView.h"
#include "FreeroamEditorView.h"
#include "GridEditorView.h"
#include "PlayerEditorView.h"

class EditorCamera {
public:
    EditorCamera();

    void changeCamera(CameraType type);
    std::shared_ptr<EditorView> get();
private:
    std::shared_ptr<EditorView> m_camera;
};