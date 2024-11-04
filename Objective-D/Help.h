#pragma once
#include "GameObject.h"
#include "TransformUtil.h"
#include <cmath>

#include "PlayMode.h"

// 도움말
class Help : public GameObject {
private:
	// 실제 스크롤 위치
	float ScrollY = -1.0;

	// 스크롤 위치 목표
	float ScrollDest = -1.0;

	// 스크롤 안내 화살표 위치
	float ArrowPosition{};
	float Num{};

	// press to enter 안내문 투명도
	float EnterAlpha{};
	float AlphaNum{};

public:
	void InputMouse(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
		switch (nMessageID) {
		case WM_MOUSEWHEEL:{
			int ScrollVar = GET_WHEEL_DELTA_WPARAM(wParam);

			// 위로 스크롤
			if (ScrollVar > 0 && ScrollDest > -1.0)
				ScrollDest -= 0.1;

			// 아애로 스크롤
			else if (ScrollVar < 0 && ScrollDest < 1.5)
				ScrollDest += 0.1;
		}
			break;
		}
	}

	void InputKey(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
		switch (nMessageID) {
		case WM_KEYDOWN:
			switch (wParam) {
			case VK_RETURN:
				// 엔터 누를 시 플레이 모드로 전환
				scene.SwitchMode(PlayMode::Start);
				break;
			}

		}
	}

	void Update(float FT) {
		//  lerp로 부드러운 스크롤을 구현한다
		ScrollY = std::lerp(ScrollY, ScrollDest, FT * 10);

		// 스크롤 안내 화살표 애니메이션
		Num += FT * 5;
		ArrowPosition = sin(Num) * 0.1;

		// press to enter 애니메이션
		AlphaNum += FT * 2;
		EnterAlpha = sin(AlphaNum);
		if (EnterAlpha < 0.0) {
			EnterAlpha = 0.0;
			AlphaNum = 0.0;
		}
	}

	void Render(CommandList CmdList) {
		// 도움말 본문
		// 행렬 초기화
		InitMatrix(CmdList, RENDER_TYPE_IMAGE);

		// 스크롤 이동
		Transform::Move(TranslateMatrix, 0.0, ScrollY, 0.0);

		// 이미지 종횡비와 동일하게 매쉬의 종횡비를 조정한다.
		Transform::ImageAspect(ScaleMatrix, 500, 1100);
		Transform::Scale(ScaleMatrix, 3.0, 3.0, 1.0);

		// 이미지 출력용 이미지 패널 매쉬 렌더링
		RenderMesh(CmdList, ImagePannel, HelpTex, ObjectShader);


		// 스크롤 안내 텍스트
		InitMatrix(CmdList, RENDER_TYPE_IMAGE);
		Transform::Move(TranslateMatrix, 0.7 * ASPECT, 0.0, 0.0);
		Transform::Scale(ScaleMatrix, 0.5, 0.5, 1.0);
		RenderMesh(CmdList, ImagePannel, ScrollHelpTex, ObjectShader);

		// 스크롤 안내 화살표(아래)
		InitMatrix(CmdList, RENDER_TYPE_IMAGE);
		Transform::Move(TranslateMatrix, 0.7 * ASPECT, -0.3 - ArrowPosition, 0.0);
		Transform::Scale(ScaleMatrix, 0.2, 0.2, 1.0);
		RenderMesh(CmdList, ImagePannel, ArrowTex, ObjectShader);

		// 스크롤 안내 화살표(위)
		InitMatrix(CmdList, RENDER_TYPE_IMAGE);
		SetToImageMode(CmdList);
		Transform::Move(TranslateMatrix, 0.7 * ASPECT, 0.3 + ArrowPosition, 0.0);
		Transform::Scale(ScaleMatrix, 0.2, 0.2, 1.0);
		Transform::Rotate(RotateMatrix, 180.0, 0.0, 0.0, 1.0);

		RenderMesh(CmdList, ImagePannel, ArrowTex, ObjectShader);

		// press to enter
		InitMatrix(CmdList, RENDER_TYPE_IMAGE);
		Transform::Move(TranslateMatrix, 0.0, ScrollY - 1.8, 0.0);
		RenderMesh(CmdList, ImagePannel, EnterTex, ObjectShader, EnterAlpha);
	}
};