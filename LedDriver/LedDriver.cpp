#include "LedDriver.h"
#include "imgui_internal.h"
#include "LedFileDialog.h"
#include "IconsFontAwesome5.h"
#include <iostream>
#include <cstring>
#include <fstream>
#include <tuple>
#include <iterator>
#include <algorithm>
#include <thread>

#include <exception>

#include <gl/GL.h>
#include <gl/GLU.h>

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
//	vertex_area_size[0] = 0;
//	vertex_area_size[1] = 0;
//	row_dist = 0.0f;
//	col_dist = 0.0f;
//	draw_area_size	 = 16.0f;
//	is_init_open	 = true;
//	//is_init_vertex   = false;
//	is_concern		 = false;
//	is_init_open	 = false;
//	is_start_play    = false;
//	is_video_play    = false;
//	is_save			 = false;
//	is_open_serial   = false;
//	is_show_init_win = true;
//	is_show_mode_win = true;
//	combo_select	 = 0;
//	nPageCount		 = 0;
//	nWhichPage		 = 0;
//	nCurrentMode	 = 0;
//	//this->Init();
//	manualLayout = LedManualLayout::CreateManualLayout();
//	commonData = new CommonData;
//	drawInitWindow = new DrawManage(commonData);
//	InitializeCriticalSection(&cs);
}

LedDriver::LedDriver(std::shared_ptr<CommonData> pData) :
	//draw_area_size(16.0f),
	is_save(false),
	is_open_serial(false),
	is_show_init_win(true),
	is_show_mode_win(true),
	is_open_manual(false),
	is_new_project(false),
	is_open_project(false),
	is_start_project(false),
	is_finishi_image(false),
	is_save_file_done(true),
	is_english(false),
	is_chinese(false)
{	
	manualLayout = LedManualLayout::CreateManualLayout();
	commonData = pData;
	controlMode = new ControlMode(commonData);
	drawInitWindow = new DrawManage(commonData, controlMode);
	project = new LedProject;
	language = LanguageSetting::GetLanguageInstance();
	//InitializeCriticalSection(&cs);

	//drawInitWindow->InitDataLinePixels();
	/*std::ifstream freadlanguage;
	std::string strlanguage;
	freadlanguage.open("Language\\config.data");
	freadlanguage >> strlanguage;
	if (strlanguage.compare("Chinese") == 0)
		is_chinese = true;
	else if (strlanguage.compare("English") == 0)
		is_english = true;
	freadlanguage.close();*/
}


LedDriver::~LedDriver()
{
	//DeleteCriticalSection(&cs);
	delete drawInitWindow;
	delete controlMode;
	delete project;
}

