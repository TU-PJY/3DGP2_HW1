#pragma once
#include "GameObject.h"
#include "CameraUtil.h"
#include "TransformUtil.h"
#include "TerrainUtil.h"


class Terrain : public GameObject {
private:
	OOBB oobb;

public:
	OOBB GetOOBB() {
		return oobb;
	}

	Terrain() {
		// �ͷ��� ������ �ͷ��� �浹ó�� ��ƿ�� �����Ѵ�.
		Transform::Move(TranslateMatrix, 0.0, -5.0, 0.0);
		Transform::Scale(ScaleMatrix, 20.0, 30.0, 40.0);
		terrainUtil.InputData(TranslateMatrix, RotateMatrix, ScaleMatrix, TerrainMesh);

		// oobb ������Ʈ
		oobb.Update(TerrainMesh, TranslateMatrix, RotateMatrix, ScaleMatrix);
	}

	void Render(CommandList CmdList) {
		// �ͷ����� ���� ������Ʈ�̹Ƿ� ��� �ʱ�ȭ �� ��ȯ�� �������� �ʴ´�.
		RenderType = RENDER_TYPE_PERS;
		EnableLight(CmdList);
		ObjectAlpha = 1.0f;
		FlipTexture(CmdList, FLIP_TYPE_NONE);
		camera.SetToDefaultMode();
		SetColor(0.0, 0.0, 0.0);

		RenderMesh(CmdList, TerrainMesh, TerrainTex, ObjectShader);
		oobb.Render(CmdList);
	}
};