// ESCrosshair.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "ESCrosshair.h"
#include <process.h>
//////////////////////////////////////////
//	    ELYSIUMSOFT CROSSHAIRS			//
//		  © ElysiumSoft 2020			//
//			Lewis Youldon				//
//////////////////////////////////////////
//	 Edit values in <InitializeGlobals>	//
//	    to configure for target game	//
//////////////////////////////////////////

using namespace std;

//	GLOBALS
HWND g_hwnd;
HBRUSH g_hbrColourKey;
RECT g_rect;
UINT g_width, g_height;
LPCSTR g_title;
COLORREF g_colour;
INT g_thickness;
UINT g_crosshair_width, g_crosshair_height;
VOID(*pfDraw)(HDC&);
string g_regtitle;

Config g_confman;

wstring widen(const std::string& s)
{
	std::vector<wchar_t> buf(MultiByteToWideChar(CP_ACP, 0, s.c_str(), s.size() + 1, 0, 0));

	MultiByteToWideChar(CP_ACP, 0, s.c_str(), s.size() + 1, &buf[0], buf.size());

	return std::wstring(&buf[0]);
}

wstring s2ws(std::string& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}

void AdjustConsoleBuffer(int16_t minLength)
{
	// Set the screen buffer to be big enough to scroll some text
	CONSOLE_SCREEN_BUFFER_INFO conInfo;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &conInfo);
	if (conInfo.dwSize.Y < minLength)
		conInfo.dwSize.Y = minLength;
	SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), conInfo.dwSize);
}

bool ReleaseConsole()
{
	bool result = true;
	FILE* fp;

	// Just to be safe, redirect standard IO to NUL before releasing.

	// Redirect STDIN to NUL
	if (freopen_s(&fp, "NUL:", "r", stdin) != 0)
		result = false;
	else
		setvbuf(stdin, NULL, _IONBF, 0);

	// Redirect STDOUT to NUL
	if (freopen_s(&fp, "NUL:", "w", stdout) != 0)
		result = false;
	else
		setvbuf(stdout, NULL, _IONBF, 0);

	// Redirect STDERR to NUL
	if (freopen_s(&fp, "NUL:", "w", stderr) != 0)
		result = false;
	else
		setvbuf(stderr, NULL, _IONBF, 0);

	// Detach from console
	if (!FreeConsole())
		result = false;

	return result;
}

bool RedirectConsoleIO()
{
	bool result = true;
	FILE* fp;

	// Redirect STDIN if the console has an input handle
	if (GetStdHandle(STD_INPUT_HANDLE) != INVALID_HANDLE_VALUE)
		if (freopen_s(&fp, "CONIN$", "r", stdin) != 0)
			result = false;
		else
			setvbuf(stdin, NULL, _IONBF, 0);

	// Redirect STDOUT if the console has an output handle
	if (GetStdHandle(STD_OUTPUT_HANDLE) != INVALID_HANDLE_VALUE)
		if (freopen_s(&fp, "CONOUT$", "w", stdout) != 0)
			result = false;
		else
			setvbuf(stdout, NULL, _IONBF, 0);

	// Redirect STDERR if the console has an error handle
	if (GetStdHandle(STD_ERROR_HANDLE) != INVALID_HANDLE_VALUE)
		if (freopen_s(&fp, "CONOUT$", "w", stderr) != 0)
			result = false;
		else
			setvbuf(stderr, NULL, _IONBF, 0);

	// Make C++ standard streams point to console as well.
	ios::sync_with_stdio(true);

	// Clear the error state for each of the C++ standard streams.
	std::wcout.clear();
	std::cout.clear();
	std::wcerr.clear();
	std::cerr.clear();
	std::wcin.clear();
	std::cin.clear();

	return result;
}

bool CreateNewConsole(int16_t minLength)
{
	bool result = false;

	// Release any current console and redirect IO to NUL
	ReleaseConsole();

	// Attempt to create new console
	if (AllocConsole())
	{
		AdjustConsoleBuffer(minLength);
		result = RedirectConsoleIO();
	}

	return result;
}

////////////////////////////////////////////////////////////////////////////////////

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	WNDCLASSEX wc;
	HWND hwnd;
	MSG msg;
	pfDraw = DrawCrosshairs;
	BOOL drawing = true;

	//CreateNewConsole(MAXINT16);

	InitializeGlobals();

	MessageBox(NULL, (g_title), "Selected target:", MB_OK);

	if (FAILED(SetupAndRegisterWindowClass(wc, hInstance)))
		return EXIT_FAILURE;

	if (FAILED(GetHWNDCoordinates()))
		return EXIT_FAILURE;

	hwnd = CreateWindowEx(WS_EX_LAYERED, "ESoft Crosshair", "ESoft Crosshair", WS_POPUP,
		g_rect.left, g_rect.top, g_width, g_height, NULL, NULL, hInstance, NULL);

	if (hwnd == NULL)
	{
		MessageBoxExA(NULL, "Failed to create window", "Error", MB_ICONERROR | MB_OK, LB_GETLOCALE);
		return EXIT_FAILURE;
	}

	SetLayeredWindowAttributes(hwnd, RGB(255, 0, 255), 0, LWA_COLORKEY);

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, NULL, 0, 0) > 0 && !GetAsyncKeyState(VK_INSERT))
	{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (GetAsyncKeyState(VK_F4))
			{
				if (drawing == TRUE)
				{
					SetLayeredWindowAttributes(hwnd, 0, 0, LWA_ALPHA);
					drawing = FALSE;
					Sleep(100);
				}
				else
				{
					SetLayeredWindowAttributes(hwnd, RGB(255, 0, 255), 0, LWA_COLORKEY);
					drawing = TRUE;
					Sleep(100);
				}
			}

			if (GetAsyncKeyState(VK_F5))
			{
				g_confman.ReadConfig();
			}
	}
	return msg.wParam;
}

