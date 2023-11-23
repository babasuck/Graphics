#include "graphic.h"
#include <math.h>
#include "utils.h"

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

void drawFillTriangle(Canvas* canvas, POINT pt1, POINT pt2, POINT pt3, COLORREF color) {
	if (pt2.y < pt1.y) _swapPoints(&pt2, &pt1);
	if (pt3.y < pt1.y) _swapPoints(&pt3, &pt1);
	if (pt3.y < pt2.y) _swapPoints(&pt3, &pt2);

	drawLine(canvas, pt1, pt2, color);
	drawLine(canvas, pt1, pt3, color);
	drawLine(canvas, pt2, pt3, color);

	for (int y = pt1.y; y <= pt3.y; y++) {
		BOOL second_half = y > pt2.y || pt2.y == pt1.y;
		int segment_height = second_half ? pt3.y - pt2.y : pt2.y - pt1.y;
		float alpha = (float)(y - pt1.y) / (pt3.y - pt1.y);
		float beta = (float)(y - (second_half ? pt2.y : pt1.y)) / segment_height;
		int ax = pt1.x + (pt3.x - pt1.x) * alpha;
		int bx = second_half ? pt2.x + (pt3.x - pt2.x) * beta : pt1.x + (pt2.x - pt1.x) * beta;

		if (ax > bx) _swap(&ax, &bx);
		for (int j = ax; j <= bx; j++) {
			putPixel(canvas, j, y, color);
		}
	}
}