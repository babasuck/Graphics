#pragma once
#pragma once
#include <Windows.h>

typedef struct Canvas Canvas;

Canvas* createCanvas(HWND hWnd, HDC hdc);
void updateRect_canvas(Canvas* canvas, RECT* rect);

// Drawing 
COLORREF putPixel(Canvas* canvas, long x, long y, COLORREF color);
void putLine(Canvas* canvas, long x0, long y0, long x1, long y1);