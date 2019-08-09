#include "LedDriver.h"
#include "imgui_internal.h"
#include "LedFileDialog.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <windows.h>
#include <process.h>
#include <exception>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
//#include <ShlObj.h>

void __cdecl ThreadInitVideo(void *param)
{
	LedDriver *pld = (LedDriver*)param;
	pld->InitVideo();
	_endthread();
}

std::string ToUTF8(const std::string str)
{
	int nw_len = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);

	wchar_t* pw_buf = new wchar_t[nw_len + 1];
	memset(pw_buf, 0, nw_len * 2 + 2);

	::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), pw_buf, nw_len);

	int len = WideCharToMultiByte(CP_UTF8, 0, pw_buf, -1, NULL, NULL, NULL, NULL);

	char* utf8_buf = new char[len + 1];
	memset(utf8_buf, 0, len + 1);

	::WideCharToMultiByte(CP_UTF8, 0, pw_buf, nw_len, utf8_buf, len, NULL, NULL);
	//AfxGetApp()->GetMainWnd()->m_hWnd;
	
	std::string outstr(utf8_buf);

	delete[] pw_buf;
	delete[] utf8_buf;

	return outstr;
}
void logfile(std::vector<char> ucharvector)
{
	LedInt2 ledpoint;
	std::fstream output;
	output.open("C:\\Users\\CGinMax\\Documents\\Visual Studio 2017\\Projects\\LedDriver\\Debuglogfile.txt", std::ios::app | std::ios::out);
	
	if (!output) {
		throw std::invalid_argument("不能打开文件");
	}
	else {
		/*for (int i = 0; i<ucharvector.size(); i++)
		output << (int)ucharvector[i];*/
		
	}
	output.close();
}

const ImU32 circle_col = ImColor(ImVec4(1.0f, 1.0f, 1.0f, 1.0f));

LedDriver::LedDriver()
{
	vertex_area_size[0] = 0;
	vertex_area_size[1] = 0;
	is_init_open = true;
	is_init_vertex = false;
	is_concern = false;
	is_init_open = false;
	is_start_play = false;
	is_video_play = false;
	nCurrentMode = 0;
	radio_select = 0;
	nPageCount = 0;
	//this->Init();
}


LedDriver::~LedDriver()
{
	
}

void LedDriver::Init()
{
	if (!sPage.empty()) {
		sPage.clear();
	}		
	InstancePageData new_obj;
	new_obj.Init(vertex_area_size, "Way1");
	sPage.push_back(new_obj);
	
	nPageCount = 1;
	
}