void LedDriver::Draw()
{
	ImGui::BeginMainMenuBar();
	if (ImGui::BeginMenu(language->m_files.c_str())) {
		ImGui::MenuItem(language->m_newProject.c_str(), NULL, &is_new_project);
		ImGui::MenuItem(language->m_openProject.c_str(), NULL, &is_open_project);
		ImGui::MenuItem(language->m_saveProject.c_str(), NULL, &is_save_project, is_start_project);
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu(language->m_option.c_str())) {
		ImGui::MenuItem(language->m_exportData.c_str(), NULL, &is_save);
		ImGui::MenuItem(language->m_serialPort.c_str(), NULL, &is_open_serial);
		if (ImGui::BeginMenu(language->m_languageOption.c_str())) {
			ImGui::MenuItem(language->m_language1.c_str(), NULL, &is_chinese);
			ImGui::MenuItem(language->m_language2.c_str(), NULL, &is_english);
			ImGui::EndMenu();
		}
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu(language->m_windowOption.c_str())) {
		ImGui::MenuItem(language->m_manual.c_str(), NULL, &is_open_manual);
		ImGui::EndMenu();
	}
	ImGui::EndMainMenuBar();
	MenuBarControl();

	ImGuiViewport* viewport = ImGui::GetMainViewport();
	
	ImGui::SetNextWindowPos(ImVec2(viewport->Pos.x,viewport->Pos.y+10.0f));
	ImGui::SetNextWindowSize(viewport->Size);
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGuiWindowFlags background_dockspace_flag = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	ImGui::Begin("Background DockSpace", false, background_dockspace_flag);

	ImGuiID dockspace_id = ImGui::GetID("Main Dockspace");
	if (ImGui::DockBuilderGetNode(dockspace_id) == NULL || language->is_change_language)
	{
		ImGui::DockBuilderRemoveNode(dockspace_id);
		ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace); 
		ImGui::DockBuilderSetNodeSize(dockspace_id, ImVec2(1280,720));

		ImGuiID dock_main_id = dockspace_id;
		ImGuiID dock_id_right = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Right, 0.35f, NULL, &dock_main_id);
		ImGuiID dock_id_right_bottom = ImGui::DockBuilderSplitNode(dock_id_right, ImGuiDir_Down, 0.60f, NULL, &dock_id_right);

		ImGui::DockBuilderDockWindow(language->m_drawMainTitle.c_str(), dock_main_id);
		ImGui::DockBuilderDockWindow(language->m_initWindowTitle.c_str(), dock_id_right);
		ImGui::DockBuilderDockWindow(language->m_controlWindowTitle.c_str(), dock_id_right_bottom);
		ImGui::DockBuilderFinish(dockspace_id);
		language->is_change_language = false;
	}
	ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);

	ImGuiIO& io = ImGui::GetIO();
	if (io.KeysDown[341]) {
		if (io.MouseWheel == 1) {
			commonData->rowDict += (commonData->rowDict / 15.0f);
			commonData->colDict += (commonData->colDict / 15.0f);
		}
		else if (io.MouseWheel == -1) {
			commonData->rowDict -= (commonData->rowDict / 15.0f);
			commonData->colDict -= (commonData->colDict / 15.0f);
		}
	}

	
	ImGui::Begin(language->m_drawMainTitle.c_str(), false, ImGuiWindowFlags_HorizontalScrollbar);
	ImDrawList *draw_list = ImGui::GetWindowDrawList();
	ImGui::Text(language->m_drawArea.c_str()); ImGui::SameLine();
	ImGui::Text("FPS:%f", io.Framerate);
	/* Text下面的光标位置*/
	ImVec2 cursorPos = ImGui::GetCursorScreenPos();
	/* 设置第一个点的x，y*/
	firstx = cursorPos.x + commonData->cicleSize * 0.5f + 3.5f;
	firsty = cursorPos.y + commonData->cicleSize * 0.5f + 3.5f;



	/* 设置画布的大小，超过屏幕大小会显示滚动条*/
	ImGui::Dummy(ImVec2(commonData->rowDict * commonData->whMatrix[0], commonData->colDict * commonData->whMatrix[1]));
	//ImGui::Dummy(ImVec2(draw_area_size * commonData->whMatrix[0], draw_area_size * commonData->whMatrix[1]));
	//ImGui::InvisibleButton("dummy btn", ImVec2(1.0f+draw_area_size * vertex_area_size[0], 1.0f+draw_area_size * vertex_area_size[1]));

	if (drawInitWindow->isInitVertex)
		FirstSetPaintWindow(draw_list);
	if (controlMode->isBackgroundShow) {
		ImVec2 cursorEnd(cursorPos.x + commonData->whMatrix[0] * commonData->rowDict, cursorPos.y + commonData->whMatrix[1] * commonData->colDict);
		DrawBackgroundImage(draw_list, cursorPos, cursorEnd);
	}

	if (!controlMode->isStartPlay && !controlMode->sPage.empty() && controlMode->nCurrentMode==0) {
		controlMode->sPage[controlMode->nWhichPage].DisplayCanvas(firstx, firsty, commonData->rowDict, commonData->colDict,commonData->cicleSize, draw_list);
	}

	if (!controlMode->isVideoPlay && controlMode->nCurrentMode == 1) {
		for (int i = 0; i < commonData->whMatrix[1]; i++) {
			for (int j = 0; j < commonData->whMatrix[0]; j++) {
				draw_list->AddCircle(
					ImVec2(firstx + j * commonData->rowDict, firsty + i * commonData->colDict), commonData->cicleSize*0.5f, IM_COL32_WHITE, 32, 2.0f);

			}
		}
	}

	if (controlMode->isStartPlay)
		SecondSetPaintWindow(draw_list);

	if (controlMode->isVideoPlay)
		ThridSetPaintWindow(draw_list);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(100, 100));
	if (!is_save_file_done) {
		ImGui::OpenPopup("Information");
		if (ImGui::BeginPopupModal("Information", NULL)) {
			ImGui::Indent(30.0f);
			ImGui::Text(language->m_saveInformation.c_str());
			ImGui::EndPopup();
		}
	}
	ImGui::PopStyleVar();
	
	ImGui::End();//Draw window End
	
	if (project->isStartProject) drawInitWindow->InitControlWindow(firstx, firsty, commonData->cicleSize);
	//if (is_show_init_win) InitControlWindow(&is_show_init_win);
	if (drawInitWindow->IsDrawFinish()) controlMode->ModeSelectWindow(firstx, firsty);
	ImGui::End();//Background DockSpace End
	
}

