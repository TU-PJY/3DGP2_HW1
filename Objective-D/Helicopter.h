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
	Vector AvoidVec;
	XMFLOAT3 Position{0.0, 30.0, -80.0};

	// 날개 회전 값
	float WingRotation{};

	// 헬리콘터 몸체 회전 값
	XMFLOAT3 HeliRotation{};
	XMFLOAT3 DestRotation{};

	// 이동 상태 및 이동 속도
	bool MoveForward{}, MoveBackward{}, MoveRight{}, MoveLeft{};
	float ForwardSpeed{};
	float StrafeSpeed{};

	// 상하 이동 상태
	bool MoveUp{}, MoveDown{};
	
	// 헬기 기울어짐
	XMFLOAT3 Tilt{};
	
	OOBB oobb;

	// 회피기동 여부
	bool AvoidState{};

	// 충돌 처리 확인 여부
	bool CheckCollisionState{}; 

	// 벡터 계산 여부
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

			// 관전 모드에서만 동작
			if (camera.CurrentMode() == CamMode::TRACK_MODE) {
				float cxDelta = (float)(mouse.CurrentPosition().x - PrevCursorPos.x) / 5.0f;
				float cyDelta = (float)(mouse.CurrentPosition().y - PrevCursorPos.y) / 5.0f;
				mouse.UpdateMotionPosition(PrevCursorPos);

				// 회피기동 상태에서는 방향을 전환할 수 없다.
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
		// 날개 회전
		WingRotation += FT * 2000;

		// 헬리콥터 회전 각도 제한
		if (DestRotation.x < -50.0)
			DestRotation.x = -50.0;

		if (DestRotation.x > 50.0)
			DestRotation.x = 50.0;

		// 방향에 해당하는 키를 누르면 속도를 음수 또는 양수로 증가
		// 이동 방향으로 몸체를 기울인다.
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

			// 두 키를 동시에 누르거나 둘 다 누르지 않으면 속도를 감소시킨다.
			if ((!MoveForward && !MoveBackward) || (MoveForward && MoveBackward)) {
				ForwardSpeed = std::lerp(ForwardSpeed, 0.0, FT);
				Tilt.x = std::lerp(Tilt.x, 0.0, FT);
			}

			if ((MoveRight && MoveLeft) || (!MoveRight && !MoveLeft)) {
				StrafeSpeed = std::lerp(StrafeSpeed, 0.0, FT);
				Tilt.z = std::lerp(Tilt.z, 0.0, FT);
			}
		}

		// 회피기동 시 좀 더 빠르게 가속한다.
		if (AvoidState) {
			StrafeSpeed = std::lerp(StrafeSpeed, 15.0 * AvoidDir, FT * 2.0);
			ForwardSpeed = std::lerp(ForwardSpeed, 15.0, FT * 2.0);
			Tilt.z = std::lerp(Tilt.z, -15.0 * AvoidDir, FT * 2.0);
			Tilt.x = std::lerp(Tilt.x, 15.0, FT * 2.0);

			// 크로스헤어를 빨간색으로 변경한다.
			if (auto crosshair = scene.Find("crosshair"); crosshair)
				crosshair->SetUnable();
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
		HeliRotation.x = std::lerp(HeliRotation.x, DestRotation.x, FT * 2);
		HeliRotation.y = std::lerp(HeliRotation.y, DestRotation.y, FT * 2);


		// 건물의 aabb와 헬리콥터의 oobb가 충돌하면 헬리콥터가 건물을 바라보는지 검사한다.
		// 만약 바라본다면 회피기동을 실행한다.
		// 건물 중점보다 오른쪽에 있으면 오른쪽으로, 왼쪽에 있으면 왼쪽으로 회피기동한다.
		// 헬리콥터가 건물의 바운드박스를 벗어나기 전까지 회피기동 방향은 바뀌지 않는다.
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
		// 헬기 몸통
		InitMatrix(CmdList, RENDER_TYPE_PERS);

		Transform::Scale(ScaleMatrix, 0.5, 0.5, 0.5);
		Transform::Move(TranslateMatrix, Position.x, Position.y, Position.z);
		Transform::Rotate(TranslateMatrix, Tilt.x, HeliRotation.y, Tilt.z);
		Transform::Rotate(TranslateMatrix, HeliRotation.x, 0.0, 0.0);

		FlipTexture(CmdList, FLIP_TYPE_V);
		BindTexture(CmdList, HelicopterTex);
		RenderMesh(CmdList, HelicopterBodyMesh, HelicopterTex, ObjectShader);

		// 몸통 부분의 oobb만 업데이트
		oobb.Update(HelicopterBodyMesh, TranslateMatrix, RotateMatrix, ScaleMatrix);

		// 헬기 날개
		// 날개 파츠만의 별도의 변환을 진행한다.
		Transform::Move(TranslateMatrix, 0.0, 2.0, 0.0);
		Transform::Rotate(TranslateMatrix, 0.0, WingRotation, 0.0);
		RenderMesh(CmdList, HelicopterWingMesh, HelicopterTex, ObjectShader);

		// 벡터 업데이트
		Math::UpdateVector(Vec, HeliRotation.x, HeliRotation.y, HeliRotation.z);

		// oobb 출력
		oobb.Render(CmdList);
	}
};