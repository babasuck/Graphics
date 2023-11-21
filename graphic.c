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

int* _interpolate(int i0, int d0, int i1, int d1) {
	int* values = (int*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, (i1 - i0 + 2) * sizeof(int));
	if (i0 == i1) {
		values[0] = d0;
		return values;
	}
	double a = (d1 - d0) / (i1 - i0);
	int d = d0;
	for (int i = i0; i <= i1; i++) {
		values[i - i0] = d;
		d += a;
	}
	return values;
}

void inline _swapPoints(long* x0, long* y0, long* x1, long* y1) {
	int t = *x0;
	*x0 = *x1;
	*x1 = t;
	t = *y0;
	*y0 = *y1;
	*y1 = t;
}

void putLine(Canvas* canvas, long x0, long y0, long x1, long y1) {
	long dx = x1 - x0;
	long dy = y1 - y0;
	if (abs(dx) > abs(dy)) {
		if (x0 > x1) {
			_swapPoints(&x0, &y0, &x1, &y1);
		}
		double a = (double)(y1 - y0) / (double)(x1 - x0);
		double y = y0;
		for (int x = x0; x <= x1; x++) {
			putPixel(canvas, x, y, RGB(0, 0, 0));
			y += a;
		}
	}
	else {
		if (y0 > y1) {
			_swapPoints(&x0, &y0, &x1, &y1);
		}
		double a = (double)(x1 - x0) / (double)(y1 - y0);
		double x = x0;
		for (int y = y0; y <= y1; y++) {
			putPixel(canvas, x, y, RGB(0, 0, 0));
			x += a;
		}
	}
}
