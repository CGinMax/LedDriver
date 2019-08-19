#include "LedDriver.h"
#include "imgui_internal.h"
#include "LedFileDialog.h"
#include "IconsFontAwesome5.h"
#include <iostream>
#include <cstring>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <cstdio>
#include <process.h>
#include <exception>

void __cdecl ThreadInitVideo(void *param)
{
	LedDriver *pld = (LedDriver*)param;
	pld->InitVideo();
	_endthread();
}

void __cdecl ThreadSaveManual(void *param)
{
	LedDriver *pld = (LedDriver*)param;
	int manual_frame_num = 0;
	std::vector<InstancePageData> page = pld->GetManualPage();
	for (auto pageiter = page.begin(); pageiter != page.end(); pageiter++) {
		if (pageiter->bGradientNone2Fill || pageiter->bGradientFill2None)
			manual_frame_num += (int)(pageiter->fTime * 1000.0f / 40.0f);
		else
			manual_frame_num += 1;
	}
	
	pld->SaveDataToFile('a', manual_frame_num, pld->GetVertexArea(), 0xFFFF);
	_endthread();
}

void __cdecl ThreadSaveVideo(void *param)
{
	LedDriver *pld = (LedDriver*)param;
	pld->SaveDataToFile('b', pld->GetVideo().GetFrameCount(), pld->GetVertexArea(), (int)pld->GetVideo().GetFrameTime());
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

LedDriver::LedDriver()
{
	vertex_area_size[0] = 0;
	vertex_area_size[1] = 0;
	draw_area_size	 = 16.0f;
	is_init_open	 = true;
	is_init_vertex   = false;
	is_concern		 = false;
	is_init_open	 = false;
	is_start_play    = false;
	is_video_play    = false;
	is_save			 = false;
	is_open_serial   = false;
	is_show_draw_win = true;
	is_show_init_win = true;
	is_show_mode_win = true;
	radio_select	 = 0;
	nPageCount		 = 0;
	nWhichPage		 = 0;
	nCurrentMode	 = 0;
	//this->Init();
	//manualLayout = LedManualLayout::CreateManualLayout();
	InitializeCriticalSection(&cs);
}


LedDriver::~LedDriver()
{
	DeleteCriticalSection(&cs);
	//delete manualLayout;
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
	//static bool manual_open = false;
	ImGui::BeginMainMenuBar();
	if (ImGui::BeginMenu(ICON_FA_TOOLS" 选项")) {
		ImGui::MenuItem(ICON_FA_SAVE" 保存数据", NULL, &is_save);
		ImGui::MenuItem(ICON_FA_SD_CARD" 打开串口", NULL, &is_open_serial);
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu(ICON_FA_WINDOW_MAXIMIZE" 窗口")) {
		//ImGui::MenuItem(u8"手工布局", NULL, &manual_open);
		//ImGui::MenuItem(u8"自动布局");
		ImGui::MenuItem(ICON_FA_PENCIL_RULER" 显示窗口", NULL, &is_show_draw_win);
		ImGui::MenuItem(ICON_FA_SLIDERS_H" 初始化设置窗口", NULL, &is_show_init_win);
		ImGui::MenuItem(ICON_FA_BRAILLE" 点阵操作窗口", NULL, &is_show_mode_win);
		ImGui::EndMenu();
	}
	ImGui::EndMainMenuBar();

	//if (manual_open) manualLayout->DrawWindow(&manual_open);
	//保存文件
	if (is_save) {
		saveFileName = LedFileDialog::OpenSaveFileDialog(); 
		if (nCurrentMode == 0)
			_beginthread(ThreadSaveManual, 0, (void*)this);
		else 
			_beginthread(ThreadSaveVideo, 0, (void*)this);
		is_save = false;
	}
	if (is_open_serial) { OpenSerialPort(); is_open_serial = false; } //打开串口

	ImGuiViewport* viewport = ImGui::GetMainViewport();
	
	ImGui::SetNextWindowPos(ImVec2(viewport->Pos.x,viewport->Pos.y+10.0f));
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
		ImGuiID dock_id_right_bottom = ImGui::DockBuilderSplitNode(dock_id_right, ImGuiDir_Down, 0.60f, NULL, &dock_id_right);

		ImGui::DockBuilderDockWindow(ICON_FA_PENCIL_RULER" 显示窗口", dock_main_id);
		ImGui::DockBuilderDockWindow(ICON_FA_SLIDERS_H" 初始化设置", dock_id_right);
		ImGui::DockBuilderDockWindow(ICON_FA_BRAILLE" 点阵操作", dock_id_right_bottom);
		ImGui::DockBuilderFinish(dockspace_id);
	}
	ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);

	ImGuiIO& io = ImGui::GetIO();
	if (io.KeysDown[341]) {
		if (io.MouseWheel == 1) draw_area_size += 3.0f; else if(io.MouseWheel == -1) draw_area_size -= 3.0f;
	}

	if (is_show_draw_win) {
		ImGui::Begin(ICON_FA_PENCIL_RULER" 显示窗口", &is_show_draw_win, ImGuiWindowFlags_HorizontalScrollbar);
		ImDrawList *draw_list = ImGui::GetWindowDrawList();
		ImGui::Text(u8"显示区域"); ImGui::SameLine();
		ImGui::Text("FPS:%f", io.Framerate);
		/* Text下面的光标位置*/
		ImVec2 cursorPos = ImGui::GetCursorScreenPos();
		/* 设置第一个点的x，y*/
		firstx = cursorPos.x + draw_area_size * 0.5f + 3.5f;
		firsty = cursorPos.y + draw_area_size * 0.5f + 3.5f;
		/* 设置画布的大小，超过屏幕大小会显示滚动条*/
		ImGui::Dummy(ImVec2(draw_area_size * vertex_area_size[0], draw_area_size * vertex_area_size[1]));
		//ImGui::InvisibleButton("dummy btn", ImVec2(1.0f+draw_area_size * vertex_area_size[0], 1.0f+draw_area_size * vertex_area_size[1]));

		if (is_init_vertex)
			FirstSetPaintWindow(draw_list);

		if (is_start_play)
			SecondSetPaintWindow(draw_list);

		if (is_video_play)
			ThridSetPaintWindow(draw_list);

		if (!is_start_play && !sPage.empty() && nCurrentMode==0) {
			sPage[nWhichPage].DisplayCanvas(firstx, firsty, draw_area_size, draw_list);
		}
		ImGui::End();//Draw window End
	}


	if (is_show_init_win) InitControlWindow(&is_show_init_win);
	if (is_show_mode_win) ModeSelectWindow(&is_show_mode_win);
	ImGui::End();//Background DockSpace End
	ImGui::PopStyleVar();
	
}


