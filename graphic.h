#pragma once
#include <Windows.h>

typedef struct Canvas Canvas;

Canvas* createCanvas(HWND hWnd, HDC hdc);
void updateRect_canvas(Canvas* canvas, RECT* rect);

// Drawing 
COLORREF putPixel(Canvas* canvas, long x, long y, COLORREF color);
void drawLine(Canvas* canvas, POINT pt1, POINT pt2, COLORREF color);
void drawTriangle(Canvas* canvas, POINT pt1, POINT pt2, POINT pt3, COLORREF color);