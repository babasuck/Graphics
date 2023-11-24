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

HDC canvas_getDC(Canvas* canvas) {
	return canvas->hdc;
}

RECT canvas_getClientRect(Canvas* canvas) {
	return canvas->clientRect;
}

void drawLine(Canvas* canvas, POINT pt1, POINT pt2, COLORREF color) {
	pt1.x += canvas->clientRect.right / 2;
	pt1.y = canvas->clientRect.bottom / 2 - pt1.y;
	pt2.x += canvas->clientRect.right / 2;
	pt2.y = canvas->clientRect.bottom / 2 - pt2.y;
	HPEN oldPen = SelectObject(canvas->hdc, CreatePen(PS_SOLID, 1, color));
	MoveToEx(canvas->hdc, pt1.x, pt1.y, 0);
	LineTo(canvas->hdc, pt2.x, pt2.y);
	DeleteObject(SelectObject(canvas->hdc, oldPen));
}

void drawTriangle(Canvas* canvas, POINT pt1, POINT pt2, POINT pt3, COLORREF color) {
	drawLine(canvas, pt1, pt2, color);
	drawLine(canvas, pt1, pt3, color);
	drawLine(canvas, pt2, pt3, color);
}