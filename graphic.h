#pragma once
#include <Windows.h>

typedef struct Canvas Canvas;

Canvas* createCanvas(HWND hWnd, HDC hdc);
void updateRect_canvas(Canvas* canvas, RECT* rect);

// Gets
HDC canvas_getDC(Canvas* canvas);
RECT canvas_getClientRect(Canvas* canvas);
// Drawing 
void drawLine(Canvas* canvas, POINT pt1, POINT pt2, COLORREF color);
void drawTriangle(Canvas* canvas, POINT pt1, POINT pt2, POINT pt3, COLORREF color);