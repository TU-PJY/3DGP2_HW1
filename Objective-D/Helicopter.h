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
	Vector AvoidVec;
	XMFLOAT3 Position{0.0, 30.0, -80.0};

	// ���� ȸ�� ��
	float WingRotation{};

	// �︮���� ��ü ȸ�� ��
	XMFLOAT3 HeliRotation{};
	XMFLOAT3 DestRotation{};

	// �̵� ���� �� �̵� �ӵ�
	bool MoveForward{}, MoveBackward{}, MoveRight{}, MoveLeft{};
	float ForwardSpeed{};
	float StrafeSpeed{};

	// ���� �̵� ����
	bool MoveUp{}, MoveDown{};
	
	// ��� ������
	XMFLOAT3 Tilt{};
	
	OOBB oobb;

	// ȸ�Ǳ⵿ ����
	bool AvoidState{};

	// �浹 ó�� Ȯ�� ����
	bool CheckCollisionState{}; 

	// ���� ��� ����
	bool AvoidCalculated{};
	int AvoidDir{};

public:
	XMFLOAT3 GetUp() { return Vec.Up; }
	XMFLOAT3 GetLook() { return Vec.Look; }
	XMFLOAT3 GetRight() { return Vec.Right; }
	XMFLOAT3 GetPosition() { return Position; }

	Helicopter() {
		Math::InitVector(Vec);
		Math::InitVector(AvoidVec);
	}

	void InputKey(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
		if (camera.CurrentMode() == CamMode::TRACK_MODE) {
			switch (nMessageID) {
			case WM_KEYDOWN:
				switch (wParam) {
				case 'W': MoveForward = true; break;
				case 'S': MoveBackward = true; break;
				case 'A': MoveLeft = true; break;
				case 'D': MoveRight = true; break;
				case 'R': MoveUp = true; break;
				case 'F': MoveDown = true; break;
				}
				break;

			case WM_KEYUP:
				switch (wParam) {
				case 'W': MoveForward = false; break;
				case 'S': MoveBackward = false; break;
				case 'A': MoveLeft = false; break;
				case 'D': MoveRight = false; break;
				case 'R': MoveUp = false; break;
				case 'F': MoveDown = false; break;
				}
			}
		}
	}
	
	void InputMouseMotion(HWND hWnd, POINT PrevCursorPos) {
		if (GetCapture() == hWnd) {
			mouse.HideCursor();

			// ���� ��忡���� ����
			if (camera.CurrentMode() == CamMode::TRACK_MODE) {
				float cxDelta = (float)(mouse.CurrentPosition().x - PrevCursorPos.x) / 5.0f;
				float cyDelta = (float)(mouse.CurrentPosition().y - PrevCursorPos.y) / 5.0f;
				mouse.UpdateMotionPosition(PrevCursorPos);

				// ȸ�Ǳ⵿ ���¿����� ������ ��ȯ�� �� ����.
				if(!AvoidState)
					UpdateMotionRotation(DestRotation, cxDelta, cyDelta);
			}
		}
	}

	void InputMouseButton(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {

	}

	bool IsRightOfTarget(XMFLOAT3& targetPosition) {
		XMFLOAT3 directionToTarget = XMFLOAT3(
			targetPosition.x - Position.x,
			targetPosition.y - Position.y,
			targetPosition.z - Position.z
		);

		XMVECTOR rightVec = XMLoadFloat3(&Vec.Right);
		XMVECTOR directionVec = XMLoadFloat3(&directionToTarget);
		float dotProduct = XMVectorGetX(XMVector3Dot(rightVec, directionVec));

		if (dotProduct < 0)
			return true;

		return false;
	}

	bool IsObjectLookingAtTarget(XMFLOAT3& objectPosition, XMFLOAT3& objectLookAt, XMFLOAT3& targetPosition, float threshold) {
		XMVECTOR objPosVec = XMVectorSet(objectPosition.x, 0.0f, objectPosition.z, 0.0f);
		XMVECTOR objLookAtVec = XMVectorSet(objectLookAt.x, 0.0f, objectLookAt.z, 0.0f);
		XMVECTOR targetPosVec = XMVectorSet(targetPosition.x, 0.0f, targetPosition.z, 0.0f);

		XMVECTOR lookAtDir = XMVector3Normalize(objLookAtVec);
		XMVECTOR toTargetDir = XMVector3Normalize(targetPosVec - objPosVec);
		float dotProduct = XMVectorGetX(XMVector3Dot(lookAtDir, toTargetDir));

		return dotProduct >= threshold;
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
		if (!AvoidState) {
			if (MoveForward) {
				ForwardSpeed = std::lerp(ForwardSpeed, 15.0, FT);
				Tilt.x = std::lerp(Tilt.x, 15.0, FT);
			}
			if (MoveBackward) {
				ForwardSpeed = std::lerp(ForwardSpeed, -15.0, FT);
				Tilt.x = std::lerp(Tilt.x, -15.0, FT);
			}
			if (MoveRight) {
				StrafeSpeed = std::lerp(StrafeSpeed, 15.0, FT);
				Tilt.z = std::lerp(Tilt.z, -15.0, FT);
			}
			if (MoveLeft) {
				StrafeSpeed = std::lerp(StrafeSpeed, -15.0, FT);
				Tilt.z = std::lerp(Tilt.z, 15.0, FT);
			}
			if (MoveUp) {
				Position.y += 10 * FT;
			}
			if (MoveDown) {
				Position.y -= 10 * FT;
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
		}

		// ȸ�Ǳ⵿ �� �� �� ������ �����Ѵ�.
		if (AvoidState) {
			StrafeSpeed = std::lerp(StrafeSpeed, 15.0 * AvoidDir, FT * 2.0);
			ForwardSpeed = std::lerp(ForwardSpeed, 15.0, FT * 2.0);
			Tilt.z = std::lerp(Tilt.z, -15.0 * AvoidDir, FT * 2.0);
			Tilt.x = std::lerp(Tilt.x, 15.0, FT * 2.0);

			// ũ�ν��� ���������� �����Ѵ�.
			if (auto crosshair = scene.Find("crosshair"); crosshair)
				crosshair->SetUnable();
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
		HeliRotation.x = std::lerp(HeliRotation.x, DestRotation.x, FT * 2);
		HeliRotation.y = std::lerp(HeliRotation.y, DestRotation.y, FT * 2);


		// �ǹ��� aabb�� �︮������ oobb�� �浹�ϸ� �︮���Ͱ� �ǹ��� �ٶ󺸴��� �˻��Ѵ�.
		// ���� �ٶ󺻴ٸ� ȸ�Ǳ⵿�� �����Ѵ�.
		// �ǹ� �������� �����ʿ� ������ ����������, ���ʿ� ������ �������� ȸ�Ǳ⵿�Ѵ�.
		// �︮���Ͱ� �ǹ��� �ٿ��ڽ��� ����� ������ ȸ�Ǳ⵿ ������ �ٲ��� �ʴ´�.
		if (auto building = scene.Find("building"); building && CheckCollisionState) {
			if (oobb.CheckCollision(building->GetAABB())) {
				if (!AvoidCalculated && IsObjectLookingAtTarget(Position, Vec.Look, XMFLOAT3(0.0, 0.0, 0.0), 0.95)) {
					if (IsRightOfTarget(XMFLOAT3(0.0, 0.0, 0.0)))
						AvoidDir = 1;
					else
						AvoidDir = -1;

					AvoidCalculated = true;
					AvoidState = true;
				}
			}

			else {
				AvoidState = false;
				AvoidCalculated = false;
			}
		}

		CheckCollisionState = true;
	}

	void Render(CommandList CmdList) {
		// ��� ����
		InitMatrix(CmdList, RENDER_TYPE_PERS);

		Transform::Scale(ScaleMatrix, 0.5, 0.5, 0.5);
		Transform::Move(TranslateMatrix, Position.x, Position.y, Position.z);
		Transform::Rotate(TranslateMatrix, Tilt.x, HeliRotation.y, Tilt.z);
		Transform::Rotate(TranslateMatrix, HeliRotation.x, 0.0, 0.0);

		FlipTexture(CmdList, FLIP_TYPE_V);
		BindTexture(CmdList, HelicopterTex);
		RenderMesh(CmdList, HelicopterBodyMesh, HelicopterTex, ObjectShader);

		// ���� �κ��� oobb�� ������Ʈ
		oobb.Update(HelicopterBodyMesh, TranslateMatrix, RotateMatrix, ScaleMatrix);

		// ��� ����
		// ���� �������� ������ ��ȯ�� �����Ѵ�.
		Transform::Move(TranslateMatrix, 0.0, 2.0, 0.0);
		Transform::Rotate(TranslateMatrix, 0.0, WingRotation, 0.0);
		RenderMesh(CmdList, HelicopterWingMesh, HelicopterTex, ObjectShader);

		// ���� ������Ʈ
		Math::UpdateVector(Vec, HeliRotation.x, HeliRotation.y, HeliRotation.z);

		// oobb ���
		oobb.Render(CmdList);
	}
};