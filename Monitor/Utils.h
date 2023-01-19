#pragma once
#include <Windows.h>

BOOL WINAPI consoleExitHandler(DWORD signal) {
	if (signal == CTRL_C_EVENT) {
		ExitProcess(0);
	}

	return TRUE;
}