void LedDriver::Draw()
{
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGuiWindowFlags background_dockspace_flag = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	//ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("Background DockSpace", false, background_dockspace_flag);
	//ImGui::PopStyleVar();

	ImGuiID dockspace_id = ImGui::GetID("Main Dockspace");
	if (ImGui::DockBuilderGetNode(dockspace_id) == NULL)
	{
		ImGui::DockBuilderRemoveNode(dockspace_id);
		ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace); 
		ImGui::DockBuilderSetNodeSize(dockspace_id, ImVec2(1280,720));

		ImGuiID dock_main_id = dockspace_id;
		//ImGuiID dock_id_left = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left, 0.20f, NULL, &dock_main_id);
		ImGuiID dock_id_right = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Right, 0.35f, NULL, &dock_main_id);
		ImGuiID dock_id_right_bottom = ImGui::DockBuilderSplitNode(dock_id_right, ImGuiDir_Down, 0.50f, NULL, &dock_id_right);

		ImGui::DockBuilderDockWindow(u8"显示窗口", dock_main_id);
		ImGui::DockBuilderDockWindow(u8"初始化设置", dock_id_right);
		ImGui::DockBuilderDockWindow(u8"点阵操作", dock_id_right_bottom);
		ImGui::DockBuilderFinish(dockspace_id);
	}
	ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);

	
	//ImGui::SetNextWindowSize(ImVec2(1280, 710));
	ImGui::Begin(u8"显示窗口", false);
	ImDrawList *draw_list = ImGui::GetWindowDrawList();
	ImGui::Text(u8"显示区域");
	ImGui::Text("fresh on time:%.3fms", 1000.0f / ImGui::GetIO().Framerate);
	/* Text下面的光标位置*/
	ImVec2 cursorPos = ImGui::GetCursorScreenPos();
	/* 设置第一个点的x，y*/
	firstx = cursorPos.x + draw_area_size * 0.5f + 3.5f;
	firsty = cursorPos.y + draw_area_size * 0.5f + 3.5f;
	/* 设置画布的大小，超过屏幕大小会显示滚动条*/
	ImGui::InvisibleButton("##dummy", ImVec2(cursorPos.x + draw_area_size * vertex_area_size[0], cursorPos.x + draw_area_size * vertex_area_size[1]));

	if (is_init_vertex)
		FirstSetPaintWindow(draw_list);
	
	if (is_start_play)
		SecondSetPaintWindow(draw_list);

	if (is_video_play)
		ThridSetPaintWindow(draw_list);

	
	ImGui::End();//Draw window End

	InitControlWindow();
	ModeSelectWindow(draw_list);
	ImGui::End();//Background DockSpace End
	ImGui::PopStyleVar();
	
}
inline void LedDriver::FirstSetPaintWindow(ImDrawList *draw_list)
{
	/* 绘制初始画布*/
	for (int i = 0; i < vertex_area_size[1]; i++) {
		for (int j = 0; j < vertex_area_size[0]; j++) {
			draw_list->AddCircle(
				ImVec2(firstx + j * draw_area_size, firsty + i * draw_area_size), draw_area_size*0.5f, circle_col, 32, 2.0f);
		}
	}

	/* 绘制数据线*/
	if (radio_select != 0) {
		std::stack<LedInt2> line_points;
		line_points = GetRoute();
		while (!line_points.empty()) {
			LedInt2 line1 = line_points.top();
			line_points.pop();
			LedInt2 line2 = line_points.top();
			draw_list->AddLine(ImVec2(firstx + line1.x*draw_area_size, firsty + line1.y*draw_area_size), ImVec2(firstx + line2.x*draw_area_size, firsty + line2.y*draw_area_size),
				IM_COL32(90, 250, 250, 255), 2.0f);
			if (line_points.size() == 1) {
				line_points.pop();
			}
		}
	}
	
}
void LedDriver::SecondSetPaintWindow(ImDrawList *draw_list)
{
	//遍历每一页的画布
	if (nIntervalNum < sPage.size()) {
		double dbNowTime = ImGui::GetTime() - sPage[nIntervalNum].nTickTime;
		if (dbNowTime < sPage[nIntervalNum].fTime) {
			std::vector<LedInt2>::iterator playIter = sPage[nIntervalNum].vEffectPoints.begin();
			//两种渐变都关闭
			if (!(sPage[nIntervalNum].bGradientNone2Fill || sPage[nIntervalNum].bGradientFill2None))
				for (; playIter != sPage[nIntervalNum].vEffectPoints.end(); playIter++)
					draw_list->AddCircleFilled(ImVec2(firstx + playIter->x * draw_area_size, firsty + playIter->y * draw_area_size), draw_area_size*0.5f, IM_COL32(255, 255, 255, 255), 32);
			//两种渐变都打开
			else if (sPage[nIntervalNum].bGradientNone2Fill&&sPage[nIntervalNum].bGradientFill2None) {
				if (dbNowTime<sPage[nIntervalNum].fTime/2.0f)
					for (; playIter != sPage[nIntervalNum].vEffectPoints.end(); playIter++)
						draw_list->AddCircleFilled(ImVec2(firstx + playIter->x * draw_area_size, firsty + playIter->y * draw_area_size), draw_area_size*0.5f, IM_COL32(255, 255, 255, 255 * (dbNowTime / sPage[nIntervalNum].fTime*2.0f)), 32);
				else 
					for (; playIter != sPage[nIntervalNum].vEffectPoints.end(); playIter++)
						draw_list->AddCircleFilled(ImVec2(firstx + playIter->x * draw_area_size, firsty + playIter->y * draw_area_size), draw_area_size*0.5f, IM_COL32(255, 255, 255, 255 * (1-(dbNowTime / sPage[nIntervalNum].fTime*2.0f-1))), 32);
			}
			//有一种渐变打开
			else {
				//无到有渐变
				if (sPage[nIntervalNum].bGradientNone2Fill)
					for (; playIter != sPage[nIntervalNum].vEffectPoints.end(); playIter++)
						draw_list->AddCircleFilled(ImVec2(firstx + playIter->x * draw_area_size, firsty + playIter->y * draw_area_size), draw_area_size*0.5f, IM_COL32(255, 255, 255, 255 * (dbNowTime / sPage[nIntervalNum].fTime)), 32);
				//有到无渐变
				else if (sPage[nIntervalNum].bGradientFill2None)
					for (; playIter != sPage[nIntervalNum].vEffectPoints.end(); playIter++)
						draw_list->AddCircleFilled(ImVec2(firstx + playIter->x * draw_area_size, firsty + playIter->y * draw_area_size), draw_area_size*0.5f, IM_COL32(255, 255, 255, 255 * (2-dbNowTime / sPage[nIntervalNum].fTime)), 32);
			}
		}
		else {
			nIntervalNum++;
			if (nIntervalNum < sPage.size())
				sPage[nIntervalNum].nTickTime = ImGui::GetTime();
		}
	}
	else {
		nIntervalNum = 0;
		sPage[nIntervalNum].nTickTime = ImGui::GetTime();
	}
	
}

