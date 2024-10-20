#pragma once
#include "GameObject.h"
#include "CameraUtil.h"
#include "ResourceManager.h"
#include "TransformUtil.h"
#include "TerrainUtil.h"


class Terrain : public GameObject {
public:
	Terrain() {
		// �ͷ��� ������ �ͷ��� �浹ó�� ��ƿ�� �����Ѵ�.
		Transform::Move(TranslateMatrix, 0.0, -5.0, 0.0);
		Transform::Scale(ScaleMatrix, 20.0, 30.0, 40.0);
		terrainUtil.InputData(TranslateMatrix, RotateMatrix, ScaleMatrix, TerrainMesh);
	}

	void Render(CommandList CmdList) {
		// �ͷ����� ���� ������Ʈ�̹Ƿ� ��� �ʱ�ȭ �� ��ȯ�� �������� �ʴ´�.
		renderType = RenderType::Pers;
		EnableLight(CmdList);
		AlphaValue = 1.0f;
		FlipTexture(CmdList, false, false);
		camera.SetToDefaultMode();
		SetColor(0.0, 0.0, 0.0);

		BindTexture(CmdList, TerrainTex);
		UseShader(CmdList, BasicShader);
		RenderMesh(CmdList, TerrainMesh);

		terrainUtil.InputData(TranslateMatrix, RotateMatrix, ScaleMatrix, TerrainMesh);
	}
};