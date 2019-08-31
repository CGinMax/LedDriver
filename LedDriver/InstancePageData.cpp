#include "InstancePageData.h"
#include <algorithm>
InstancePageData::InstancePageData()
{
	bOpen = true;
	bCheckMouse = false;
	bGradientNone2Fill = bGradientFill2None = false;
	bModify = false;
	fTime = 0.0f;
	nTickTime = 0.0;
	
}


InstancePageData::~InstancePageData()
{
	
}

void InstancePageData::Init(int wh[2], std::string tn)
{
	nCanvasSize[0] = wh[0];
	nCanvasSize[1] = wh[1];
	szTabName = tn;
	vnCanvas.resize(nCanvasSize[0] * nCanvasSize[1]);
	std::fill(vnCanvas.begin(), vnCanvas.end(), 0);
}

void InstancePageData::DisplayCanvas(float canvas_startx, float canvas_starty, float row_dict, float col_dict, float primitive_size, ImDrawList *canvas_draw_list)
{
	for (int nrow = 0; nrow < nCanvasSize[1]; nrow++) {
		for (int ncol = 0; ncol < nCanvasSize[0]; ncol++) {
			if (vnCanvas[nrow*nCanvasSize[0] + ncol] <= 10) {
				canvas_draw_list->AddCircle(ImVec2(canvas_startx + ncol * row_dict, canvas_starty + nrow * col_dict),
					primitive_size*0.5f, IM_COL32_WHITE, 32, 2.0f);
			}
			else {
				canvas_draw_list->AddCircleFilled(ImVec2(canvas_startx + ncol * row_dict, canvas_starty + nrow * col_dict),
					primitive_size*0.5f, IM_COL32(255, 255, 255, vnCanvas[nrow*nCanvasSize[0] + ncol]), 32);
			}
		}
	}
}
