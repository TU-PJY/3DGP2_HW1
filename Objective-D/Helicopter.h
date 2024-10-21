#pragma once
#include "GameObject.h"
#include "TransformUtil.h"
#include "MathUtil.h"
#include "CameraUtil.h"
#include <cmath>

// 헬리콘터 오브젝트

class Helicopter : public GameObject {
private:
	Vector Vec;
	XMFLOAT3 Position{0.0, 40.0, 40.0};

	float WingRotation{};

	XMFLOAT3 HeliRotation{};
	XMFLOAT3 DestRotation{};

	bool MoveForward{}, MoveBackward{}, MoveRight{}, MoveLeft{};
	float Speed = 10.0;

public:
	XMFLOAT3 GetUp() { return Vec.Up; }
	XMFLOAT3 GetLook() { return Vec.Look; }
	XMFLOAT3 GetRight() { return Vec.Right; }
	XMFLOAT3 GetPosition() { return Position; }

	Helicopter() {
		Math::InitVector(Vec.Up, Vec.Right, Vec.Look);
	}

	void InputKey(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
		if (camera.Mode == CamMode::DEFAULT_MODE) {
			switch (nMessageID) {
			case WM_KEYDOWN:
				switch (wParam) {
				case 'W': MoveForward = true; break;
				case 'S': MoveBackward = true; break;
				case 'A': MoveLeft = true; break;
				case 'D': MoveRight = true; break;
				}
				break;

			case WM_KEYUP:
				switch (wParam) {
				case 'W': MoveForward = false; break;
				case 'S': MoveBackward = false; break;
				case 'A': MoveLeft = false; break;
				case 'D': MoveRight = false; break;
				}
			}
		}
	}
	
	void InputMouseMotion(HWND hWnd, POINT PrevCursorPos) {
		if (GetCapture() == hWnd) {
			mouse.HideCursor();

			// 관전 모드에서만 동작
			if (camera.Mode == CamMode::DEFAULT_MODE) {
				float cxDelta = (float)(mouse.CurrentPosition().x - PrevCursorPos.x) / 5.0f;
				float cyDelta = (float)(mouse.CurrentPosition().y - PrevCursorPos.y) / 5.0f;
				mouse.SetPositionToPrev(PrevCursorPos);

				DestRotation.x += cyDelta * 0.5;
				DestRotation.y += cxDelta * 0.5;
			}
		}
	}

	void InputMouseButton(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {

	}

	void Update(float FT) {
		// 날개 회전
		WingRotation += FT * 2000;

		if (DestRotation.x < -60.0)
			DestRotation.x = -60.0;

		if (DestRotation.x > 60.0)
			DestRotation.x = 60.0;

		if (MoveForward) {
			Position.x += sin(XMConvertToRadians(HeliRotation.y)) * Speed * FT;
			Position.z += cos(XMConvertToRadians(HeliRotation.y)) * Speed * FT;
		}
		if (MoveBackward) {
			Position.x -= sin(XMConvertToRadians(HeliRotation.y)) * Speed * FT;
			Position.z -= cos(XMConvertToRadians(HeliRotation.y)) * Speed * FT;
		}
		if (MoveRight) {
			Position.x += cos(XMConvertToRadians(HeliRotation.y)) * Speed * FT;
			Position.z -= sin(XMConvertToRadians(HeliRotation.y)) * Speed * FT;
		}
		if (MoveLeft) {
			Position.x += cos(XMConvertToRadians(HeliRotation.y)) * -Speed * FT;
			Position.z -= sin(XMConvertToRadians(HeliRotation.y)) * -Speed * FT;
		}

		HeliRotation.x = std::lerp(HeliRotation.x, DestRotation.x, FT);
		HeliRotation.y = std::lerp(HeliRotation.y, DestRotation.y, FT);
	}

	void Render(CommandList CmdList) {
		// 헬기 몸통
		InitMatrix(CmdList, RenderType::Pers);
		Transform::Scale(ScaleMatrix, 0.5, 0.5, 0.5);

		Transform::Move(TranslateMatrix, Position.x, Position.y, Position.z);
		Transform::Rotate(TranslateMatrix, 0.0, HeliRotation.y, 0.0);
		Transform::Rotate(TranslateMatrix, HeliRotation.x, 0.0, 0.0);

		FlipTexture(CmdList, false, true);
		BindTexture(CmdList, HelicopterTex);
		UseShader(CmdList, BasicShader);
		RenderMesh(CmdList, HelicopterBodyMesh);



		// 헬기 날개
		InitMatrix(CmdList, RenderType::Pers);
		Transform::Scale(ScaleMatrix, 0.5, 0.5, 0.5);

		Transform::Move(TranslateMatrix, Position.x, Position.y, Position.z);
		Transform::Rotate(TranslateMatrix, 0.0, HeliRotation.y, 0.0);
		Transform::Rotate(TranslateMatrix, HeliRotation.x, 0.0, 0.0);

		Transform::Move(TranslateMatrix, 0.0, 2.0, 0.0);

		Transform::Rotate(TranslateMatrix, 0.0, WingRotation, 0.0);

		FlipTexture(CmdList, false, true);
		BindTexture(CmdList, HelicopterTex);
		UseShader(CmdList, BasicShader);
		RenderMesh(CmdList, HelicopterWingMesh);

		// 벡터 업데이트
		Math::UpdateVector(Vec.Up, Vec.Right, Vec.Look, HeliRotation.x, HeliRotation.y, HeliRotation.z);
	}
};