////////////////////////////////////////////////////////////////////////////////////

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	switch (msg)
	{
	case WM_PAINT:
	{
		hdc = BeginPaint(hwnd, &ps);
		pfDraw(hdc);
		SetWindowPos(g_hwnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		EndPaint(hwnd, &ps);
		break;
	}

	case WM_CLOSE:
	{
		DestroyWindow(hwnd);
		break;
	}

	case WM_DESTROY:
	{
		PostQuitMessage(0);
		break;
	}

	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	return 0;
}

BOOL CALLBACK FindWindowPartialCallback(HWND hWnd, LPARAM lParam)
{
	char caption[256];
	GetWindowText(hWnd, caption, 256);
	char* found = strstr(caption, g_title);
	if (found)
	{
		cout << "Found title: \033[1;33m" << caption << "\033[0m" << endl;
		MessageBox(NULL, caption, "Found window!", MB_ICONINFORMATION | MB_OK);
		string tit = caption;
		tit.shrink_to_fit();
		g_regtitle = tit;
		return FALSE;
	}

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////

VOID DrawCrosshairs(HDC& hdc)
{
	HPEN pen0;
	HPEN pen1 = CreatePen(PS_SOLID, g_thickness, g_colour);
	pen0 = (HPEN)SelectObject(hdc, pen1);

	//	DRAW HORIZONTAL SECTION
	MoveToEx(hdc, ((g_width / 2) - (g_crosshair_width / 2)), (g_height / 2), NULL);
	LineTo(hdc, ((g_width / 2) + (g_crosshair_width / 2)), (g_height / 2));

	//	DRAW VERTICAL SECTION
	MoveToEx(hdc, (g_width / 2), ((g_height / 2) - (g_crosshair_height / 2)), NULL);
	LineTo(hdc, (g_width / 2), ((g_height / 2) + (g_crosshair_height / 2)));
}

HRESULT SetupAndRegisterWindowClass(WNDCLASSEX& wc, HINSTANCE& hInstance)
{
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = 0;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = g_hbrColourKey;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = "ESoft Crosshair";
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&wc))
	{
		MessageBoxA(NULL, "Failed to register window!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		return E_ABORT;
	}

	return S_OK;
}


////////////////////////////////////////////////////////////////////////////////////

HRESULT GetHWNDCoordinates()
{
	BOOL bval = EnumChildWindows(NULL, (WNDENUMPROC)FindWindowPartialCallback, NULL);
	g_hwnd = FindWindowEx(NULL, NULL, NULL, g_regtitle.c_str());
	if (g_hwnd == NULL)
	{
		MessageBoxA(NULL, "Could not find window with specified title, is it open?", "ERROR", MB_ICONERROR | MB_OK);
		return E_HANDLE;
	}

	if (!(GetWindowRect(g_hwnd, &g_rect)))
	{
		MessageBoxA(NULL, "Could not get window dimensions", "ERROR", MB_ICONERROR | MB_OK);
		return E_FAIL;
	}

	g_width = g_rect.right - g_rect.left;
	g_height = g_rect.bottom - g_rect.top;

	return S_OK;
}

////////////////////////////////////////////////////////////////////////////////////

/* VOID InitializeGlobals()
{
	//	TARGET WINDOW NAME
	g_title = L"FiveM";	// Change to target window title!

	//	CROSSHAIR COLOUR
	g_red = 255;
	g_green = 0;
	g_blue = 0;

	//	CROSSHAIR SIZE AND THICKNESS
	g_thickness = 1;		  // Fiddle with
	g_crosshair_width = 27;	  // Values until
	g_crosshair_height = 27;  //    Happy

	//	DEFINES COLOUR TO KEY OUT FOR TRANSPARENCY
	g_hbrColourKey = CreateSolidBrush(RGB(255, 0, 255));
} */


VOID InitializeGlobals()
{
	g_confman = Config();

	cout << "Reading config file..." << endl;

	g_confman.ReadConfig();

	//g_cfg = g_confman.GetConfig();
	g_title = g_confman.GetConfig().target.c_str();
	g_colour = g_confman.GetConfig().colour;
	g_thickness = g_confman.GetConfig().thickness;
	g_crosshair_width = g_confman.GetConfig().width;
	g_crosshair_height = g_confman.GetConfig().height;

	g_hbrColourKey = CreateSolidBrush(RGB(255, 0, 255));
}