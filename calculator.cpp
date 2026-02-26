#include <windows.h>
#include <cstring>
#include <cstdio>

HWND hEdit1, hEdit2;
HBRUSH hCyanBrush;

/* This is where all the input to the window goes to */
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	switch(Message) {

		case WM_CREATE:
		{
			hCyanBrush = CreateSolidBrush(RGB(0, 255, 255));

			CreateWindow("STATIC",
				"Please input two number",
				WS_VISIBLE | WS_CHILD | SS_CENTER,
				50, 20, 160, 25,
				hwnd, NULL,
				((LPCREATESTRUCT)lParam)->hInstance,
				NULL);

			hEdit1 = CreateWindow("EDIT", "",
				WS_VISIBLE | WS_CHILD | WS_BORDER | ES_LEFT,
				50, 50, 150, 25,
				hwnd, (HMENU)1,
				((LPCREATESTRUCT)lParam)->hInstance,
				NULL);

			hEdit2 = CreateWindow("EDIT", "",
				WS_VISIBLE | WS_CHILD | WS_BORDER | ES_LEFT,
				  50, 80, 150, 25,
				hwnd, (HMENU)2,
				((LPCREATESTRUCT)lParam)->hInstance,
				NULL);

			CreateWindow("BUTTON", "+",
				WS_VISIBLE | WS_CHILD,
				40, 120, 40, 30,
				hwnd, (HMENU)10,
				((LPCREATESTRUCT)lParam)->hInstance, NULL);

			CreateWindow("BUTTON", "-",
				WS_VISIBLE | WS_CHILD,
				80, 120, 40, 30,
				hwnd, (HMENU)11,
				((LPCREATESTRUCT)lParam)->hInstance, NULL);

			CreateWindow("BUTTON", "*",
				WS_VISIBLE | WS_CHILD,
				120, 120, 40, 30,
				hwnd, (HMENU)12,
				((LPCREATESTRUCT)lParam)->hInstance, NULL);

			CreateWindow("BUTTON", "/",
				WS_VISIBLE | WS_CHILD,
				160, 120, 40, 30,
				hwnd, (HMENU)13,
				((LPCREATESTRUCT)lParam)->hInstance, NULL);

			break;
		}

		case WM_ERASEBKGND:
		{
			RECT rect;
			GetClientRect(hwnd, &rect);
			FillRect((HDC)wParam, &rect, hCyanBrush);
			return 1;
		}

		case WM_CTLCOLORSTATIC:
		{
			HDC hdc = (HDC)wParam;
			SetBkMode(hdc, TRANSPARENT);
			return (LRESULT)hCyanBrush;
		}

		case WM_COMMAND:
		{
			int id = LOWORD(wParam);

			if (id >= 10 && id <= 13) {

				char buffer1[100];
				char buffer2[100];
				char resultText[100];

				GetWindowText(hEdit1, buffer1, 100);
				GetWindowText(hEdit2, buffer2, 100);

				double num1 = strtod(buffer1, NULL);
				double num2 = strtod(buffer2, NULL);
				double result = 0.0;

				switch (id) {
					case 10: result = num1 + num2; break;
					case 11: result = num1 - num2; break;
					case 12: result = num1 * num2; break;
					case 13:
						if (num2 != 0.0)
							result = num1 / num2;
						else {
							MessageBox(hwnd, "Cannot divide by 0", "Error", MB_OK);
							return 0;
						}
						break;
				}

				snprintf(resultText, sizeof(resultText), "%.15g", result);
				MessageBox(hwnd, resultText, "Result", MB_OK);
			}
			break;
		}
		
		/* Upon destruction, tell the main thread to stop */
		case WM_DESTROY: {
			DeleteObject(hCyanBrush);
			PostQuitMessage(0);
			break;
		}
		
		/* All other messages (a lot of them) are processed using default procedures */
		default:
			return DefWindowProc(hwnd, Message, wParam, lParam);
	}
	return 0;
}

/* The 'main' function of Win32 GUI programs: this is where execution starts */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	WNDCLASSEX wc; /* A properties struct of our window */
	HWND hwnd; /* A 'HANDLE', hence the H, or a pointer to our window */
	MSG msg; /* A temporary location for all messages */

	/* zero out the struct and set the stuff we want to modify */
	memset(&wc,0,sizeof(wc));
	wc.cbSize	 = sizeof(WNDCLASSEX);
	wc.lpfnWndProc	 = WndProc; /* This is where we will send messages to */
	wc.hInstance	 = hInstance;
	wc.hCursor	 = LoadCursor(NULL, IDC_ARROW);
	
	/* White, COLOR_WINDOW is just a #define for a system color, try Ctrl+Clicking it */
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wc.lpszClassName = "WindowClass";
	wc.hIcon	 = LoadIcon(NULL, IDI_APPLICATION); /* Load a standard icon */
	wc.hIconSm	 = LoadIcon(NULL, IDI_APPLICATION); /* use the name "A" to use the project icon */

	if(!RegisterClassEx(&wc)) {
		MessageBox(NULL, "Window Registration Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}

	hwnd = CreateWindowEx(WS_EX_CLIENTEDGE,"WindowClass","Caption",
		WS_VISIBLE | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		250,
		200,
		NULL,NULL,hInstance,NULL);

	if(hwnd == NULL) {
		MessageBox(NULL, "Window Creation Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}

	/*
		This is the heart of our program where all input is processed and 
		sent to WndProc. Note that GetMessage blocks code flow until it receives something, so
		this loop will not produce unreasonably high CPU usage
	*/
	while(GetMessage(&msg, NULL, 0, 0) > 0) { /* If no error is received... */
		TranslateMessage(&msg); /* Translate key codes to chars if present */
		DispatchMessage(&msg); /* Send it to WndProc */
	}
	return msg.wParam;
}