void LedDriver::FirstSetPaintWindow(ImDrawList *draw_list)
{
	/* 绘制初始画布*/
	for (int i = 0; i < vertex_area_size[1]; i++) {
		for (int j = 0; j < vertex_area_size[0]; j++) {
			draw_list->AddCircle(
				ImVec2(firstx + j * draw_area_size, firsty + i * draw_area_size), draw_area_size*0.5f, IM_COL32_WHITE, 32, 2.0f);
		}
	}

	/* 绘制数据线*/
	if (radio_select != 0) {
		std::list<LedInt2> line_points = GetRoute();
		std::list<LedInt2>::iterator line_iter1 = line_points.begin();
		std::list<LedInt2>::iterator line_iter2 = line_points.begin();
		line_iter2++;
		while (line_iter2 != line_points.end()) {
			draw_list->AddLine(ImVec2(firstx + (*line_iter1).x*draw_area_size, firsty + (*line_iter1).y*draw_area_size), ImVec2(firstx + (*line_iter2).x*draw_area_size, firsty + (*line_iter2).y*draw_area_size),
				IM_COL32(90, 250, 250, 255), 2.0f);
			line_iter1++;
			line_iter2++;
		}
	}
	
}
void LedDriver::SecondSetPaintWindow(ImDrawList *draw_list)
{
	//遍历每一页的画布
	dbNowTime = ImGui::GetTime() - sPage[nIntervalNum].nTickTime;
	std::vector<LedInt2>::iterator playIter = sPage[nIntervalNum].vEffectPoints.begin();

	//无到有渐变
	if (sPage[nIntervalNum].bGradientNone2Fill)
		for (; playIter != sPage[nIntervalNum].vEffectPoints.end(); playIter++)
			draw_list->AddCircleFilled(ImVec2(firstx + playIter->x * draw_area_size, firsty + playIter->y * draw_area_size), draw_area_size*0.5f, IM_COL32(255, 255, 255, 255 * (dbNowTime / sPage[nIntervalNum].fTime)), 32);
	//有到无渐变
	else if (sPage[nIntervalNum].bGradientFill2None)
		for (; playIter != sPage[nIntervalNum].vEffectPoints.end(); playIter++)
			draw_list->AddCircleFilled(ImVec2(firstx + playIter->x * draw_area_size, firsty + playIter->y * draw_area_size), draw_area_size*0.5f, IM_COL32(255, 255, 255, 255 * (1 - (dbNowTime>sPage[nIntervalNum].fTime?sPage[nIntervalNum].fTime:dbNowTime) / sPage[nIntervalNum].fTime)), 32);
	//无渐变
	else
		for (; playIter != sPage[nIntervalNum].vEffectPoints.end(); playIter++)
			draw_list->AddCircleFilled(ImVec2(firstx + playIter->x * draw_area_size, firsty + playIter->y * draw_area_size), draw_area_size*0.5f, IM_COL32_WHITE, 32);
	
	if (dbNowTime > sPage[nIntervalNum].fTime) {
		nIntervalNum++;
		nIntervalNum = nIntervalNum < sPage.size() ? nIntervalNum : 0;
		sPage[nIntervalNum].nTickTime = ImGui::GetTime();
	}
	
}

