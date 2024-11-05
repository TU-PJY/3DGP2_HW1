#include "MouseUtil.h"
#include "PlayMode.h"
#include <random>

// 게임 모드
#include "CameraController.h"
#include "Terrain.h"
#include "Tree.h"
#include "Helicopter.h"
#include "Crosshair.h"
#include "Building.h"
#include "Skybox.h"

void PlayMode::Start() {
	SetBackgroundColor(0.7, 0.7, 0.7);
	
	scene.AddObject(new Skybox, "skybox", LAYER_1);
	scene.AddObject(new CameraController, "cam_controller", LAYER_1);
	scene.AddObject(new Terrain, "terrain", LAYER_1);
	scene.AddObject(new Helicopter, "helicopter", LAYER_1);
	scene.AddObject(new Building, "building", LAYER_1);
	scene.AddObject(new Crosshair, "crosshair", LAYER_3);

	std::random_device rd{};
	std::uniform_real_distribution<float> DistX(-70.0, 70.0);
	std::uniform_real_distribution<float> DistY(-70.0, 70.0);

	// 랜덤 위치에 나무 15그루를 추가한다
	for (int i = 0; i < 15; ++i) {
		float RandX, RandZ;
		RandX = DistX(rd);
		RandZ = DistY(rd);
		scene.AddObject(new Tree(RandX, RandZ), "tree", LAYER_1);
	}

	mouse.StartMotionCapture(MainHWND);

	scene.RegisterKeyController(KeyboardController);
	scene.RegisterMouseController(MouseController);
	scene.RegisterMouseMotionController(MouseMotionController);
	scene.RegisterDestructor(Destructor);
	scene.RegisterModeName("PlayMode");
}

void PlayMode::Destructor() {

}

void PlayMode::KeyboardController(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
	if (nMessageID == WM_KEYDOWN && wParam == VK_ESCAPE)
		scene.Exit();

	scene.InputKey(hWnd, nMessageID, wParam, lParam, "cam_controller");
	scene.InputKey(hWnd, nMessageID, wParam, lParam, "helicopter");
}

void PlayMode::MouseMotionController(HWND hWnd) {
	mouse.UpdateMousePosition(hWnd);

	scene.InputMouseMotion(hWnd, "cam_controller");
	scene.InputMouseMotion(hWnd, "helicopter");
}

void PlayMode::MouseController(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
	mouse.StartMotionCapture(MainHWND);
	scene.InputMouse(hWnd, nMessageID, wParam, lParam, "cam_controller");
}