void LedDriver::ThridSetPaintWindow(ImDrawList *draw_list)
{
	static double fNowTime = ImGui::GetTime();

	if (frameiter != testVideo.videoFrameList.end()) {
		std::list<LedInt2> vdrawImage = testVideo.MakePrimitiveInfo(*frameiter, vertex_area_size[0], vertex_area_size[1]);;
		
		for (auto liter = vdrawImage.begin(); liter != vdrawImage.end(); liter++) {
			draw_list->AddCircleFilled(ImVec2(firstx + (*liter).y*draw_area_size, firsty + (*liter).x*draw_area_size), draw_area_size*0.5f, IM_COL32(255, 255, 255, 255), 32);
		}
		//fNowTime += (1000.0f / ImGui::GetIO().Framerate);
		if (ImGui::GetTime() - fNowTime > (testVideo.GetFrameTime() / 1000.0)) {
			frameiter++;
			fNowTime = ImGui::GetTime();
		}
	}
	else {
		frameiter = testVideo.videoFrameList.begin();
	}
	
}

/* 控制显示窗口*/
void LedDriver::InitControlWindow()
{
	static int input_size[2] = { 0,0 };
	if (!ImGui::Begin(u8"初始化设置", false)) {
		ImGui::End();
		return;
	}
	ImGui::InputInt2(u8"长X宽", input_size);
	if (ImGui::Button(u8"绘制点阵")) {
		vertex_area_size[0] = input_size[0];
		vertex_area_size[1] = input_size[1];
		is_init_vertex = true;
		
		is_concern = false;
	}
	ImGui::SameLine();
	if (ImGui::Button(u8"清除点阵")) {
		vertex_area_size[0] = input_size[0] = 0;
		vertex_area_size[1] = input_size[1] = 0;
		is_init_vertex = false;
		is_concern = false;
		radio_select = 0;
		Clear();
	}

	ImGui::Separator();
	//ImGui::Text(u8"方向选择");
	const char* items[] = { u8"无", u8"上左纵向", u8"上左横向", u8"下左纵向", u8"下左横向", u8"上右纵向", u8"上右横向", u8"下右纵向", u8"下右横向" };
	ImGui::Combo(u8"灯布局方向", &radio_select, items, IM_ARRAYSIZE(items));
	/*ImGui::BeginGroup();

	ImGui::BeginGroup();
	ImGui::Button("button1", ImVec2(130, 80));
	ImGui::RadioButton(u8"无", &radio_select, 0);
	ImGui::EndGroup();

	ImGui::SameLine();

	ImGui::BeginGroup();
	ImGui::Button("button2", ImVec2(130, 80));
	ImGui::RadioButton(u8"左上角开始，往下走", &radio_select, 1);
	ImGui::EndGroup();
	ImGui::SameLine();

	ImGui::BeginGroup();
	ImGui::Button("button3", ImVec2(130, 80));
	ImGui::RadioButton(u8"左上角开始，往右走", &radio_select, 2);
	ImGui::EndGroup();

	ImGui::BeginGroup();
	ImGui::Button("button4", ImVec2(130, 80));
	ImGui::RadioButton(u8"左下角开始，往上走", &radio_select, 3);
	ImGui::EndGroup();
	ImGui::SameLine();

	ImGui::BeginGroup();
	ImGui::Button("button5", ImVec2(130, 80));
	ImGui::RadioButton(u8"左下角开始，往右走", &radio_select, 4);
	ImGui::EndGroup();
	ImGui::SameLine();

	ImGui::BeginGroup();
	ImGui::Button("button6", ImVec2(130, 80));
	ImGui::RadioButton(u8"右上角开始，往下走", &radio_select, 5);
	ImGui::EndGroup();

	ImGui::BeginGroup();
	ImGui::Button("button7", ImVec2(130, 80));
	ImGui::RadioButton(u8"右上角开始，往左走", &radio_select, 6);
	ImGui::EndGroup();
	ImGui::SameLine();

	ImGui::BeginGroup();
	ImGui::Button("button8", ImVec2(130, 80));
	ImGui::RadioButton(u8"右下角开始，往上走", &radio_select, 7);
	ImGui::EndGroup();
	ImGui::SameLine();

	ImGui::BeginGroup();
	ImGui::Button("button9", ImVec2(130, 80));
	ImGui::RadioButton(u8"右下角开始，往左走", &radio_select, 8);
	ImGui::EndGroup();

	ImGui::EndGroup();*/

	if (ImGui::Button(u8"绘制确认")) {
		index_stack = GetRoute();
		is_concern = true;
		is_init_vertex = false;
		this->Init();
	}
	ImGui::SameLine();
	if (is_concern)
		ImGui::Text(u8"设置完成");


	ImGui::End();
}

