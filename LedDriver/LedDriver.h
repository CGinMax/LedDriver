#pragma once
#include <assert.h>
#include <string>
#include <stack>
#include <vector>
#include <list>
#include <set>
#include "imgui.h"
#include "CustomTypes.h"
#include "InstancePageData.h"
#include "LedReadPhoto.h"
#include "LedReadVideo.h"
//#include "DrawManage.h"



class LedDriver
{
public:
	LedDriver();
	~LedDriver();
	void Init();
	void Draw();

	void Clear();
	std::stack<LedInt2> GetRoute();
	void FirstSetPaintWindow(ImDrawList *draw_list);
	void SecondSetPaintWindow(ImDrawList *draw_list);
	void ThridSetPaintWindow(ImDrawList *draw_list);
	void InitControlWindow();
	void ModeSelectWindow(ImDrawList *dl);
	void MouseClickDraw(int pageindex);
	void InitVideo();
	std::string SelectFileNameDialog();
	void SaveManualDataToFile();
	void SaveVideoDataToFile();
	//std::vector<int> central_points_pos;

private:
	const float draw_area_size = 16.0f;
	float firstx;
	float firsty;
	int vertex_area_size[2];
	
	/* 初始化数据变量*/
	int radio_select;
	bool is_init_open;
	bool is_init_vertex;
	bool is_concern;
	bool is_set_open;
	bool is_start_play;
	bool is_video_play;
	
	std::stack<LedInt2> index_stack;
	std::vector<InstancePageData> sPage;
	size_t nIntervalNum;
	int nPageCount;
	int nCurrentMode;
	//
	LedReadVideo testVideo;
	

	std::string saveFileName;
	void RouteMoveLeftRight(int & x, int & y, bool & d, int incrse, std::stack<LedInt2> &my_stack);
	void RouteMoveUpDown(int & x, int & y, bool & d, int incrse, std::stack<LedInt2> &my_stack);
	std::stack<LedInt2> MatrixStartTraverseUpDown(int xStart, int yStart, bool isPstDirection);
	std::stack<LedInt2> MatrixStartTraverseLeftRight(int xStart, int yStart, bool isPstDirection);
};