void LedDriver::ThridSetPaintWindow(ImDrawList *draw_list)
{
	if (frameIndex < testVideo.m_videoPrimitiveData.size()){
		std::list<LedInt2> vdrawImage = testVideo.m_videoPrimitiveData[frameIndex];
		
		for (auto liter = vdrawImage.begin(); liter != vdrawImage.end(); liter++) {
			draw_list->AddCircleFilled(ImVec2(firstx + (*liter).x*draw_area_size, firsty + (*liter).y*draw_area_size), draw_area_size*0.5f, IM_COL32_WHITE, 32);
		}
		//fNowTime += (1000.0f / ImGui::GetIO().Framerate);
		if (ImGui::GetTime() - dbNowTime > (testVideo.GetFrameTime() / 1000.0)) {
			frameIndex++;
			dbNowTime = ImGui::GetTime();
		}
	}
	else {
		frameIndex = 0;
	}
	
}

/* 控制显示窗口*/
void LedDriver::InitControlWindow(bool *p_open)
{
	static int input_size[2] = { 0,0 };
	if (!ImGui::Begin(ICON_FA_SLIDERS_H" 初始化设置", p_open)) {
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

	if (ImGui::Button(u8"绘制确认")) {
		index_list = GetRoute();
		is_concern = true;
		is_init_vertex = false;
		this->Init();
	}
	ImGui::SameLine();
	if (is_concern)
		ImGui::Text(u8"设置完成");


	ImGui::End();
}

void LedDriver::ModeSelectWindow(bool *p_open)
{
	
	if (ImGui::Begin(ICON_FA_BRAILLE" 点阵操作", p_open))
	{
		
		ImGui::Combo(u8"模式选择", &nCurrentMode, u8"\uf4fe"" 手动选择模式\0" u8"\uf03d"" Video模式\0\0");
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
			ImGui::BeginTabBar("autoTabBar", ImGuiTabBarFlags_None);
			for (size_t i = 0; i < sPage.size(); i++) {
				if (!sPage[i].bOpen) {
					std::vector<InstancePageData>::iterator tIter = sPage.begin() + i;
					sPage.erase(tIter); continue;
				}

				ImGuiTabItemFlags tiflag = (sPage[i].bModify ? ImGuiTabItemFlags_UnsavedDocument : 0);
				if (ImGui::BeginTabItem(sPage[i].szTabName.c_str(), &(sPage[i].bOpen), tiflag)) {
					nWhichPage = i;
					ImGui::Checkbox(u8"选点", &(sPage[i].bCheckMouse)); ImGui::SameLine();
					//ImGui::CheckboxFlags()
					if (ImGui::Checkbox(u8"渐变:暗到亮", &(sPage[i].bGradientNone2Fill))) {
						sPage[i].bGradientFill2None = false;
						sPage[i].bModify = true;
					}
					ImGui::SameLine();
					if (ImGui::Checkbox(u8"渐变:亮到暗", &(sPage[i].bGradientFill2None))) {
						sPage[i].bGradientNone2Fill = false;
						sPage[i].bModify = true;
					}

					if (ImGui::InputFloat(u8"点亮时间", &(sPage[i].fTime), 0.0f, 0.0f, "%.2f", ImGuiInputTextFlags_CharsDecimal)) {
						sPage[i].bModify = true;
					}
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
						sPage[i].bModify = false;
					}

					ImGui::EndTabItem();
				}
				if (sPage[i].bCheckMouse) {
					sPage[i].bModify = true;
					ImGui::GetForegroundDrawList()->AddCircleFilled(ImGui::GetMousePos(), draw_area_size*0.5f, IM_COL32_WHITE, 32);
					MouseClickDraw(i);
				}
			}
			ImGui::EndTabBar();
			
			ImGui::Separator();
			ImGui::Unindent(20.0f);

			if (ImGui::Button(u8"演示")) {
				//初始化播放的第一个时刻的状态
				sPage[0].nTickTime = ImGui::GetTime();
				nIntervalNum = 0;
				is_start_play = true;
			}
			ImGui::SameLine(); 
			if (ImGui::Button(u8"关闭演示")) is_start_play = false;
			
			
			break;
		case 1:
			if (ImGui::Button(u8"导入")) {
				testVideo.SetVideoFileName(SelectFileNameDialog());
				_beginthread(ThreadInitVideo, 0, (void*)this);
			}
			ImGui::SameLine();
			if (ImGui::Button(u8"演示播放")) {
				is_video_play = true;
				dbNowTime = ImGui::GetTime();
				frameIndex = 0;
			}
			ImGui::SameLine();
			if (ImGui::Button(u8"关闭演示"))
				is_video_play = false;
			
			testVideo.m_isInit ? ImGui::Text(testVideo.GetVideoFileName().c_str()) : ImGui::Text(" ");
			break;
		}

	}
	ImGui::End();//End model control
}


