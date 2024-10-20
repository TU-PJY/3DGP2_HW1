#pragma once
#include "GameObject.h"
#include "CameraUtil.h"
#include "ResourceManager.h"
#include "TransformUtil.h"

class Terrain : public GameObject {
private:
	
public:
	void Render(CommandList CmdList) {
		InitMatrix(CmdList, RenderType::Pers);
		Transform::Scale(ScaleMatrix, 10.3, 10.3, 10.3);
		Transform::Rotate(RotateMatrix, -90.0, 180.0, 0.0);
		Transform::Move(TranslateMatrix,  0.0, -5.0, 0.0);
		BindTexture(CmdList, TerrainTex);
		UseShader(CmdList, BasicShader);
		RenderMesh(CmdList, TerrainMesh);
	}
};