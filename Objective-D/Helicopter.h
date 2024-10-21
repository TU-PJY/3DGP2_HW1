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

	XMFLOAT4X4 HeadMatrix{}; // 몸통의 기초 변환 결과를 저장하여 날개 변환으로 전달한다.

	bool MoveForward{}, MoveBackward{}, MoveRight{}, MoveLeft{};
	float ForwardSpeed{};
	float StrafeSpeed{};

	XMFLOAT3 Tilt{};

public:
	XMFLOAT3 GetUp() { return Vec.Up; }
	XMFLOAT3 GetLook() { return Vec.Look; }
	XMFLOAT3 GetRight() { return Vec.Right; }
	XMFLOAT3 GetPosition() { return Position; }

	Helicopter() {
		Math::InitVector(Vec.Up, Vec.Right, Vec.Look);
	}

	void InputKey(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
		if (camera.Mode == CamMode::TRACK_MODE) {
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
			if (camera.Mode == CamMode::TRACK_MODE) {
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

		// 헬리콥터 회전 각도 제한
		if (DestRotation.x < -50.0)
			DestRotation.x = -50.0;

		if (DestRotation.x > 50.0)
			DestRotation.x = 50.0;

		// 방향에 해당하는 키를 누르면 속도를 음수 또는 양수로 증가
		// 이동 방향으로 몸체를 기울인다.
		if (MoveForward) {
			ForwardSpeed = std::lerp(ForwardSpeed, 10.0, FT);
			Tilt.x = std::lerp(Tilt.x, 15.0, FT);
		}
		if (MoveBackward) {
			ForwardSpeed = std::lerp(ForwardSpeed, -10.0, FT);
			Tilt.x = std::lerp(Tilt.x, -15.0, FT);
		}
		if (MoveRight) {
			StrafeSpeed = std::lerp(StrafeSpeed, 8.0, FT);
			Tilt.z = std::lerp(Tilt.z, -15.0, FT);
		}
		if (MoveLeft) {
			StrafeSpeed = std::lerp(StrafeSpeed, -8.0, FT);
			Tilt.z = std::lerp(Tilt.z, 15.0, FT);
		}


		// 두 키를 동시에 누르거나 둘 다 누르지 않으면 속도를 감소시킨다.
		if ((!MoveForward && !MoveBackward) || (MoveForward && MoveBackward)) {
			ForwardSpeed = std::lerp(ForwardSpeed, 0.0, FT);
			Tilt.x = std::lerp(Tilt.x, 0.0, FT);
		}

		if ((MoveRight && MoveLeft) || (!MoveRight && !MoveLeft)) {
			StrafeSpeed = std::lerp(StrafeSpeed, 0.0, FT);
			Tilt.z = std::lerp(Tilt.z, 0.0, FT);
		}
		
		// 이동
		Position.x += sin(XMConvertToRadians(HeliRotation.y)) * ForwardSpeed * FT;
		Position.z += cos(XMConvertToRadians(HeliRotation.y)) * ForwardSpeed * FT;
		Position.x += cos(XMConvertToRadians(HeliRotation.y)) * StrafeSpeed * FT;
		Position.z -= sin(XMConvertToRadians(HeliRotation.y)) * StrafeSpeed * FT;

		// 이동 범위 제한, 맵 밖으로 나갈 수 없다.
		if (Position.x > 95.0)
			Position.x = 95.0;
		else if (Position.x < -95.0)
			Position.x = -95.0;

		if (Position.z > 95.0)
			Position.z = 95.0;
		else if (Position.z < -95.0)
			Position.z = -95.0;

		// 헬리콥터 부드러운 회전
		HeliRotation.x = std::lerp(HeliRotation.x, DestRotation.x, FT);
		HeliRotation.y = std::lerp(HeliRotation.y, DestRotation.y, FT);
	}

	void Render(CommandList CmdList) {
		// 헬기 몸통
		InitMatrix(CmdList, RenderType::Pers);

		Transform::Scale(ScaleMatrix, 0.5, 0.5, 0.5);
		Transform::Move(TranslateMatrix, Position.x, Position.y, Position.z);
		Transform::Rotate(TranslateMatrix, Tilt.x, HeliRotation.y, Tilt.z);
		Transform::Rotate(TranslateMatrix, HeliRotation.x, 0.0, 0.0);

		FlipTexture(CmdList, false, true);
		BindTexture(CmdList, HelicopterTex);
		UseShader(CmdList, BasicShader);
		RenderMesh(CmdList, HelicopterBodyMesh);

		// 헬기 날개
		// 날개 파츠만의 별도의 변환을 진행한다.
		Transform::Move(TranslateMatrix, 0.0, 2.0, 0.0);
		Transform::Rotate(TranslateMatrix, 0.0, WingRotation, 0.0);
		RenderMesh(CmdList, HelicopterWingMesh);

		// 벡터 업데이트
		Math::UpdateVector(Vec.Up, Vec.Right, Vec.Look, HeliRotation.x, HeliRotation.y, HeliRotation.z);
	}
};