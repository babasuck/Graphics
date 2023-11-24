#include <Windows.h>
#include "graphic.h"

// Global entry for HWND 
HWND globalHWND;
HDC backDC;
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
    static RECT clientRect;
    switch (msg) {
    case WM_CREATE:
        break;
    case WM_DESTROY:
        ExitProcess(0);
        break;
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        //GetClientRect(hWnd, &clientRect);
   
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
    srand((unsigned int)time(NULL)); // Инициализация генератора случайных чисел

    POINT pt0 = { 0, 0 }; // Начальная точка в центре
    POINT ptRandom;       // Случайная точка

    RECT clientRect = canvas_getClientRect(canvas);
    // Рисуем 1000 случайных линий
    for (int i = 0; i < 10; ++i) {
        // Генерируем случайные координаты для конечной точки
        ptRandom.x = rand() % (clientRect.right) - (clientRect.right / 2);
        ptRandom.y = rand() % (clientRect.bottom) - (clientRect.bottom / 2);

        // Выбираем случайный цвет для каждой линии
        COLORREF randomColor = RGB(rand() % 256, rand() % 256, rand() % 256);

        // Рисуем линию
        drawLine(canvas, pt0, ptRandom, randomColor);
    }
}

void rendering() {
    HDC hdc = GetDC(globalHWND);
    RECT clientRect;
    GetClientRect(globalHWND, &clientRect);

    backDC = CreateCompatibleDC(hdc);
    HBITMAP bmpMem = CreateCompatibleBitmap(hdc, clientRect.right, clientRect.bottom);
    DeleteObject(SelectObject(backDC, bmpMem));
    HBRUSH hbrBkg = CreateSolidBrush(GetSysColor(COLOR_WINDOW));
    Canvas* canvas = createCanvas(globalHWND, backDC);


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
            bmpMem = CreateCompatibleBitmap(hdc, clientRect.right, clientRect.bottom);
            DeleteObject(SelectObject(backDC, bmpMem));
            SIZE_CHANGED = 0;
        }

        FillRect(backDC, &clientRect, hbrBkg);
        renderRoutine(canvas);

        // Вычисление и отображение FPS
        QueryPerformanceCounter(&t2);
        elapsedTime = (t2.QuadPart - t1.QuadPart) * 1000.0 / frequency.QuadPart;
        int frameRate = (int)(1000.0 / elapsedTime + 0.5);
        wsprintf(fpsString, TEXT("FPS: %d"), frameRate);
        SetTextColor(backDC, RGB(0, 0, 0));
        SetBkMode(backDC, TRANSPARENT);
        TextOut(backDC, 10, 10, fpsString, lstrlen(fpsString));
        BitBlt(hdc, 0, 0, clientRect.right, clientRect.bottom, backDC, 0, 0, SRCCOPY);
    }

    DeleteObject(hbrBkg);
    DeleteObject(bmpMem);
    DeleteDC(backDC);
    ReleaseDC(globalHWND, hdc);
}