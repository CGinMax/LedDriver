#pragma once
#include <set>
#include <string>
#include <vector>
#include "imgui.h"
#include "CustomTypes.h"
class InstancePageData
{
public:
	InstancePageData();
	~InstancePageData();

public:
	bool bOpen;
	bool bCheckMouse;
	//ÊÇ·ñ½¥±ä
	bool bGradientNone2Fill;
	bool bGradientFill2None;
	int nCanvasSize[2];
	float fTime;
	std::string szTabName;
	double nTickTime;
	std::vector<LedInt2> vEffectPoints;
	std::vector<int> vnCanvas;
	void Init(int wh[2], std::string tn);
	void DisplayCanvas(float canvas_startx, float canvas_starty, float primitive_size, ImDrawList *canvas_draw_list);
};

