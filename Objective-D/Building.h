#pragma once
#include "GameObject.h"
#include "CameraUtil.h"

class Building : public GameObject {
private:
	AABB aabb;
	AABB aabbInside;

public:
	AABB GetAABB() {
		return aabb;
	}

	AABB GetInsideAABB() {
		return aabbInside;
	}

	Building() {
		Transform::Scale(ScaleMatrix, 0.04, 0.04, 0.04);
		Transform::Rotate(RotateMatrix, -90.0, 0.0, 0.0);
		Transform::Move(TranslateMatrix, 0.0, 8.0, 0.0);
	}

	void Update(float FT) {
		aabb.Update(XMFLOAT3(0.0, 8.0, 0.0), XMFLOAT3(30.0, 40.0, 30.0));

		// 헬리콥터의 레이와 충돌검사하기 위한 안쪽 aabb
		aabbInside.Update(XMFLOAT3(0.0, 8.0, 0.0), XMFLOAT3(10.0, 40.0, 10.0));
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
		aabbInside.Render(CmdList);
	}
};