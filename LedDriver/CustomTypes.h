#pragma once
struct LedInt2 {
	int x, y;
	LedInt2() { x = y = 0; }
	LedInt2(int _x, int _y) { x = _x; y = _y; }
	//int  operator[] (size_t idx) const { assert(idx <= 1); return (&x)[idx]; }
	//int& operator[] (size_t idx) { assert(idx <= 1); return (&x)[idx]; }

};