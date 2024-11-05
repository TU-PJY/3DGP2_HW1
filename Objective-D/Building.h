#pragma once
#include "GameObject.h"
#include "CameraUtil.h"

class Building : public GameObject {
private:
	AABB aabb;

public:
	AABB GetAABB() {
		return aabb;
	}

	XMFLOAT4X4 GetMatrix() {
		return ResultMatrix;
	}

	Building() {
		Transform::Scale(ScaleMatrix, 0.04, 0.04, 0.04);
		Transform::Rotate(RotateMatrix, -90.0, 0.0, 0.0);
		Transform::Move(TranslateMatrix, 0.0, 8.0, 0.0);
	}

	void Update(float FT) {
		aabb.Update(XMFLOAT3(0.0, 8.0, 0.0), XMFLOAT3(30.0, 40.0, 30.0));
	}

	void Render(CommandList CmdList) {
		RenderType = RENDER_TYPE_PERS;
		EnableLight(CmdList);
		ObjectAlpha = 1.0f;
		FlipTexture(CmdList, FLIP_TYPE_NONE);
		camera.SetToDefaultMode();
		SetColor(0.0, 0.0, 0.0);

		RenderMesh(CmdList, BuildingMesh, BuildingTex, ObjectShader);
		aabb.Render(CmdList);
	}
};