void LedDriver::MenuBarControl()
{
	if (is_new_project){
		if (project->NewProject(commonData)) {
			drawInitWindow->ProjectUpdate();
			InitLineImage();
		}
		is_new_project = false;
		is_start_project = true;
	}
	if (is_open_project) {
		if (project->OpenProject(commonData)) {
			drawInitWindow->ProjectUpdate();
			drawInitWindow->isInitVertex = true;
			InitLineImage();
		}
		is_open_project = false;
		is_start_project = true;
	}
	if (is_save_project) {
		project->SaveProject();
		is_save_project = false;
	}

	if (is_open_manual) {
		if (!manualLayout->IsInitCanvas()) {
			manualLayout->SetCoordinate(commonData->whMatrix, commonData->cicleSize, commonData->rowDict, commonData->colDict, drawInitWindow->GetRoute());
			manualLayout->SetStatus(true);
		}
		if (manualLayout->IsSettingDone()) {
			commonData->comboSelect = 17;
			commonData->lineList = manualLayout->GetLineDirection();
			is_open_manual = false;
		}
		manualLayout->DrawWindow(&is_open_manual);
	}
	else {
		manualLayout->SetStatus(false);
	}
	//保存文件
	if (is_save) {
		LedFileDialog fileDialog;
		saveFileName = fileDialog.OpenSaveFileDialog();
		//取消文件选择，不进行数据保存
		if (!saveFileName.empty()) {
			if (controlMode->nCurrentMode == 0) {
				std::thread save_manual(&LedDriver::ThreadSaveManual, this);
				save_manual.join();
			}
			else if (controlMode->nCurrentMode == 1) {
				std::thread save_video(&LedDriver::ThreadSaveVideo, this);
				save_video.detach();
			}
		}
		
		is_save = false;
	}
	if (is_open_serial) { OpenSerialPort(); is_open_serial = false; } //打开串口
	if (is_chinese) {
		language->SetCurrentLanguage("Chinese");
		is_chinese = false;
		language->is_change_language = true;
	}
	if (is_english) {
		language->SetCurrentLanguage("English");
		is_english = false;
		language->is_change_language = true;
	}
}


