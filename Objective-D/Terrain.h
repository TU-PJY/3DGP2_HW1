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
		Transform::Scale(ScaleMatrix, 0.3, 0.3, 0.3);
		Transform::Move(TranslateMatrix,  0.0, -10.0, 0.0);
		BindTexture(CmdList, TerrainTex);
		UseShader(CmdList, BasicShader);
		RenderMesh(CmdList, TerrainMesh);
	}
};