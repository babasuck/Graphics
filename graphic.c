#include "graphic.h"

struct Canvas {
	HWND hWnd;
	HDC hdc;
	RECT clientRect;
};

Canvas* createCanvas(HWND hWnd, HDC hdc) {
	Canvas* canvas = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(*canvas));
	canvas->hWnd = hWnd;
	canvas->hdc = hdc;
	GetClientRect(hWnd, &canvas->clientRect);
	return canvas;
}

void updateRect_canvas(Canvas* canvas, RECT* rect) {
	GetClientRect(canvas->hWnd, &canvas->clientRect);
}

COLORREF putPixel(Canvas* canvas, long x, long y, COLORREF color) {
	long s_x = canvas->clientRect.right / 2 + x;
	long s_y = canvas->clientRect.bottom / 2 - y;
	SetPixel(canvas->hdc, s_x, s_y, color);
}

void putLine(Canvas* canvas, long x0, long y0, long x1, long y1) {
	double a = (y1 - y0) / (x1 - x0);
	double b = y0 - a * x0;
	for (int x = x0; x <= x1; x++) {
		int y = a * x + b;
		putPixel(canvas, x, y, RGB(0, 0,0));
	}
}
