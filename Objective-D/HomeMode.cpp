#include "HomeMode.h"
#include "MouseUtil.h"

// ���� ȭ�� ���. ������ ǥ���Ѵ�.
#include "Help.h"

void HomeMode::Start() {
	// ���� ȭ�鿡���� ����� �������� ����
	SetBackgroundColor(0.0, 0.0, 0.0);

	// ���� ��ü �߰�
	framework.AddObject(new Help, "help", Layer::L3);

	framework.RegisterKeyController(KeyboardController);
	framework.RegisterMouseController(MouseController);
	framework.RegisterMouseMotionController(MouseMotionController);
	framework.RegisterDestructor(Destructor);
	framework.RegisterModeName("HomeMode");
}

void HomeMode::Destructor() {
	// ���⿡ ��� ���� �� �ʿ��� �۾� �߰� (���ҽ� �޸� ���� ��)
}

void HomeMode::KeyboardController(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
	// esc ���� �� ���α׷� ����
	if (nMessageID == WM_KEYDOWN && wParam == VK_ESCAPE)
		// ���α׷��� �����ϴ� �����ӿ�ũ ��� �Լ�
		framework.Exit();
}

//  ���콺 ����� ������ ��ü �����ͷ� �����Ѵ�
void HomeMode::MouseMotionController(HWND hWnd) {
	// ���콺 ��ǥ�� ����Ʈ ��ǥ�� ��ȯ�Ѵ�.
	mouse.UpdateMousePosition(hWnd);
}

// ���콺 ��ư Ŭ�� �� �� ��ũ�� �̺�Ʈ�� ������ ��ü �����ͷ� �����Ѵ�
void HomeMode::MouseController(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
	// ��Ŭ��, ��Ŭ�� ���� ���¸� ������Ʈ �Ѵ�
	mouse.UpdateButtonState(nMessageID);

	if (auto help = framework.Find("help"); help)
		help->InputMouseWheel(hWnd, nMessageID, wParam, lParam);
}