#include "graphic.h"
#include <math.h>


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

void _swapPoints(long* x0, long* y0, long* x1, long* y1) {
	int t = *x0;
	*x0 = *x1;
	*x1 = t;
	t = *y0;
	*y0 = *y1;
	*y1 = t;
}

// interpolate alghoritm
void __drawLine(Canvas* canvas, POINT pt1, POINT pt2, COLORREF color) {
	long x0 = pt1.x;
	long y0 = pt1.y;
	long x1 = pt2.x;
	long y1 = pt2.y;
	double dx = x1 - x0;
	double dy = y1 - y0;
	if (abs(dx) > abs(dy)) {
		if (x0 > x1) {
			_swapPoints(&x0, &y0, &x1, &y1);
		}
		double a = dy / dx;
		double y = y0;
		for (int x = x0; x <= x1; x++) {
			putPixel(canvas, x, y, color);
			y += a;
		}
	}
	else {
		if (y0 > y1) {
			_swapPoints(&x0, &y0, &x1, &y1);
		}
		double a = dx / dy;
		double x = x0;
		for (int y = y0; y <= y1; y++) {
			putPixel(canvas, x, y, color);
			x += a;
		}
	}
}

// Bresenham algorithm 
void drawLine(Canvas* canvas, POINT pt1, POINT pt2, COLORREF color) {
	long dx = abs(pt2.x - pt1.x), sx = pt1.x < pt2.x ? 1 : -1;
	long dy = -abs(pt2.y - pt1.y), sy = pt1.y < pt2.y ? 1 : -1;
	long error = dx + dy, e2;

	while (1) {
		putPixel(canvas, pt1.x, pt1.y, color);
		if (pt1.x == pt2.x && pt1.y == pt2.y) break;
		e2 = 2 * error;
		if (e2 >= dy) { error += dy; pt1.x += sx; }
		if (e2 <= dx) { error += dx; pt1.y += sy; }
	}
}

void drawTriangle(Canvas* canvas, POINT pt1, POINT pt2, POINT pt3, COLORREF color) {
	drawLine(canvas, pt1, pt2, color);
	drawLine(canvas, pt1, pt3, color);
	drawLine(canvas, pt2, pt3, color);
}