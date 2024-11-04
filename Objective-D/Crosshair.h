#pragma once
#include "GameObject.h"
#include "LineBrush.h"
#include "CameraUtil.h"

// 크로스헤어(조준점)

class Crosshair : public GameObject {
private:
	LineBrush line;

	// 헬리콥터 회피기동 시 사용 불가함을 표시한다.
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

		// 추적 카메라일때만 출력
		if (camera.CurrentMode() == CamMode::TRACK_MODE) {
			line.Draw(CmdList, -0.4, 0.0, -0.03, 0.0, 0.01);
			line.Draw(CmdList, 0.03, 0.0, 0.4, 0.0, 0.01);
			line.Draw(CmdList, 0.0, -0.03, 0.0, -0.1, 0.01);
			line.Draw(CmdList, 0.0, 0.03, 0.0, 0.1, 0.01);
		}

		Unable = false;
	}
};