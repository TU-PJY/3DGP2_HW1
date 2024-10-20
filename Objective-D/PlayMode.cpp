#include "MouseUtil.h"
#include "PlayMode.h"
#include <random>

// 게임 모드
#include "CameraController.h"
#include "Terrain.h"
#include "Tree.h"


void PlayMode::Start() {
	SetBackgroundColor(0.7, 0.7, 0.7);
	
	framework.AddObject(new CameraController, "cam_controller", Layer::L1);
	framework.AddObject(new Terrain, "terrain", Layer::L1);

	std::random_device rd{};
	std::uniform_real_distribution<float> DistX(-70.0, 70.0);
	std::uniform_real_distribution<float> DistY(-70.0, 70.0);

	// 랜덤 위치에 나무 15그루를 추가한다
	for (int i = 0; i < 15; ++i) {
		float RandX, RandZ;
		RandX = DistX(rd);
		RandZ = DistY(rd);
		framework.AddObject(new Tree(RandX, RandZ), "tree", Layer::L1);
	}

	mouse.CaptureMotion(MainHWND);

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

	if (auto cam_controller = framework.Find("cam_controller"); cam_controller)
		cam_controller->InputKey(hWnd, nMessageID, wParam, lParam);
}

void PlayMode::MouseMotionController(HWND hWnd) {
	mouse.UpdateMousePosition(hWnd);

	if (auto cam_controller = framework.Find("cam_controller"); cam_controller)
		cam_controller->InputMouseMotion(hWnd, mouse.PrevCursorPos);
}

void PlayMode::MouseController(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
	mouse.UpdateButtonState(nMessageID);

	if (auto cam_controller = framework.Find("cam_controller"); cam_controller)
		cam_controller->InputMouseButton(hWnd, nMessageID, wParam, lParam);
}