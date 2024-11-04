#pragma once
#include "GameObject.h"
#include "LineBrush.h"
#include "CameraUtil.h"

// ũ�ν����(������)

class Crosshair : public GameObject {
private:
	LineBrush line;

	// �︮���� ȸ�Ǳ⵿ �� ��� �Ұ����� ǥ���Ѵ�.
	bool Unable{};

public:
	void SetUnable() {
		Unable = true;
	}

	void Render(CommandList CmdList) {
		if(Unable)
			line.SetColor(1.0, 0.0, 0.0);
		else
			line.SetColor(1.0, 1.0, 1.0);

		// ���� ī�޶��϶��� ���
		if (camera.CurrentMode() == CamMode::TRACK_MODE) {
			line.Draw(CmdList, -0.4, 0.0, -0.03, 0.0, 0.01);
			line.Draw(CmdList, 0.03, 0.0, 0.4, 0.0, 0.01);
			line.Draw(CmdList, 0.0, -0.03, 0.0, -0.1, 0.01);
			line.Draw(CmdList, 0.0, 0.03, 0.0, 0.1, 0.01);
		}

		Unable = false;
	}
};