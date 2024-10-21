#pragma once
#include "GameObject.h"
#include "TransformUtil.h"
#include "MathUtil.h"
#include "CameraUtil.h"
#include <cmath>

// �︮���� ������Ʈ

class Helicopter : public GameObject {
private:
	Vector Vec;
	XMFLOAT3 Position{0.0, 40.0, 40.0};

	float WingRotation{};

	XMFLOAT3 HeliRotation{};
	XMFLOAT3 DestRotation{};

	XMFLOAT4X4 HeadMatrix{}; // ������ ���� ��ȯ ����� �����Ͽ� ���� ��ȯ���� �����Ѵ�.

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

			// ���� ��忡���� ����
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
		// ���� ȸ��
		WingRotation += FT * 2000;

		// �︮���� ȸ�� ���� ����
		if (DestRotation.x < -50.0)
			DestRotation.x = -50.0;

		if (DestRotation.x > 50.0)
			DestRotation.x = 50.0;

		// ���⿡ �ش��ϴ� Ű�� ������ �ӵ��� ���� �Ǵ� ����� ����
		// �̵� �������� ��ü�� ����δ�.
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


		// �� Ű�� ���ÿ� �����ų� �� �� ������ ������ �ӵ��� ���ҽ�Ų��.
		if ((!MoveForward && !MoveBackward) || (MoveForward && MoveBackward)) {
			ForwardSpeed = std::lerp(ForwardSpeed, 0.0, FT);
			Tilt.x = std::lerp(Tilt.x, 0.0, FT);
		}

		if ((MoveRight && MoveLeft) || (!MoveRight && !MoveLeft)) {
			StrafeSpeed = std::lerp(StrafeSpeed, 0.0, FT);
			Tilt.z = std::lerp(Tilt.z, 0.0, FT);
		}
		
		// �̵�
		Position.x += sin(XMConvertToRadians(HeliRotation.y)) * ForwardSpeed * FT;
		Position.z += cos(XMConvertToRadians(HeliRotation.y)) * ForwardSpeed * FT;
		Position.x += cos(XMConvertToRadians(HeliRotation.y)) * StrafeSpeed * FT;
		Position.z -= sin(XMConvertToRadians(HeliRotation.y)) * StrafeSpeed * FT;

		// �̵� ���� ����, �� ������ ���� �� ����.
		if (Position.x > 95.0)
			Position.x = 95.0;
		else if (Position.x < -95.0)
			Position.x = -95.0;

		if (Position.z > 95.0)
			Position.z = 95.0;
		else if (Position.z < -95.0)
			Position.z = -95.0;

		// �︮���� �ε巯�� ȸ��
		HeliRotation.x = std::lerp(HeliRotation.x, DestRotation.x, FT);
		HeliRotation.y = std::lerp(HeliRotation.y, DestRotation.y, FT);
	}

	void Render(CommandList CmdList) {
		// ��� ����
		InitMatrix(CmdList, RenderType::Pers);

		Transform::Scale(ScaleMatrix, 0.5, 0.5, 0.5);
		Transform::Move(TranslateMatrix, Position.x, Position.y, Position.z);
		Transform::Rotate(TranslateMatrix, Tilt.x, HeliRotation.y, Tilt.z);
		Transform::Rotate(TranslateMatrix, HeliRotation.x, 0.0, 0.0);

		FlipTexture(CmdList, false, true);
		BindTexture(CmdList, HelicopterTex);
		UseShader(CmdList, BasicShader);
		RenderMesh(CmdList, HelicopterBodyMesh);

		// ��� ����
		// ���� �������� ������ ��ȯ�� �����Ѵ�.
		Transform::Move(TranslateMatrix, 0.0, 2.0, 0.0);
		Transform::Rotate(TranslateMatrix, 0.0, WingRotation, 0.0);
		RenderMesh(CmdList, HelicopterWingMesh);

		// ���� ������Ʈ
		Math::UpdateVector(Vec.Up, Vec.Right, Vec.Look, HeliRotation.x, HeliRotation.y, HeliRotation.z);
	}
};