void LedDriver::Clear()
{
	index_list.clear();
	sPage.clear();
}

/* 获取路径*/
std::list<LedInt2> LedDriver::GetRoute()
{
	std::list<LedInt2> route_points;
	
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



//TODO:Fix bug
void LedDriver::MouseClickDraw(int pageindex)
{
	static int cx = 0;
	static int cy = 0;
	if (ImGui::IsMouseClicked(0)) {
		ImVec2 cpoint = ImGui::GetIO().MouseClickedPos[0];
		cx = (int)((cpoint.x - firstx + draw_area_size * 0.5f) / draw_area_size);
		cy = (int)((cpoint.y - firsty + draw_area_size * 0.5f) / draw_area_size);
		if (cx < vertex_area_size[0] && cy < vertex_area_size[1])
			sPage[pageindex].vnCanvas[cy*vertex_area_size[0] + cx] = sPage[pageindex].vnCanvas[cy*vertex_area_size[0] + cx] ? 0 : 1;			
	}
	
	if (ImGui::IsMouseDragging(0)) {
		ImVec2 dragArea = ImGui::GetIO().MousePos;
		//ImVec2 dragArea = ImVec2(ImGui::GetMouseDragDelta(0).x + cpoint.x, ImGui::GetMouseDragDelta(0).y + cpoint.y);
		int dragx = (int)((dragArea.x - firstx + draw_area_size * 0.5f) / draw_area_size);
		int dragy = (int)((dragArea.y - firsty + draw_area_size * 0.5f) / draw_area_size);
		if (dragx < vertex_area_size[0] && dragy < vertex_area_size[1])
			sPage[pageindex].vnCanvas[dragy*vertex_area_size[0] + dragx] = sPage[pageindex].vnCanvas[cy*vertex_area_size[0] + cx];

	}
}

void LedDriver::InitVideo()
{
	testVideo.Init(vertex_area_size);
}

std::string LedDriver::SelectFileNameDialog()
{
	std::string filenameUnicode = LedFileDialog::SelectFileNameDialog();
	std::string filenameUtf8 = ToUTF8(filenameUnicode);
	return filenameUtf8;
}

void LedDriver::SaveDataToFile(unsigned char mod, int frameNumber, int frameSize, int frameTime)
{
	EnterCriticalSection(&cs);
	FILE *out_to_file;
	unsigned char *lightData;
	out_to_file = fopen(saveFileName.c_str(), "wb");
	//模式标志
	fwrite(&mod, sizeof(unsigned char), 1, out_to_file);
	//帧数
	fwrite(&frameNumber, sizeof(int), 1, out_to_file);
	//每帧大小
	fwrite(&frameSize, sizeof(int), 1, out_to_file);//frameSize=vertex_area_size[0]*vertex_area_size[1]
	//时间
	fwrite(&frameTime, sizeof(int), 1, out_to_file);

	lightData = (unsigned char *)malloc(sizeof(unsigned char)*frameSize);

	switch (mod)
	{
	//手动模式
	case 'a': 

		for (size_t i = 0; i < sPage.size(); i++) {
			if (sPage[i].bGradientFill2None || sPage[i].bGradientNone2Fill) {
				int gradientFPS = (int)(sPage[i].fTime*25.0f);//1000/40
				unsigned char incre = (unsigned char)(255 / gradientFPS);
				unsigned char lightcd;
				lightcd = sPage[i].bGradientFill2None ? 0xFF : 0x05;
				for (; gradientFPS > 0; gradientFPS--) {
					frameTime = 40;
					//时间
					fwrite(&frameTime, sizeof(int), 1, out_to_file);
					auto out_iter = index_list.begin();
					for (int j = 0; out_iter != index_list.end(); j++) {
						lightData[j] = (unsigned char)(sPage[i].vnCanvas[(*out_iter).x + (*out_iter).y*vertex_area_size[0]] == 0 ? 0x00 : lightcd);
						out_iter++;
					}
					//数据
					fwrite(lightData, sizeof(unsigned char), frameSize, out_to_file);
					sPage[i].bGradientFill2None ? lightcd -= incre : lightcd += incre;
				}
			}
			else {
				frameTime = (int)(sPage[i].fTime * 1000.0f);
				//时间
				fwrite(&frameTime, sizeof(int), 1, out_to_file);
				auto out_iter = index_list.begin();
				for (int j = 0; out_iter != index_list.end(); j++) {
					lightData[j] = (unsigned char)(sPage[i].vnCanvas[(*out_iter).x + (*out_iter).y*vertex_area_size[0]] == 0 ? 0x00 : 0xFF);
					out_iter++;
				}
				//数据
				fwrite(lightData, sizeof(unsigned char), frameSize, out_to_file);
			}
		}
		
		break;
	//视频模式
	case 'b':
		//遍历每一帧画面
		for (size_t i = 0; i < testVideo.m_videoPrimitiveData.size(); i++) {

			memset(lightData, 0, frameSize);
			//遍历每一个点，TODO:BinarySearch
			for (auto liter = testVideo.m_videoPrimitiveData[i].begin(); liter != testVideo.m_videoPrimitiveData[i].end(); liter++) {
				
				int dirc = std::distance(index_list.begin(), std::find(index_list.begin(), index_list.end(), *liter));
				lightData[dirc] = 0xFF;
			}

			fwrite(lightData, sizeof(unsigned char), frameSize, out_to_file);
		}
		break;
	}
	free(lightData);
	fclose(out_to_file);
	LeaveCriticalSection(&cs);
}

inline std::vector<InstancePageData> LedDriver::GetManualPage()
{
	return this->sPage;
}

inline int LedDriver::GetVertexArea()
{
	return vertex_area_size[0]*vertex_area_size[1];
}

inline LedReadVideo & LedDriver::GetVideo()
{
	return testVideo;
}

void LedDriver::OpenSerialPort()
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));
	/*si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = TRUE;*/
	::CreateProcess(TEXT("./LedSerialTool/LedSerialTool.exe"), NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);

}

