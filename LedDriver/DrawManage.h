#pragma once
#include "imgui.h"
#include <stack>
struct LedInt2 {
	int x, y;
	LedInt2() { x = y = 0; }
	LedInt2(int _x, int _y) { x = _x; y = _y; }
	int  operator[] (size_t idx) const { assert(idx <= 1); return (&x)[idx]; }
	int& operator[] (size_t idx) { assert(idx <= 1); return (&x)[idx]; }

};
class DrawManage
{
public:
	DrawManage();
	~DrawManage();

public:
	float firstpoint[2];
	int nMatrixArea[2];
	std::stack<LedInt2> GetOrder();
	void SetOrder(int rs);
	std::stack<LedInt2> sOutputOrder;
private:
};

