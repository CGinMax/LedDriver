#pragma once
#include <assert.h>
#include <string>
#include <stack>
#include <vector>
#include <list>
#include <set>
#include <mutex>
#include "imgui.h"
#include "CustomTypes.h"
#include "LedReadPhoto.h"
#include "LedReadVideo.h"
#include "LedManualLayout.h"
#include "ControlMode.h"
#include "DrawManage.h"
#include "LedProject.h"
#include "LanguageSetting.h"

class LedDriver
{
public:
	LedDriver();
	LedDriver(std::shared_ptr<CommonData> pData);
	~LedDriver();
	void Draw();
	void MenuBarControl();
	void FirstSetPaintWindow(ImDrawList *draw_list);
	void SecondSetPaintWindow(ImDrawList *draw_list);
	void ThridSetPaintWindow(ImDrawList *draw_list);
	void ThreadSaveManual();
	void ThreadSaveVideo();
	void DrawBackgroundImage(ImDrawList *draw_list, ImVec2 &left_up, ImVec2 &right_down);
	void InitLineImage();

private:
	//float draw_area_size;
	float firstx;
	float firsty;
	
	/* 初始化数据变量*/
	bool is_new_project;
	bool is_open_project;
	bool is_save_project;
	bool is_save;
	bool is_start_project;
	bool is_open_serial;
	bool is_english;
	bool is_chinese;
	//
	std::string saveFileName;
	bool is_open_manual;
	bool is_show_init_win;
	bool is_show_mode_win;
	bool is_finishi_image;

	bool is_save_file_done;

	LedManualLayout *manualLayout;
	std::shared_ptr<CommonData> commonData;
	DrawManage *drawInitWindow;
	ControlMode *controlMode;
	LedProject *project;
	LanguageSetting *language;
	std::mutex m_mutex;

	void SaveDataToFile(unsigned char mod, int frameNumber, int frameSize, int frameTime);
	void OpenSerialPort();
};

