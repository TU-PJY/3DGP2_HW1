#include "HomeMode.h"
#include "MouseUtil.h"

// 시작 화면 모드. 도움말을 표시한다.
#include "Help.h"

void HomeMode::Start() {
	// 도움말 화면에서는 배경을 검정으로 변경
	SetBackgroundColor(0.0, 0.0, 0.0);

	// 도움말 객체 추가
	framework.AddObject(new Help, "help", Layer::L3);

	framework.RegisterKeyController(KeyboardController);
	framework.RegisterMouseController(MouseController);
	framework.RegisterMouseMotionController(MouseMotionController);
	framework.RegisterDestructor(Destructor);
	framework.RegisterModeName("HomeMode");
}

void HomeMode::Destructor() {
	// 여기에 모드 종료 시 필요한 작업 추가 (리소스 메모리 해제 등)
}

void HomeMode::KeyboardController(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
	// esc 누를 시 프로그램 종료
	if (nMessageID == WM_KEYDOWN && wParam == VK_ESCAPE)
		// 프로그램을 종료하는 프레임워크 멤버 함수
		framework.Exit();

	if (auto help = framework.Find("help"); help)
		help->InputKey(hWnd, nMessageID, wParam, lParam);
}

//  마우스 모션을 지정된 객체 포인터로 전달한다
void HomeMode::MouseMotionController(HWND hWnd) {
	// 마우스 좌표를 뷰포트 좌표로 변환한다.
	mouse.UpdateMousePosition(hWnd);
}

// 마우스 버튼 클릭 및 휠 스크롤 이벤트를 지정된 객체 포인터로 전달한다
void HomeMode::MouseController(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
	// 좌클릭, 우클릭 눌림 상태를 업데이트 한다
	mouse.UpdateButtonState(nMessageID);

	if (auto help = framework.Find("help"); help)
		help->InputMouseWheel(hWnd, nMessageID, wParam, lParam);
}