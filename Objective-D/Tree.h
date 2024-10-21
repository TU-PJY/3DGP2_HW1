#pragma once
#include "GameObject.h"
#include "ResourceManager.h"
#include "TransformUtil.h"
#include "MathUtil.h"
#include "TerrainUtil.h"

// ���� ������

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

		// ���� ��ġ������ �ͷ��� ���� ���� ��� �ش� ���̷� �̵��Ѵ�.
		Position.y = terrainUtil.GetFloorHeight(Position.x, Position.z, 0.0);
	}

	void Update(float FT) {
		HeliRotation = Math::CalcDegree2D(camera.GetPosition().x, camera.GetPosition().z, Position.x, Position.z);
	}

	void Render(CommandList CmdList) {
		InitMatrix(CmdList, RenderType::Pers);
		Transform::Move(TranslateMatrix, Position.x, Position.y, Position.z);
		Transform::Scale(ScaleMatrix, 10.0, 10.0, 1.0);

		// ī�޶� �ٶ󺸵��� �Ѵ�.
		Math::LookAt(RotateMatrix, Vec.Up, Vec.Look, Vec.Right, camera.GetPosition(), Position, camera.GetUpVector());

		FlipTexture(CmdList, false, true);
		BindTexture(CmdList, TreeTex);
		UseShader(CmdList, BasicShader);
		RenderMesh(CmdList, ImagePannel);
	}
};