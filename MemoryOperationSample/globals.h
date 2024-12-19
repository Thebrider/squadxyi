#pragma once
//#pragma execution_character_set("utf-8")
#include <Windows.h>
#include <TlHelp32.h>
#include <string>
#include <cstdint>
#include <cstdlib>
#include <cstddef>
#include <locale.h>
#include <array>
#include <iostream>

struct _globals
{
	LONG process_id;
	HANDLE handle;
	HWND OverlayWnd;
	HWND TargetWnd;
	int Width, Height;
	const CHAR* lWindowName = "Element";
	uint64_t Base;
	uint64_t GOM;
	uint64_t HardSettings;
	uint64_t GameWorld;
	uint64_t World;
};

extern _globals globals;

