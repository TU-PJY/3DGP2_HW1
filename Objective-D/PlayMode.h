#pragma once
#pragma once
#include "FrameworkUtil.h"

namespace PlayMode {
	void Start();
	void Destructor();
	void KeyboardController(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	void MouseMotionController(HWND hWnd);
	void MouseController(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
}