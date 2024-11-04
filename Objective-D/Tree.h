#pragma once
#include "GameObject.h"
#include "TransformUtil.h"
#include "MathUtil.h"
#include "TerrainUtil.h"

// ���� ������

class Tree : public GameObject {
private:
	Vector Vec{};
	XMFLOAT3 Position{};
	float Rotation{};

public:
	Tree(float X, float Z) {
		Math::InitVector(Vec);
		Position.x = X;
		Position.z = Z;

		// ���� ��ġ������ �ͷ��� ���� ���� ��� �ش� ���̷� �̵��Ѵ�.
		Position.y = terrainUtil.GetFloorHeight(Position.x, Position.z, 0.0);
	}

	void Update(float FT) {
		Rotation = Math::CalcDegree2D(camera.GetPosition().x, camera.GetPosition().z, Position.x, Position.z);
	}

	void Render(CommandList CmdList) {
		InitMatrix(CmdList, RENDER_TYPE_PERS);
		Transform::Move(TranslateMatrix, Position.x, Position.y, Position.z);
		Transform::Scale(ScaleMatrix, 10.0, 10.0, 1.0);

		// ī�޶� �ٶ󺸵��� �Ѵ�.
		Math::LookAt(RotateMatrix, Vec, Position, camera.GetPosition(), camera.GetUpVector());

		FlipTexture(CmdList, FLIP_TYPE_V);
		RenderMesh(CmdList, ImagePannel, TreeTex, ObjectShader);
	}
};