#include <windows.h>
#include <tchar.h>

#include "client_app.h"
#include "client_handler.h"

CefRefPtr<ClientHandler> g_handler;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	switch (message)
	{
	case WM_CREATE: 
		{
			// Create the single static handler class instance
			g_handler = new ClientHandler();
			g_handler->SetMainHwnd(hWnd);

			// Create the child windows used for navigation
			RECT rect;
			GetClientRect(hWnd, &rect);

			CefWindowInfo info;
			CefBrowserSettings settings;

			info.SetAsChild(hWnd, rect);

			// Creat the new child browser window
			CefBrowserHost::CreateBrowser(info, g_handler.get(),
				g_handler->GetStartupURL(), settings, NULL);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;
	case WM_SETFOCUS:
		if (g_handler.get() && g_handler->GetBrowser()) {
			// Pass focus to the browser window
			CefWindowHandle hwnd =
				g_handler->GetBrowser()->GetHost()->GetWindowHandle();
			if (hwnd)
				PostMessage(hwnd, WM_SETFOCUS, wParam, NULL);
		}
		break;
	case WM_SIZE:
		// Minimizing resizes the window to 0x0 which causes our layout to go all
		// screwy, so we just ignore it.
		if (wParam != SIZE_MINIMIZED && g_handler.get() &&
			g_handler->GetBrowser()) {
				CefWindowHandle hwnd =
					g_handler->GetBrowser()->GetHost()->GetWindowHandle();

				if (hwnd) {
					// Resize the browser window and address bar to match the new frame
					// window size
					RECT rect;
					GetClientRect(hWnd, &rect);
					HDWP hdwp = BeginDeferWindowPos(1);
					hdwp = DeferWindowPos(hdwp, hwnd, NULL,
						rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top,
						SWP_NOZORDER);
					EndDeferWindowPos(hdwp);
				}
		}
		break;
	case WM_ERASEBKGND:
		if (g_handler.get() && g_handler->GetBrowser()) {
			CefWindowHandle hwnd =
				g_handler->GetBrowser()->GetHost()->GetWindowHandle();
			if (hwnd) {
				// Dont erase the background if the browser window has been loaded
				// (this avoids flashing)
				return 0;
			}
		}
		break;
	case WM_CLOSE:
		if (g_handler.get() && !g_handler->IsClosing()) {
			CefRefPtr<CefBrowser> browser = g_handler->GetBrowser();
			if (browser.get()) {
				// Notify the browser window that we would like to close it. This
				// will result in a call to ClientHandler::DoClose() if the
				// JavaScript 'onbeforeunload' event handler allows it.
				browser->GetHost()->CloseBrowser(false);

				// Cancel the close.
				return 0;
			}
		}
		// Allow the close.
		break;
	case WM_DESTROY:
		// Quitting CEF is handled in ClientHandler::OnBeforeClose().
		//PostQuitMessage(0);
		break;
	
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

void SetupWindow(HINSTANCE hInstance, int nCmdShow) {
	static int c = 1;
	TCHAR szWindowClass[] = _T("CEFHelloWorld");
	TCHAR szTitle[] = _T("CEF Hello World");
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style          = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc    = WndProc;
	wcex.cbClsExtra     = 0;
	wcex.cbWndExtra     = 0;
	wcex.hInstance      = hInstance;
	wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName   = NULL;
	wcex.lpszClassName  = szWindowClass;
	wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL,
			_T("Call to RegisterClassEx failed!"),
			_T("Win32 Guided Tour"),
			NULL);
	}
	HWND hWnd = CreateWindow(
		szWindowClass,
		szTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		1080, 680,
		NULL,
		NULL,
		hInstance,
		NULL
		);

	if (!hWnd)
	{
		MessageBox(NULL,
			_T("Call to CreateWindow failed!"),
			_T("Win32 Guided Tour"),
			NULL);
	}

	ShowWindow(hWnd, nCmdShow); 
	UpdateWindow(hWnd);
}

ClientApp::ClientApp() { };

int WINAPI WinMain(HINSTANCE hInstance,
				   HINSTANCE hPrevInstance,
				   LPSTR lpCmdLine,
				   int nCmdShow)
{
	CefMainArgs main_args(hInstance);
	CefRefPtr<ClientApp> app(new ClientApp);

	CefSettings settings;
	settings.pack_loading_disabled = true;

	// Initialize CEF.
	CefInitialize(main_args, settings, app.get());

	SetupWindow(hInstance, nCmdShow);

	// Run the CEF message loop. This function will block until the application
	// recieves a WM_QUIT message.
	CefRunMessageLoop();

	// Shut down CEF.
	CefShutdown();
	return 0;
}
