#pragma once
#include <vector>
#include <deque>
#include <list>
#include "CustomTypes.h"
#include "LanguageSetting.h"

class LedManualLayout
{
public:
	~LedManualLayout();

	static LedManualLayout *CreateManualLayout();
	void DrawWindow(bool *p_open);
	void SetCoordinate(int area[2], float cicle_size, float row_dict, float col_dict, std::list<LedInt2> tlist);
	std::list<LedInt2> GetLineDirection();
	bool IsInitCanvas();
	bool IsSettingDone();
	void SetStatus(bool status);
private:
	LedManualLayout();
	static LedManualLayout *ledmanuallayout;
private:
	bool bAddingLine;
	bool bInitCanvas;
	bool bCheckLine;
	bool bSettingDone;
	float fLatticeSize;
	int area[2];
	float rowDict;
	float colDict;
	LedInt2 liRectPoints[2];
	std::list<LedInt2> lCoordinate;
	LanguageSetting *lml_language;
};



