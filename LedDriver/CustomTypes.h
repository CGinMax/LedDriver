#pragma once
#include <windows.h>
#include <list>
#include <memory>
namespace Led {
	struct Image {
		int width;
		int height;
		int channels;

	};

}
struct LedInt2 {
	int x, y;
	LedInt2() { x = y = 0; }
	LedInt2(int _x, int _y) { x = _x; y = _y; }
	//int  operator[] (size_t idx) const { assert(idx <= 1); return (&x)[idx]; }
	//int& operator[] (size_t idx) { assert(idx <= 1); return (&x)[idx]; }
	bool operator == (const LedInt2 tli) {
		return ((x == tli.x) && (y == tli.y));
	}
};
class CommonData
{
public:
	CommonData();
	~CommonData();
	void Init();

	int whMatrix[2];
	float rowDict;
	float colDict;
	float cicleSize;
	int comboSelect;
	std::list<LedInt2> lineList;
};

