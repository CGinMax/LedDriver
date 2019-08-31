#pragma once
#include <string>
#include <vector>
#include <utility>
#include "imgui.h"
#include "CustomTypes.h"
#include "LedReadVideo.h"
class InstancePageData
{
public:
	InstancePageData();
	~InstancePageData();
public:
	void Init(int wh[2], std::string tn);
	void DisplayCanvas(float canvas_startx, float canvas_starty, float row_dict, float col_dict, float primitive_size, ImDrawList *canvas_draw_list);

public:
	bool bOpen;
	bool bCheckMouse;
	//ÊÇ·ñ½¥±ä
	bool bGradientNone2Fill;
	bool bGradientFill2None;
	bool bModify;
	int nCanvasSize[2];
	float fTime;
	std::string szTabName;
	double nTickTime;
	std::vector<std::pair<LedInt2,unsigned char>> vEffectPoints;
	std::vector<unsigned char> vnCanvas;
	LedReadImage readImage;
};