void LedDriver::ModeSelectWindow(ImDrawList *dl)
{
	//is_set_open = ;
	if (ImGui::Begin(u8"点阵操作", false))
	{
		//ImGui::TreeNode(u8"手动选择模式");
		ImGui::Combo(u8"模式选择", &nCurrentMode, u8"手动选择模式\0Video模式\0\0");
		switch (nCurrentMode)
		{
		case 0:
			/*添加一页选项页*/
			if (ImGui::Button(u8"添加")) {
				nPageCount++;
				InstancePageData singlePage;
				char nbuf[8];
				sprintf(nbuf, "Way%d", nPageCount);
				singlePage.Init(vertex_area_size, nbuf);
				sPage.push_back(singlePage);
			}

			ImGui::Indent(20.0f);
			ImGui::Separator();
			//多个区间页
			if (ImGui::BeginTabBar("autoTabBar", ImGuiTabBarFlags_None)) {
				for (size_t i = 0; i < sPage.size(); i++) {
					if (!sPage[i].bOpen) {
						std::vector<InstancePageData>::iterator tIter = sPage.begin() + i;
						sPage.erase(tIter); continue;
					}

					if (ImGui::BeginTabItem(sPage[i].szTabName.c_str(), &(sPage[i].bOpen))) {
						ImGui::Checkbox(u8"选点", &(sPage[i].bCheckMouse)); ImGui::SameLine();
						ImGui::Checkbox(u8"渐变:暗到亮", &(sPage[i].bGradientNone2Fill)); ImGui::SameLine();
						ImGui::Checkbox(u8"渐变:亮到暗", &(sPage[i].bGradientFill2None));

						ImGui::InputFloat(u8"点亮时间", &(sPage[i].fTime));
						if (ImGui::Button(u8"确定")) {
							if (!sPage[i].vEffectPoints.empty())
								sPage[i].vEffectPoints.clear();

							/* 获得值为1的点二维坐标*/
							std::vector<int>::iterator effIter = std::find_if(sPage[i].vnCanvas.begin(), sPage[i].vnCanvas.end(), [](int v) {return (v != 0); });
							while (effIter != sPage[i].vnCanvas.end())
							{
								int dirc = std::distance(sPage[i].vnCanvas.begin(), effIter);
								sPage[i].vEffectPoints.push_back(LedInt2(dirc%vertex_area_size[0], dirc / vertex_area_size[0]));
								effIter = std::find_if(effIter + 1, sPage[i].vnCanvas.end(), [](int v) {return (v != 0); });
							}

						}

						if (!is_start_play)
							sPage[i].DisplayCanvas(firstx, firsty, draw_area_size, dl);
						ImGui::EndTabItem();
					}
					if (sPage[i].bCheckMouse) {
						ImGui::GetForegroundDrawList()->AddCircleFilled(ImGui::GetMousePos(), draw_area_size*0.5f, IM_COL32(255, 255, 255, 255), 32);
						MouseClickDraw(i);
					}
				}
				ImGui::EndTabBar();
			}
			ImGui::Separator();
			ImGui::Unindent(20.0f);

			if (ImGui::Button(u8"演示")) {
				//TODO:添加安全检查

				//初始化播放的第一个时刻的状态
				sPage[0].nTickTime = ImGui::GetTime();
				nIntervalNum = 0;
				is_start_play = true;
			}
			ImGui::SameLine(); if (ImGui::Button(u8"关闭演示")) is_start_play = false; ImGui::SameLine();
			if (ImGui::Button(u8"保存")) {
			} 
			
			break;
		case 1:
			if (ImGui::Button(u8"导入")) {
				videoFile = SelectFileNameDialog();
				//videoFile = std::string("C:\\Users\\CGinMax\\Desktop\\ledimage\\test.mp4");
				//初始化图片
				HANDLE hThread = (HANDLE)_beginthread(ThreadInitVideo, 0, (void*)this);
				//testVideo.Init(videoFile, vertex_area_size);
			}
			ImGui::SameLine();
			ImGui::Text(videoFile.c_str());

			if (ImGui::Button(u8"演示播放")) {
				is_video_play = true;
				frameiter = testVideo.videoFrameList.begin();
			}
			ImGui::SameLine();
			if (ImGui::Button(u8"关闭演示"))
				is_video_play = false;
			ImGui::SameLine();
			if (ImGui::Button(u8"保存")) {
				SaveDataToFile();
			}
			ImGui::SameLine();
			ImGui::Button(u8"打开串口");
			break;
		}

	}
	ImGui::End();//End model control
}


