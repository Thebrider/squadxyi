#include <Windows.h>
#include <iostream>
#include <thread>
#include <string>
#include "PMemHelper.h"
#include "eftstructs.h"
#include "Overlay.h"


MemoryManagement* memio = nullptr;
auto pRender = PRENDER::Instance();
auto gameData = EFTData::Instance();
static std::exception_ptr teptr = nullptr;
//render function
void render_scene()
{
	//auto gameData = EFTData::Instance();
	//auto player = EFTPlayer();
	// clear the window alpha
	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);
	d3ddev->BeginScene();    // begins the 3D scene
	
	static char cTitle[256];
	sprintf_s(cTitle, _xor_("EFT 0.13 \nFrame Rate: %i\nPlayers: %i").operator const char* (), FrameRate(), gameData->playercount /*, player.countS, gameData->pscavcount */ );
	GradientBoxOutlined(40, 70, StringWidth(pFont, cTitle) + 4, 40, 1, D3DCOLOR_RGBA(100, 100, 100, 255), D3DCOLOR_RGBA(50, 50, 50, 255), D3DCOLOR_RGBA(0, 0, 0, 255), true);
	String(42, 72, D3DCOLOR_RGBA(255, 255, 255, 255), pFont, true, cTitle);

	//calculate and and draw esp stuff
	pRender->Render();


	
	Circle(s_width / 2, s_height / 2, 2, 0, 1, true, 32, D3DCOLOR_ARGB(255, 255, 0, 0));

	d3ddev->EndScene();    // ends the 3D scene

	d3ddev->Present(NULL, NULL, NULL, NULL);   // displays the created frame on the screen
}

//set up overlay window
void start_window()
{
	
	RECT rc;
	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),
		0,
		WindowProc,
		0,
		0,
		nullptr,
		LoadIcon(0, IDI_APPLICATION),
		LoadCursor(0, IDC_ARROW),
		nullptr,
		nullptr,
		globals.lWindowName,
		LoadIcon(nullptr, IDI_APPLICATION)
	};
	//(LPCWSTR)
	if (!RegisterClassEx(&wcex))
		return;
	globals.TargetWnd = NULL;
	while (!globals.TargetWnd)
		globals.TargetWnd = FindWindow(("UnityWndClass"), 0);
	if (globals.TargetWnd != NULL)
	{
		GetWindowRect(globals.TargetWnd, &rc);
		s_width = rc.right - rc.left;
		s_height = rc.bottom - rc.top;
	}
	
	globals.OverlayWnd = CreateWindowEx(
		0x80,
		globals.lWindowName,
		globals.lWindowName,
		WS_POPUP | WS_VISIBLE,
		rc.left,
		rc.top,
		s_width,
		s_height,
		NULL,
		NULL,
		0,
		NULL
	);

	const MARGINS Margin = { 0, 0,s_width, s_height };

	SetLayeredWindowAttributes(globals.OverlayWnd, RGB(0, 0, 0), 255, ULW_COLORKEY | LWA_ALPHA);
	//SetLayeredWindowAttributes(globals.OverlayWnd, 0, 255, LWA_ALPHA);
	//SetLayeredWindowAttributes(globals.OverlayWnd, RGB(0, 0, 0), 0, ULW_COLORKEY);
	DwmExtendFrameIntoClientArea(globals.OverlayWnd, &Margin);
	SetWindowLong(globals.OverlayWnd, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TRANSPARENT);

	// Activate transparency on color black.
	

	ShowWindow(globals.OverlayWnd, SW_SHOW);
	initD3D(globals.OverlayWnd);
}

uint32_t EntryMode()
{

	MSG msg;
	//RECT rc;
	std::thread LGB([]
		{
			while (true)
			{
				try {
					gameData->InitOffsets();
				}
				catch (exception e) {
					teptr = std::current_exception();
					continue;
					
				}

			}
		});
	LGB.detach();

	std::thread W2S([]
		{	
			int counter = 0;
			while (true) {
				try {
					Sleep(7);
					counter++;
					gameData->GetMatrix(counter);
				}
				catch (exception e) {
					teptr = std::current_exception();
					continue;
					
				}
			}
		});
	W2S.detach();

	std::thread Read([] 
	{
		while (true) 
		{
			try {
				gameData->Read();
			}
			catch (exception e) {
				teptr = std::current_exception();
				continue;
			
			}
			
				
		}
	});	
	Read.detach();

	std::thread items([]
		{
			
			while (true) {
						
				try {
					gameData->Getitem();
				}
				catch (exception e) {
					teptr = std::current_exception();
					continue;
					
				}

			}
			
		});
	items.detach();



	

	while (TRUE)
	{
		ZeroMemory(&msg, sizeof(MSG));
		if (PeekMessage(&msg, globals.OverlayWnd, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		if (msg.message == WM_QUIT || GetAsyncKeyState(VK_HOME)) {
	
			exit(0);
		}


		SetWindowPos(globals.TargetWnd, globals.OverlayWnd, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);

		UpdateWindow(globals.OverlayWnd);

		
		
		//render your esp
		render_scene();
		
		
	}

	DestroyWindow(globals.OverlayWnd);
	return msg.wParam;
}


int main()
{
	AllocConsole();
	setlocale(LC_ALL, "");
	/*printf(_xor_("Welcome , press enter to initialize systems"));
	getchar();*/

	//printf(_xor_("systems up.. press enter in game"));
	//getchar();

	FreeConsole();

	memio = new MemoryManagement();
	memio->Init();

	start_window();

	
	uint32_t entry = (uint32_t)EntryMode();
	

	if (teptr) {
		try {
			std::rethrow_exception(teptr);
		}
		catch (const std::exception& ex)
		{
			std::cerr << "Thread exited with exception: " << ex.what() << "\n";
		}
	}
	getchar();
	return entry;
}