/* 左右方向移动*/
void LedDriver::RouteMoveLeftRight(int & x, int & y, bool & d, int incrse, std::list<LedInt2> &my_list)
{
	if (d) {
		if (x < vertex_area_size[0] - 1) {
			my_list.push_back(LedInt2(x, y));
			x += 1;
		}
		else {
			my_list.push_back(LedInt2(x, y));
			y += incrse;
			d = !d;
		}
	}
	else {
		if (x > 0) {
			my_list.push_back(LedInt2(x, y));
			x -= 1;
		}
		else {
			my_list.push_back(LedInt2(x, y));
			y += incrse;
			d = !d;
		}
	}
}
/* 上下方向移动*/
void LedDriver::RouteMoveUpDown(int & x, int & y, bool & d, int incrse, std::list<LedInt2> &my_list)
{
	if (d) {
		if (y < vertex_area_size[1]-1) {
			my_list.push_back(LedInt2(x, y));
			y += 1;
		}
		else {
			my_list.push_back(LedInt2(x, y));
			x += incrse;
			d = !d;
		}
	}
	else {
		if (y > 0) {
			my_list.push_back(LedInt2(x, y));
			y -= 1;
		}
		else {
			my_list.push_back(LedInt2(x, y));
			x += incrse;
			d = !d;
		}
	}
}

