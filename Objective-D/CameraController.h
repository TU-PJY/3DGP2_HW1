#pragma once
#include "GameObject.h"
#include "CameraUtil.h"
#include "MouseUtil.h"

// 관전 카메라 컨트롤러
class CameraController : public GameObject {
public:
	bool MoveForward{}, MoveBackward{}, MoveRight{}, MoveLeft{};
	bool MoveUp{}, MoveDown{};

	XMFLOAT3 CamPosition{};  // 카메라 위치 기억용
	XMFLOAT3 CamRotation{};

	void InputKey(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
		switch (nMessageID) {
		case WM_KEYDOWN:
			switch (wParam) {
			case 'W':
				MoveForward = true;
				break;

			case 'A':
				MoveLeft = true;
				break;

			case 'S':
				MoveBackward = true;
				break;

			case 'D':
				MoveRight = true;
				break;

			case VK_SPACE:
				MoveUp = true;
				break;

			case VK_CONTROL:
				MoveDown = true;
				break;


			case VK_F2:  // 관전 모드로 변경한다.
				camera.SetCameraMode(CamMode::SPECTOR_MODE);
				// 마지막으로 기억한 위치로 카메라를 이동시킨다.
				camera.Move(CamPosition);
				break;

			case VK_F1:  // 일반 모드로 변경한다.
				camera.SetCameraMode(CamMode::DEFAULT_MODE);
				break;
			}
			break;

		case WM_KEYUP:
			switch (wParam) {
			case 'W':
				MoveForward = false;
				break;

			case 'A':
				MoveLeft = false;
				break;

			case 'S':
				MoveBackward = false;
				break;

			case 'D':
				MoveRight = false;
				break;

			case VK_SPACE:
				MoveUp = false;
				break;

			case VK_CONTROL:
				MoveDown = false;
				break;
			}
			break;
		}
	}

	void InputMouseMotion(HWND hWnd, POINT PrevCursorPos) {
		if (camera.Mode == CamMode::SPECTOR_MODE) {
			if (mouse.LBUTTONDOWN && GetCapture() == hWnd) {
				mouse.HideCursor();
				GetCapture();

				float cxDelta = (float)(mouse.CurrentPosition().x - PrevCursorPos.x) / 5.0f;
				float cyDelta = (float)(mouse.CurrentPosition().y - PrevCursorPos.y) / 5.0f;
				mouse.SetPositionToPrev(PrevCursorPos);

				CamRotation.x += cyDelta * 0.008;
				CamRotation.y += cxDelta * 0.008;
			}
		}
	}

	void InputMouseButton(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
		if (camera.Mode == CamMode::SPECTOR_MODE) {
			switch (nMessageID) {
			case WM_LBUTTONDOWN:
				mouse.CaptureMotion(hWnd);
				break;

			case WM_LBUTTONUP:
				mouse.ReleaseMotion();
				break;
			}
		}
	}

	void Update(float FT) {
		// 관점 모드에서만 동작한다.
		if (camera.Mode == CamMode::SPECTOR_MODE) {
			if (MoveForward)
				camera.MoveForward(FT * 15);
			if (MoveBackward)
				camera.MoveForward(-FT * 15);
			if (MoveRight)
				camera.MoveStrafe(FT * 15);
			if (MoveLeft)
				camera.MoveStrafe(-FT * 15);
			if (MoveDown)
				camera.MoveVertical(-FT * 10);
			if (MoveUp)
				camera.MoveVertical(FT * 10);

			camera.Rotate(CamRotation.x, CamRotation.y, 0.0);

			CamPosition = camera.GetPosition();
		}
	}
};