void LedDriver::FirstSetPaintWindow(ImDrawList *draw_list)
{
	/* 绘制初始画布*/
	for (int i = 0; i < commonData->whMatrix[1]; i++) {
		for (int j = 0; j < commonData->whMatrix[0]; j++) {
			draw_list->AddCircle(
				ImVec2(firstx + j * commonData->rowDict, firsty + i * commonData->colDict), commonData->cicleSize*0.5f, IM_COL32_WHITE, 32, 2.0f);
			
			/*draw_list->AddCircle(
				ImVec2(firstx + j * (commonData->cicleSize), firsty + i * (commonData->cicleSize)), commonData->cicleSize*0.5f, IM_COL32_WHITE, 32, 2.0f);
*/
		}
	}

	/* 绘制数据线*/
	if (commonData->comboSelect != 0) {
		std::list<LedInt2> line_points = drawInitWindow->GetRoute();
		std::list<LedInt2>::iterator line_iter1 = line_points.begin();
		std::list<LedInt2>::iterator line_iter2 = line_points.begin();
		line_iter2++;
		draw_list->AddCircleFilled(ImVec2(firstx + (*line_iter1).x*commonData->rowDict, firsty + (*line_iter1).y*commonData->colDict), commonData->cicleSize*0.5f, IM_COL32(0, 255, 0, 255));
		while (line_iter2 != line_points.end()) {
			draw_list->AddLine(ImVec2(firstx + (*line_iter1).x*commonData->rowDict, firsty + (*line_iter1).y*commonData->colDict), ImVec2(firstx + (*line_iter2).x*commonData->rowDict, firsty + (*line_iter2).y*commonData->colDict),
				IM_COL32(90, 250, 250, 255), 2.0f);
			line_iter1++;
			line_iter2++;
		}
	}
	
}
void LedDriver::SecondSetPaintWindow(ImDrawList *draw_list)
{
	//遍历每一页的画布
	
	controlMode->dbNowTime = ImGui::GetTime() - controlMode->sPage[controlMode->nIntervalNum].nTickTime;
	std::vector<std::pair<LedInt2, unsigned char>>::iterator playIter = controlMode->sPage[controlMode->nIntervalNum].vEffectPoints.begin();

	//无到有渐变
	if (controlMode->sPage[controlMode->nIntervalNum].bGradientNone2Fill)
		for (; playIter != controlMode->sPage[controlMode->nIntervalNum].vEffectPoints.end(); playIter++)
			draw_list->AddCircleFilled(ImVec2(firstx + playIter->first.x * commonData->rowDict, firsty + playIter->first.y * commonData->colDict), commonData->cicleSize*0.5f, IM_COL32(255, 255, 255, playIter->second * (controlMode->dbNowTime / controlMode->sPage[controlMode->nIntervalNum].fTime)), 32);
	//有到无渐变
	else if (controlMode->sPage[controlMode->nIntervalNum].bGradientFill2None)
		for (; playIter != controlMode->sPage[controlMode->nIntervalNum].vEffectPoints.end(); playIter++)
			draw_list->AddCircleFilled(ImVec2(firstx + playIter->first.x * commonData->rowDict, firsty + playIter->first.y * commonData->colDict), commonData->cicleSize*0.5f, IM_COL32(255, 255, 255, playIter->second * (1 - (controlMode->dbNowTime> controlMode->sPage[controlMode->nIntervalNum].fTime? controlMode->sPage[controlMode->nIntervalNum].fTime: controlMode->dbNowTime) / controlMode->sPage[controlMode->nIntervalNum].fTime)), 32);
	//无渐变
	else
		for (; playIter != controlMode->sPage[controlMode->nIntervalNum].vEffectPoints.end(); playIter++)
			draw_list->AddCircleFilled(ImVec2(firstx + playIter->first.x * commonData->rowDict, firsty + playIter->first.y * commonData->colDict), commonData->cicleSize*0.5f, IM_COL32(255, 255, 255, playIter->second), 32);
	
	if (controlMode->dbNowTime > controlMode->sPage[controlMode->nIntervalNum].fTime) {
		controlMode->nIntervalNum++;
		controlMode->nIntervalNum = controlMode->nIntervalNum < controlMode->sPage.size() ? controlMode->nIntervalNum : 0;
		controlMode->sPage[controlMode->nIntervalNum].nTickTime = ImGui::GetTime();
	}
	
}

void LedDriver::ThridSetPaintWindow(ImDrawList *draw_list)
{
	LedReadVideo *video = controlMode->GetVideo();
	//if (!video->IsCanPlay()) return;
	if (controlMode->frameIndex < video->m_videoPrimitiveData.size()) {
		//auto vdrawImage = video->m_videoPrimitiveData.begin();
		//std::advance(vdrawImage, controlMode->frameIndex);
		std::list<std::tuple<int, int, unsigned char>> vdrawImage = video->m_videoPrimitiveData[controlMode->frameIndex];

		for (auto liter = vdrawImage.begin(); liter != vdrawImage.end(); liter++) {
			draw_list->AddCircleFilled(ImVec2(firstx + std::get<0>(*liter)*commonData->rowDict, firsty + std::get<1>(*liter)*commonData->colDict), commonData->cicleSize*0.5f, IM_COL32(255, 255, 255, std::get<2>(*liter)), 32);
		}
		//fNowTime += (1000.0f / ImGui::GetIO().Framerate);
		if (ImGui::GetTime() - controlMode->dbNowTime > (video->GetFrameTime() / 1000.0)) {
			controlMode->frameIndex++;
			controlMode->dbNowTime = ImGui::GetTime();
		}
	}
	else {
		controlMode->frameIndex = 0;
	}
}

