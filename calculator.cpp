#include <windows.h>
#include <cstring>
#include <cstdio>

HWND hEdit1, hEdit2;
HBRUSH hCyanBrush;

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

                sprintf(resultText, "%.15g", result);
                MessageBox(hwnd, resultText, "Result", MB_OK);
            }
            break;
        }

        case WM_DESTROY:
        {
            DeleteObject(hCyanBrush);
            PostQuitMessage(0);
            break;
        }

        default:
            return DefWindowProc(hwnd, Message, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

    WNDCLASSEX wc;
    HWND hwnd;
    MSG msg;

    memset(&wc, 0, sizeof(wc));
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.lpszClassName = "WindowClass";
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if(!RegisterClassEx(&wc)) {
        MessageBox(NULL, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    hwnd = CreateWindowEx(WS_EX_CLIENTEDGE,
        "WindowClass",
        "My Calculator",
        WS_VISIBLE | WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        250, 200,
        NULL, NULL, hInstance, NULL);

    if(hwnd == NULL) {
        MessageBox(NULL, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    while(GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}