std::list<LedInt2> LedDriver::MatrixStartTraverseUpDown(int xStart, int yStart, bool isPstDirection)
{
	ImDrawList *draw_start = ImGui::GetWindowDrawList();
	draw_start->AddCircleFilled(ImVec2(firstx+xStart*draw_area_size, firsty+yStart*draw_area_size), draw_area_size*0.5f, IM_COL32_WHITE);
	std::list<LedInt2> tmp_list;
	
	if (xStart == 0) {
		while (xStart < vertex_area_size[0]) {
			RouteMoveUpDown(xStart, yStart, isPstDirection, 1, tmp_list);
		}
	}
	else {
		while (xStart >= 0) {
			RouteMoveUpDown(xStart, yStart, isPstDirection, -1, tmp_list);
		}
	}
	return tmp_list;
}

std::list<LedInt2> LedDriver::MatrixStartTraverseLeftRight(int xStart, int yStart, bool isPstDirection)
{
	ImDrawList *draw_start = ImGui::GetWindowDrawList();
	draw_start->AddCircleFilled(ImVec2(firstx + xStart * draw_area_size, firsty + yStart * draw_area_size), draw_area_size*0.5f, IM_COL32_WHITE);
	std::list<LedInt2> tmp_list;

	if (!yStart) {
		while (yStart < vertex_area_size[1]) {
			RouteMoveLeftRight(xStart, yStart, isPstDirection, 1, tmp_list);
		}
	}
	else {
		while (yStart >= 0) {
			RouteMoveLeftRight(xStart, yStart, isPstDirection, -1, tmp_list);
		}
	}
	return tmp_list;
}

