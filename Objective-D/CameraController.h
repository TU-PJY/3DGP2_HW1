#pragma once
#include "GameObject.h"
#include "CameraUtil.h"
#include "MouseUtil.h"

// ���� ī�޶� ��Ʈ�ѷ�
class CameraController : public GameObject {
public:
	bool MoveForward{}, MoveBackward{}, MoveRight{}, MoveLeft{};
	bool MoveUp{}, MoveDown{};

	XMFLOAT3 CamPosition{};  // ī�޶� ��ġ ����
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


			case VK_F2:  // ���� ���� �����Ѵ�.
				camera.SetCameraMode(CamMode::SPECTOR_MODE);
				// ���������� ����� ��ġ�� ī�޶� �̵���Ų��.
				camera.Move(CamPosition);
				break;

			case VK_F1:  // �Ϲ� ���� �����Ѵ�.
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
		// ���� ��忡���� �����Ѵ�.
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