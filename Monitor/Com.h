#pragma once
#include <Windows.h>
#include <iostream>

#include "Utils.h"

class Com
{
public:
	Com(char* buffer);
	~Com();
	void RegisterCom(char* portNo);
	void UnregisterCom();
	BOOL CheckPort();
	BOOL DoGetCommState();
	BOOL DoSetCommState(DWORD baudRate, BYTE byteSize, BYTE stopBits, BYTE parity);
	BOOL SetTimeouts(DWORD readIntervalTimeout, DWORD readTotalTimeoutConstant, DWORD readTotalTimeoutMultiplier, DWORD writeTotalTimeoutConstant, DWORD writeTotalTimeoutMultiplier);
	BOOL SetMask(DWORD mask);
	BOOL WaitComEvent(LPOVERLAPPED pOverlapped);
	void Print(LPOVERLAPPED pOverlapped);

private:
	HANDLE _handle;
	DCB _dcbSerialParams = { 0 };
	COMMTIMEOUTS _timeouts = { 0 };
	char* _buffer;
	DWORD _dwEventMask;
	wchar_t _portNo[20] = { 0 };
	char _readData;
	DWORD _noBytesRead;

};

Com::Com(char* buffer)
{
	_buffer = buffer;
}

Com::~Com()
{
}

void Com::RegisterCom(char* portNo) {
	size_t num = 0;
	mbstowcs_s(&num, _portNo, portNo, strlen(portNo) + 1);
	_handle = CreateFile(_portNo, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
}

void Com::UnregisterCom() {
	CloseHandle(_handle);
}

BOOL Com::CheckPort() {
	return _handle == INVALID_HANDLE_VALUE;
}

BOOL Com::DoGetCommState() {
	_dcbSerialParams.DCBlength = sizeof(_dcbSerialParams);
	return GetCommState(_handle, &_dcbSerialParams);
}

BOOL Com::DoSetCommState(DWORD baudRate, BYTE byteSize, BYTE stopBits, BYTE parity) {
	_dcbSerialParams.BaudRate = baudRate;
	_dcbSerialParams.ByteSize = byteSize;
	_dcbSerialParams.StopBits = stopBits;
	_dcbSerialParams.Parity = parity;
	return SetCommState(_handle, &_dcbSerialParams);

}

BOOL Com::SetTimeouts(DWORD readIntervalTimeout, DWORD readTotalTimeoutConstant, DWORD readTotalTimeoutMultiplier, DWORD writeTotalTimeoutConstant, DWORD writeTotalTimeoutMultiplier) {
	_timeouts.ReadIntervalTimeout = readIntervalTimeout;
	_timeouts.ReadTotalTimeoutConstant = readTotalTimeoutConstant;
	_timeouts.ReadTotalTimeoutMultiplier = readTotalTimeoutMultiplier;
	_timeouts.WriteTotalTimeoutConstant = writeTotalTimeoutConstant;
	_timeouts.WriteTotalTimeoutMultiplier = writeTotalTimeoutMultiplier;
	return SetCommTimeouts(_handle, &_timeouts);
}

BOOL Com::SetMask(DWORD mask) {
	return SetCommMask(_handle, mask);
}

BOOL Com::WaitComEvent(LPOVERLAPPED pOverlapped) {
	return WaitCommEvent(_handle, &_dwEventMask, pOverlapped);
}

void Com::Print(LPOVERLAPPED pOverlapped) {
	unsigned char loop = 0;
	printf_s("\n\n");

	// Not sure if this works
	// Idea: catch Ctrl+C
	SetConsoleCtrlHandler(consoleExitHandler, TRUE);

	while (1) {
		do {
			ReadFile(_handle, _buffer, sizeof(_readData), &_noBytesRead, pOverlapped);
			++loop;
		} while (_noBytesRead > 0);

		--loop;
		int index = 0;
		for (index = 0; index < loop; ++index)
		{
			printf_s("%c", _buffer[index]);
		}
	}
}
