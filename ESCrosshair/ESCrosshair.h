#pragma once

#include <rapidjson/rapidjson.h>

#include <Windows.h>
#include <iostream>
#include <fstream>
#include <string>

#include <Shlwapi.h>
#include <shellapi.h>
#include <process.h>

#include <filesystem>

#include "Config.h"


HRESULT SetupAndRegisterWindowClass(WNDCLASSEXA&, HINSTANCE&);
HRESULT GetHWNDCoordinates();
VOID DrawCrosshairs(HDC&);
VOID InitializeGlobals();
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK FindWindowPartialCallback(HWND hWnd, LPARAM lParam);


bool CreateNewConsole(int16_t minLength);
bool RedirectConsoleIO();
bool ReleaseConsole();
void AdjustConsoleBuffer(int16_t minLength);
wstring s2ws(std::string& s);
wstring widen(const std::string& s);