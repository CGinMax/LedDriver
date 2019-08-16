#pragma once
#include <vector>
#include "imgui.h"
class LedManualLayout
{
public:
	~LedManualLayout();

	static LedManualLayout *CreateManualLayout();
	void DrawWindow(bool *p_open);

private:
	LedManualLayout();
	static LedManualLayout *ledmanuallayout;
private:
	bool bAddingLine;
	float fLatticeSize;
	ImVec2 ivRectPoints[2];

};



