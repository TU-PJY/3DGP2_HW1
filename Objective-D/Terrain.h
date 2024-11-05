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
		// 터레인 정보를 터레인 충돌처리 유틸에 전달한다.
		Transform::Move(TranslateMatrix, 0.0, -5.0, 0.0);
		Transform::Scale(ScaleMatrix, 20.0, 30.0, 40.0);
		terrainUtil.InputData(TranslateMatrix, RotateMatrix, ScaleMatrix, TerrainMesh);

		// oobb 업데이트
		oobb.Update(TerrainMesh, TranslateMatrix, RotateMatrix, ScaleMatrix);
	}

	void Render(CommandList CmdList) {
		// 터레인은 정적 오브젝트이므로 행렬 초기화 및 변환을 실행하지 않는다.
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