#include "utils.h"

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

void _swap(long* a, long* b) {
	long temp = *a;
	*a = *b;
	*b = temp;
}

void _swapPoints(POINT* pt0, POINT* pt1) {
	int tempX = pt0->x;
	pt0->x = pt1->x;
	pt1->x = tempX;

	int tempY = pt0->y;
	pt0->y = pt1->y;
	pt1->y = tempY;
}