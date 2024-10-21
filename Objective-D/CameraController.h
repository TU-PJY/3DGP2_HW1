#pragma once
#include "GameObject.h"
#include "CameraUtil.h"
#include "MouseUtil.h"

// 관전 카메라 컨트롤러
class CameraController : public GameObject {
private:
	bool MoveForward{}, MoveBackward{}, MoveRight{}, MoveLeft{};
	bool MoveUp{}, MoveDown{};

	XMFLOAT3 CamPosition{0.0, 30.0, 0.0};  // 카메라 위치 기억용

	// 카메라 회전
	XMFLOAT3 CamRotation{};
	XMFLOAT3 DestCamRotation{};

public:
	void InputKey(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
		switch (nMessageID) {
		case WM_KEYDOWN:
			switch (wParam) {
			case 'W': MoveForward = true; break;
			case 'A': MoveLeft = true; break;
			case 'S': MoveBackward = true; break;
			case 'D': MoveRight = true; break;
			case VK_SPACE: MoveUp = true; break;
			case VK_CONTROL: MoveDown = true; break;
				
			case VK_F2:  // 관전 모드로 변경한다.
				camera.SetCameraMode(CamMode::SPECTOR_MODE);
				// 마지막으로 기억한 위치로 카메라를 이동시킨다.
				camera.SetPosition(CamPosition);
				break;

			case VK_F1:  // 일반 모드로 변경한다.
				camera.SetCameraMode(CamMode::TRACK_MODE);
				break;
			}
			break;

		case WM_KEYUP:
			switch (wParam) {
			case 'W': MoveForward = false; break;
			case 'A': MoveLeft = false; break;
			case 'S': MoveBackward = false; break;
			case 'D': MoveRight = false; break;
			case VK_SPACE: MoveUp = false; break;
			case VK_CONTROL: MoveDown = false; break;
			}
			break;
		}
	}

	void InputMouseMotion(HWND hWnd, POINT PrevCursorPos) {
		if (GetCapture() == hWnd) {
			mouse.HideCursor();

			// 관전 모드에서만 동작
			if (camera.Mode == CamMode::SPECTOR_MODE) {
				float cxDelta = (float)(mouse.CurrentPosition().x - PrevCursorPos.x) / 5.0f;
				float cyDelta = (float)(mouse.CurrentPosition().y - PrevCursorPos.y) / 5.0f;
				mouse.SetPositionToPrev(PrevCursorPos);

				DestCamRotation.x += cyDelta * 0.003;
				DestCamRotation.y += cxDelta * 0.003;
			}
		}
	}

	void Update(float FT) {
		// 관전 모드에서만 동작한다.
		if (camera.Mode == CamMode::SPECTOR_MODE) {
			if (MoveForward)
				camera.MoveForwardWithoutHeight(FT * 40);
			if (MoveBackward)
				camera.MoveForwardWithoutHeight(-FT * 40);
			if (MoveRight)
				camera.MoveStrafeWithoutHeight(FT * 40);
			if (MoveLeft)
				camera.MoveStrafeWithoutHeight(-FT * 40);
			if (MoveUp)
				camera.MoveVertical(FT * 40);
			if (MoveDown)
				camera.MoveVertical(-FT * 40);

			// 부드러운 카메라 회전
			CamRotation.x = std::lerp(CamRotation.x, DestCamRotation.x, FT * 15);
			CamRotation.y = std::lerp(CamRotation.y, DestCamRotation.y, FT * 15);

			camera.Rotate(CamRotation.x, CamRotation.y, 0.0);

			CamPosition = camera.GetPosition();
		}
	}
};