#pragma once
#include "GameObject.h"
#include "LineBrush.h"
#include "CameraUtil.h"

// ũ�ν����(������)

class Crosshair : public GameObject {
private:
	LineBrush line;

public:
	Crosshair() {
		line.SetColor(1.0, 1.0, 1.0);
	}

	void Render(CommandList CmdList) {
		// ���� ī�޶��϶��� ���
		if (camera.Mode == CamMode::TRACK_MODE) {
			line.Draw(CmdList, -0.4, 0.0, -0.03, 0.0, 0.01);
			line.Draw(CmdList, 0.03, 0.0, 0.4, 0.0, 0.01);
			line.Draw(CmdList, 0.0, -0.03, 0.0, -0.1, 0.01);
			line.Draw(CmdList, 0.0, 0.03, 0.0, 0.1, 0.01);
		}
	}
};