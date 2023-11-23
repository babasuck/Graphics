#include <Windows.h>
#include "graphic.h"

// Global entry for HWND 
HWND globalHWND;
BOOL SIZE_CHANGED = 0;

LRESULT WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
void rendering();

int WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nShowCmd
) {
    WNDCLASSEX wnd = { 0 };
    wnd.cbSize = sizeof(wnd);
    wnd.hInstance = 0;
    wnd.lpszClassName = L"wnd";
    wnd.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wnd.lpfnWndProc = WndProc;
    wnd.hCursor = LoadCursor(0, IDC_ARROW);
    wnd.hIcon = LoadIcon(0, IDI_APPLICATION);
    RegisterClassEx(&wnd);
    globalHWND = CreateWindowEx(0, L"wnd", L"Test", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, 0, 0, 0, 0);
    CreateThread(0, 0, rendering, 0, 0, 0);
    MSG msg = { 0 };
    while (GetMessage(&msg, globalHWND, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}



LRESULT WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    PAINTSTRUCT ps = { 0 };
    HDC hdc;
    switch (msg) {
    case WM_CREATE:
        break;
    case WM_DESTROY:
        ExitProcess(0);
        break;
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        EndPaint(hWnd, &ps);
        break;
    case WM_ERASEBKGND:
        return 1;
    case WM_SIZING:
        SIZE_CHANGED = 1;
        return DefWindowProc(hWnd, msg, wParam, lParam);
    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
}

void renderRoutine(Canvas* canvas) {
    POINT pt2 = { 100, -100 };
    POINT tPt1 = { -100, -100 };
    POINT tPt2 = { 0, 100 };
    drawFillTriangle(canvas, tPt1, tPt2, pt2, RGB(75, 100, 125));
}

void rendering() {
    HDC hdc = GetDC(globalHWND);
    RECT clientRect;
    GetClientRect(globalHWND, &clientRect);

    HDC hdcMem = CreateCompatibleDC(hdc);
    HBITMAP bmpMem = CreateCompatibleBitmap(hdc, clientRect.right, clientRect.bottom);
    SelectObject(hdcMem, bmpMem);
    HBRUSH hbrBkg = CreateSolidBrush(GetSysColor(COLOR_WINDOW));

    Canvas* canvas = createCanvas(globalHWND, hdcMem);

    // Инициализация таймера для FPS
    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);
    LARGE_INTEGER t1, t2;
    double elapsedTime;
    TCHAR fpsString[50];

    BOOL running = TRUE;
    while (running) {
        QueryPerformanceCounter(&t1);

        if (SIZE_CHANGED) {
            GetClientRect(globalHWND, &clientRect);
            updateRect_canvas(canvas, &clientRect);
            DeleteObject(bmpMem);
            bmpMem = CreateCompatibleBitmap(hdc, clientRect.right, clientRect.bottom);
            SelectObject(hdcMem, bmpMem);
            SIZE_CHANGED = FALSE;
        }

        FillRect(hdcMem, &clientRect, hbrBkg);
        renderRoutine(canvas);

        // Вычисление и отображение FPS
        QueryPerformanceCounter(&t2);
        elapsedTime = (t2.QuadPart - t1.QuadPart) * 1000.0 / frequency.QuadPart;
        int frameRate = (int)(1000.0 / elapsedTime + 0.5);
        wsprintf(fpsString, TEXT("FPS: %d"), frameRate);
        SetTextColor(hdcMem, RGB(0, 0, 0));
        SetBkMode(hdcMem, TRANSPARENT);
        TextOut(hdcMem, 10, 10, fpsString, lstrlen(fpsString));

        BitBlt(hdc, 0, 0, clientRect.right, clientRect.bottom, hdcMem, 0, 0, SRCCOPY);
    }

    DeleteObject(hbrBkg);
    DeleteObject(bmpMem);
    DeleteDC(hdcMem);
    ReleaseDC(globalHWND, hdc);
}