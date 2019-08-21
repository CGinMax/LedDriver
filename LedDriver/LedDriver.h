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
#include "LedManualLayout.h"

class LedDriver
{
public:
	LedDriver();
	~LedDriver();
	void Init();
	void Draw();

	void Clear();
	std::list<LedInt2> GetRoute();
	void FirstSetPaintWindow(ImDrawList *draw_list);
	void SecondSetPaintWindow(ImDrawList *draw_list);
	void ThridSetPaintWindow(ImDrawList *draw_list);
	void InitControlWindow(bool *p_open);
	void ModeSelectWindow(bool *p_open);
	void MouseClickDraw(int pageindex);
	void InitVideo();
	std::string SelectFileNameDialog();
	void SaveDataToFile(unsigned char mod, int frameNumber, int frameSize, int frameTime);
	std::vector<InstancePageData> GetManualPage();
	int GetVertexArea();
	LedReadVideo& GetVideo();

private:
	float draw_area_size;
	float firstx;
	float firsty;
	int vertex_area_size[2];
	
	/* 初始化数据变量*/
	int combo_select;
	bool is_init_open;
	bool is_init_vertex;
	bool is_concern;
	bool is_set_open;
	bool is_start_play;
	bool is_video_play;

	bool is_save;
	bool is_open_serial;
	int nCurrentMode;
	std::list<LedInt2> index_list;
	std::vector<InstancePageData> sPage;
	size_t nIntervalNum;
	int nPageCount;
	int nWhichPage;
	//
	LedReadVideo testVideo;
	std::string saveFileName;

	size_t frameIndex;
	double dbNowTime;

	bool is_show_draw_win;
	bool is_show_init_win;
	bool is_show_mode_win;

	LedManualLayout *manualLayout;

	CRITICAL_SECTION cs;

	void OpenSerialPort();
	void RouteMoveLeftRight(int & x, int & y, bool & d, int incrse, std::list<LedInt2> &my_list);
	void RouteMoveUpDown(int & x, int & y, bool & d, int incrse, std::list<LedInt2> &my_list);
	std::list<LedInt2> MatrixStartTraverseUpDown(int xStart, int yStart, bool isPstDirection);
	std::list<LedInt2> MatrixStartTraverseLeftRight(int xStart, int yStart, bool isPstDirection);
};

