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

void rendering() {
    HDC hdc = GetDC(globalHWND);
    RECT clientRect = { 0 };
    HDC hdcMem = CreateCompatibleDC(hdc);
    HBRUSH hbrBkg = (HBRUSH)(COLOR_WINDOW + 1);
    Canvas* canvas = createCanvas(globalHWND, hdcMem);
    GetClientRect(globalHWND, &clientRect);
    updateRect_canvas(canvas, &clientRect);
    HBITMAP bmpMem = CreateCompatibleBitmap(hdc, clientRect.right, clientRect.bottom);
    DeleteObject(SelectObject(hdcMem, bmpMem));
    while (1) {
        if (SIZE_CHANGED) {
            GetClientRect(globalHWND, &clientRect);
            updateRect_canvas(canvas, &clientRect);
            HBITMAP bmpMem = CreateCompatibleBitmap(hdc, clientRect.right, clientRect.bottom);
            DeleteObject(SelectObject(hdcMem, bmpMem));
            SIZE_CHANGED = 0;
        }
        FillRect(hdcMem, &clientRect, hbrBkg);
        putPixel(canvas, 0, 0, RGB(255, 0, 0));
        POINT pt1 = { 100, 100 };
        POINT pt2 = {100, -100 };
        POINT tPt1 = { -100, -100 };
        POINT tPt2 = { 0, 100 };
        drawTriangle(canvas, tPt1, tPt2, pt2, RGB(255, 0, 0));
        POINT pt_1 = { 0, 0 };
        POINT pt_2 = { 100, -100 };
        drawLine(canvas, pt_1, pt_2, RGB(255, 0, 0));
        BitBlt(hdc, 0, 0, clientRect.right, clientRect.bottom, hdcMem, 0, 0, SRCCOPY);
        DeleteObject(bmpMem);
        Sleep(14);
    }
}