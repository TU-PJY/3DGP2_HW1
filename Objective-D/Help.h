#pragma once
#include "GameObject.h"
#include "ResourceManager.h"
#include "TransformUtil.h"
#include <cmath>

#include "PlayMode.h"

// ����
class Help : public GameObject {
private:
	// ���� ��ũ�� ��ġ
	float ScrollY = -1.0;

	// ��ũ�� ��ġ ��ǥ
	float ScrollDest = -1.0;

	// ��ũ�� �ȳ� ȭ��ǥ ��ġ
	float ArrowPosition{};
	float Num{};

	// press to enter �ȳ��� ����
	float EnterAlpha{};
	float AlphaNum{};

public:
	void InputMouseWheel(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
		switch (nMessageID) {
		case WM_MOUSEWHEEL:{
			int ScrollVar = GET_WHEEL_DELTA_WPARAM(wParam);

			// ���� ��ũ��
			if (ScrollVar > 0 && ScrollDest > -1.0)
				ScrollDest -= 0.1;

			// �ƾַ� ��ũ��
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
				// ���� ���� �� �÷��� ���� ��ȯ
				framework.SwitchMode(PlayMode::Start);
				break;
			}

		}
	}

	void Update(float FT) {
		//  lerp�� �ε巯�� ��ũ���� �����Ѵ�
		ScrollY = std::lerp(ScrollY, ScrollDest, FT * 10);

		// ��ũ�� �ȳ� ȭ��ǥ �ִϸ��̼�
		Num += FT * 5;
		ArrowPosition = sin(Num) * 0.1;

		// press to enter �ִϸ��̼�
		AlphaNum += FT * 2;
		EnterAlpha = sin(AlphaNum);
		if (EnterAlpha < 0.0) {
			EnterAlpha = 0.0;
			AlphaNum = 0.0;
		}
	}

	void Render(CommandList CmdList) {
		// ���� ����
		// ��� �ʱ�ȭ
		InitMatrix(CmdList, RenderType::Ortho);

		// �̹��� ���� ����
		SetToImageMode(CmdList);

		// ��ũ�� �̵�
		Transform::Move(TranslateMatrix, 0.0, ScrollY, 0.0);

		// �̹��� ��Ⱦ��� �����ϰ� �Ž��� ��Ⱦ�� �����Ѵ�.
		Transform::ImageAspect(ScaleMatrix, 500, 1100);
		Transform::Scale(ScaleMatrix, 3.0, 3.0, 1.0);

		// �ؽ�ó ���ε�
		BindTexture(CmdList, HelpTex);

		// ���� �˻縦 ��Ȱ��ȭ �� ���̴� ���������� ���
		UseShader(CmdList, BasicShader, false);

		// �̹��� ��¿� �̹��� �г� �Ž� ������
		RenderMesh(CmdList, ImagePannel);

		// ��ũ�� �ȳ� �ؽ�Ʈ
		InitMatrix(CmdList, RenderType::Ortho);
		SetToImageMode(CmdList);
		Transform::Move(TranslateMatrix, 0.7 * ASPECT_RATIO, 0.0, 0.0);
		Transform::Scale(ScaleMatrix, 0.5, 0.5, 1.0);
		BindTexture(CmdList, ScrollHelpTex);
		UseShader(CmdList, BasicShader, false);
		RenderMesh(CmdList, ImagePannel);

		// ��ũ�� �ȳ� ȭ��ǥ(�Ʒ�)
		InitMatrix(CmdList, RenderType::Ortho);
		SetToImageMode(CmdList);
		Transform::Move(TranslateMatrix, 0.7 * ASPECT_RATIO, -0.3 - ArrowPosition, 0.0);
		Transform::Scale(ScaleMatrix, 0.2, 0.2, 1.0);
		BindTexture(CmdList, ArrowTex);
		UseShader(CmdList, BasicShader, false);
		RenderMesh(CmdList, ImagePannel);

		// ��ũ�� �ȳ� ȭ��ǥ(��)
		InitMatrix(CmdList, RenderType::Ortho);
		SetToImageMode(CmdList);
		FlipTexture(CmdList, false, false);
		Transform::Move(TranslateMatrix, 0.7 * ASPECT_RATIO, 0.3 + ArrowPosition, 0.0);
		Transform::Scale(ScaleMatrix, 0.2, 0.2, 1.0);
		BindTexture(CmdList, ArrowTex);
		UseShader(CmdList, BasicShader, false);
		RenderMesh(CmdList, ImagePannel);

		// press to enter
		InitMatrix(CmdList, RenderType::Ortho);
		SetToImageMode(CmdList);
		Transform::Move(TranslateMatrix, 0.0, ScrollY - 1.8, 0.0);
		SetAlpha(CmdList, EnterAlpha);
		BindTexture(CmdList, EnterTex);
		UseShader(CmdList, BasicShader, false);
		RenderMesh(CmdList, ImagePannel);
	}
};