void LedDriver::Clear()
{
	while (!index_stack.empty()) {
		index_stack.pop();
	}
	sPage.clear();
}

/* 获取路径*/
std::stack<LedInt2> LedDriver::GetRoute()
{
	std::stack<LedInt2> route_points;
	
	switch (radio_select)
	{
	case 0:
		break;
	case 1:
		route_points = MatrixStartTraverseUpDown(0, 0, true);
		break;
	case 2:
		route_points = MatrixStartTraverseLeftRight(0, 0, true);
		break;
	case 3:
		route_points = MatrixStartTraverseUpDown(0, vertex_area_size[1]-1, false);
		break;
	case 4:
		route_points = MatrixStartTraverseLeftRight(0, vertex_area_size[1] - 1, true);
		break;
	case 5:
		route_points = MatrixStartTraverseUpDown(vertex_area_size[0]-1, 0, true);
		break;
	case 6:
		route_points = MatrixStartTraverseLeftRight(vertex_area_size[0] - 1, 0, false);
		break;
	case 7:
		route_points = MatrixStartTraverseUpDown(vertex_area_size[0]-1, vertex_area_size[1] - 1, false);
		break;
	case 8:
		route_points = MatrixStartTraverseLeftRight(vertex_area_size[0] - 1, vertex_area_size[1] - 1, false);
		break;
	}

	return route_points;
}

