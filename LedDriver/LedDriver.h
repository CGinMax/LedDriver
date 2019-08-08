#pragma once
#include <assert.h>
#include <string>
#include <stack>
#include <vector>
#include <utility>
#include <set>
#include "imgui.h"
#include "CustomTypes.h"
#include "InstancePageData.h"
#include "LedReadPhoto.h"
#include "LedReadVideo.h"
//#include "DrawManage.h"


using _StackPair = std::pair<std::stack<ImVec2>, std::stack<LedInt2>>;
class LedDriver
{
public:
	LedDriver();
	~LedDriver();
	void Init();
	void Draw();

	void InitVertex();
	void Clear();
	std::stack<LedInt2> GetRoute();
	void FirstSetPaintWindow(ImDrawList *draw_list);
	void SecondSetPaintWindow(ImDrawList *draw_list);
	void ThridSetPaintWindow(ImDrawList *draw_list);
	void InitControlWindow();
	void ModeSelectWindow(ImDrawList *dl);
	std::string SelectFileNameDialog();
	std::string SaveDataToFile();
	//std::vector<int> central_points_pos;
	std::stack<LedInt2> index_stack;
	const float draw_area_size = 16.0f;

private:
	float firstx;
	float firsty;
	int vertex_area_size[2];
	//DrawManage dmLed;
	/* 初始化数据变量*/
	int radio_select;
	bool is_init_open;
	bool is_init_vertex;
	bool is_concern;
	bool is_set_open;
	int current_mode;
	bool is_start_play;
	bool is_video_play;
	ImDrawList *second_dlist = nullptr;
	std::vector<InstancePageData> sPage;
	size_t nIntervalNum;
	int nPageCount;

	std::string videoFile;
	//LedReadPhoto testImage;
	LedReadVideo testVideo;

	void MouseClickDraw(int pageindex);
	
	void RouteMoveLeftRight(int & x, int & y, bool & d, int incrse, std::stack<LedInt2> &my_stack);
	void RouteMoveUpDown(int & x, int & y, bool & d, int incrse, std::stack<LedInt2> &my_stack);
	std::stack<LedInt2> MatrixStartTraverseUpDown(int xStart, int yStart, bool isPstDirection);
	std::stack<LedInt2> MatrixStartTraverseLeftRight(int xStart, int yStart, bool isPstDirection);
};

