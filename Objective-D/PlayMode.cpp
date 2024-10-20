#include "MouseUtil.h"
#include "PlayMode.h"

// 게임 모드
#include "Terrain.h"

void PlayMode::Start() {
	SetBackgroundColor(0.7, 0.7, 0.7);
	
	framework.AddObject(new Terrain, "terrain", Layer::L1);

	framework.RegisterKeyController(KeyboardController);
	framework.RegisterMouseController(MouseController);
	framework.RegisterMouseMotionController(MouseMotionController);
	framework.RegisterDestructor(Destructor);
	framework.RegisterModeName("PlayMode");
}

void PlayMode::Destructor() {

}

void PlayMode::KeyboardController(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
	if (nMessageID == WM_KEYDOWN && wParam == VK_ESCAPE)
		framework.Exit();
}

void PlayMode::MouseMotionController(HWND hWnd) {
	mouse.UpdateMousePosition(hWnd);
}

void PlayMode::MouseController(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
	mouse.UpdateButtonState(nMessageID);
}