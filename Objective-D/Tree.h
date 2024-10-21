#pragma once
#include "GameObject.h"
#include "ResourceManager.h"
#include "TransformUtil.h"
#include "MathUtil.h"
#include "TerrainUtil.h"

// 나무 빌보드

class Tree : public GameObject {
private:
	Vector Vec{};
	XMFLOAT3 Position{};
	float HeliRotation{};

public:
	Tree(float X, float Z) {
		Math::InitVector(Vec.Up, Vec.Right, Vec.Look);
		Position.x = X;
		Position.z = Z;

		// 랜덤 위치에서의 터레인 높이 값을 얻어 해당 높이로 이동한다.
		Position.y = terrainUtil.GetFloorHeight(Position.x, Position.z, 0.0);
	}

	void Update(float FT) {
		HeliRotation = Math::CalcDegree2D(camera.GetPosition().x, camera.GetPosition().z, Position.x, Position.z);
	}

	void Render(CommandList CmdList) {
		InitMatrix(CmdList, RenderType::Pers);
		Transform::Move(TranslateMatrix, Position.x, Position.y, Position.z);
		Transform::Scale(ScaleMatrix, 10.0, 10.0, 1.0);

		// 카메라를 바라보도록 한다.
		Math::LookAt(RotateMatrix, Vec.Up, Vec.Look, Vec.Right, camera.GetPosition(), Position, camera.GetUpVector());

		FlipTexture(CmdList, false, true);
		BindTexture(CmdList, TreeTex);
		UseShader(CmdList, BasicShader);
		RenderMesh(CmdList, ImagePannel);
	}
};