void LedDriver::SaveDataToFile(unsigned char mod, int frameNumber, int frameSize, int frameTime)
{
	is_save_file_done = false;
	
	std::lock_guard<std::mutex> threadLock(m_mutex);
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
		for (size_t i = 0; i < controlMode->sPage.size(); i++) {
			if (controlMode->sPage[i].bGradientFill2None || controlMode->sPage[i].bGradientNone2Fill) {
				int gradientFPS = (int)(controlMode->sPage[i].fTime*25.0f);//1000/40
				unsigned char incre = (unsigned char)(255 / gradientFPS);
				unsigned char lightcd;
				lightcd = controlMode->sPage[i].bGradientFill2None ? 0xFF : 0x05;
				for (; gradientFPS > 0; gradientFPS--) {
					frameTime = 40;
					//时间
					fwrite(&frameTime, sizeof(int), 1, out_to_file);
					auto out_iter = commonData->lineList.begin();
					for (int j = 0; out_iter != commonData->lineList.end(); j++) {
						lightData[j] = (unsigned char)(controlMode->sPage[i].vnCanvas[(*out_iter).x + (*out_iter).y*(commonData->whMatrix[0])] == 0 ? 0x00 : lightcd);
						out_iter++;
					}
					//数据
					fwrite(lightData, sizeof(unsigned char), frameSize, out_to_file);
					controlMode->sPage[i].bGradientFill2None ? lightcd -= incre : lightcd += incre;
				}
			}
			else {
				frameTime = (int)(controlMode->sPage[i].fTime * 1000.0f);
				//时间
				fwrite(&frameTime, sizeof(int), 1, out_to_file);
				auto out_iter = commonData->lineList.begin();
				for (int j = 0; out_iter != commonData->lineList.end(); j++) {
					lightData[j] = (unsigned char)(controlMode->sPage[i].vnCanvas[(*out_iter).x + (*out_iter).y*(commonData->whMatrix[0])] == 0 ? 0x00 : 0xFF);
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
		LedReadVideo *video = controlMode->GetVideo();
		//for (auto primtIter = video->m_videoPrimitiveData.begin(); primtIter != video->m_videoPrimitiveData.end(); primtIter++) {
		for (size_t i = 0; i < video->m_videoPrimitiveData.size(); i++) {
			
			memset(lightData, 0, frameSize);
			//遍历每一个点，TODO:BinarySearch
			//for (auto liter = primtIter->begin(); liter != primtIter->end(); liter++) {
			for (auto liter = video->m_videoPrimitiveData[i].begin(); liter != video->m_videoPrimitiveData[i].end(); liter++) {
				
				LedInt2 effLi(std::get<0>(*liter), std::get<1>(*liter));
				int dirc = std::distance(commonData->lineList.begin(), std::find(commonData->lineList.begin(), commonData->lineList.end(), effLi));
				lightData[dirc] = std::get<2>(*liter);
			}
			
			fwrite(lightData, sizeof(unsigned char), frameSize, out_to_file);
		}
		
		break;
	}
	free(lightData);
	fclose(out_to_file);
	is_save_file_done = true;
}


void LedDriver::ThreadSaveManual()
{
	int manual_frame_num = 0;
	for (auto pageiter = controlMode->sPage.begin(); pageiter != controlMode->sPage.end(); pageiter++) {
		if (pageiter->bGradientNone2Fill || pageiter->bGradientFill2None)
			manual_frame_num += (int)(pageiter->fTime * 1000.0f / 40.0f);
		else
			manual_frame_num += 1;
	}

	SaveDataToFile('a', manual_frame_num, commonData->whMatrix[0]*commonData->whMatrix[1], 0xFFFF);
}

void LedDriver::ThreadSaveVideo()
{
	SaveDataToFile('b', controlMode->GetVideo()->GetFrameCount(), commonData->whMatrix[0]*commonData->whMatrix[1], int(controlMode->GetVideo()->GetFrameTime()));
}

void LedDriver::DrawBackgroundImage(ImDrawList *draw_list, ImVec2 &left_up, ImVec2 &right_down)
{
	unsigned int texture = controlMode->GetImageTexture();
	draw_list->AddImage((ImTextureID)texture, left_up, right_down);
}

void LedDriver::InitLineImage()
{
	//绘制连线图
	if (!is_finishi_image) {
		drawInitWindow->InitDataLinePixels();
		is_finishi_image = true;
	}
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