void LedDriver::MouseClickDraw(int pageindex)
{
	if (ImGui::IsMouseClicked(0)) {
		ImVec2 cpoint = ImGui::GetIO().MouseClickedPos[0];
		int cx = (int)((cpoint.x - firstx + draw_area_size * 0.5f) / draw_area_size);
		int cy = (int)((cpoint.y - firsty + draw_area_size * 0.5f) / draw_area_size);
		if (cx < vertex_area_size[0] && cy < vertex_area_size[1])
			sPage[pageindex].vnCanvas[cy*vertex_area_size[0] + cx] = sPage[pageindex].vnCanvas[cy*vertex_area_size[0] + cx] ? 0 : 1;			//central_points_pos[cy*vertex_area_size[0] + cx] = central_points_pos[cy*vertex_area_size[0] + cx] ? 0 : 1;

	}
}

void LedDriver::InitVideo()
{
	testVideo.Init(videoFile, vertex_area_size);
}

std::string LedDriver::SelectFileNameDialog()
{
	std::string filenameUnicode = LedFileDialog::SelectFileNameDialog();
	std::string filenameUtf8 = ToUTF8(filenameUnicode);
	return filenameUtf8;
}

std::string LedDriver::SaveDataToFile()
{
	std::string saveFileName = LedFileDialog::OpenSaveFileDialog();
	return saveFileName;
}



/* 左右方向移动*/
void LedDriver::RouteMoveLeftRight(int & x, int & y, bool & d, int incrse, std::stack<LedInt2> &my_stack)
{
	if (d) {
		if (x < vertex_area_size[0] - 1) {
			my_stack.push(LedInt2(x, y));
			x += 1;
		}
		else {
			my_stack.push(LedInt2(x, y));
			y += incrse;
			d = !d;
		}
	}
	else {
		if (x > 0) {
			my_stack.push(LedInt2(x, y));
			x -= 1;
		}
		else {
			my_stack.push(LedInt2(x, y));
			y += incrse;
			d = !d;
		}
	}
}
/* 上下方向移动*/
void LedDriver::RouteMoveUpDown(int & x, int & y, bool & d, int incrse, std::stack<LedInt2> &my_stack)
{
	if (d) {
		if (y < vertex_area_size[1]-1) {
			my_stack.push(LedInt2(x, y));
			y += 1;
		}
		else {
			my_stack.push(LedInt2(x, y));
			x += incrse;
			d = !d;
		}
	}
	else {
		if (y > 0) {
			my_stack.push(LedInt2(x, y));
			y -= 1;
		}
		else {
			my_stack.push(LedInt2(x, y));
			x += incrse;
			d = !d;
		}
	}
}

std::stack<LedInt2> LedDriver::MatrixStartTraverseUpDown(int xStart, int yStart, bool isPstDirection)
{
	ImDrawList *draw_start = ImGui::GetWindowDrawList();
	draw_start->AddCircleFilled(ImVec2(firstx+xStart*draw_area_size, firsty+yStart*draw_area_size), draw_area_size*0.5f, circle_col);
	std::stack<LedInt2> tmp_stack;
	
	if (!xStart) {
		while (xStart < vertex_area_size[0]) {
			RouteMoveUpDown(xStart, yStart, isPstDirection, 1, tmp_stack);
		}
	}
	else {
		while (xStart >= 0) {
			RouteMoveUpDown(xStart, yStart, isPstDirection, -1, tmp_stack);
		}
	}
	return tmp_stack;
}

std::stack<LedInt2> LedDriver::MatrixStartTraverseLeftRight(int xStart, int yStart, bool isPstDirection)
{
	ImDrawList *draw_start = ImGui::GetWindowDrawList();
	draw_start->AddCircleFilled(ImVec2(firstx + xStart * draw_area_size, firsty + yStart * draw_area_size), draw_area_size*0.5f, circle_col);
	std::stack<LedInt2> tmp_stack;

	if (!yStart) {
		while (yStart < vertex_area_size[1]) {
			RouteMoveLeftRight(xStart, yStart, isPstDirection, 1, tmp_stack);
		}
	}
	else {
		while (yStart >= 0) {
			RouteMoveLeftRight(xStart, yStart, isPstDirection, -1, tmp_stack);
		}
	}
	return tmp_stack;
}

