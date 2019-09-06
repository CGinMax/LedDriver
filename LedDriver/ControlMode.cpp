#include "ControlMode.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "IconsFontAwesome5.h"
#include "LedFileDialog.h"
#include <functional>
#include <algorithm>
#include <thread>
#include <gl/GL.h>
#include <gl/GLU.h>

std::string ControlMode::ToUTF8(const std::string str)
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

ControlMode::ControlMode() : 
	nCurrentMode(0),
	nPageCount(0),
	nWhichPage(0),
	nIntervalNum(0),
	isBackgroundShow(false),
	isStartPlay(false),
	isVideoPlay(false),
	dbNowTime(0),
	frameIndex(0),
	firstPointx(0),
	firstPointy(0),
	backgroundImagePixels(nullptr)
{
	sPage.clear();
	memset(strSuccess, 0, sizeof(strSuccess));
}

ControlMode::ControlMode(std::shared_ptr<CommonData> spData) : 
	nCurrentMode(0),
	nPageCount(0),
	nWhichPage(0),
	nIntervalNum(0),
	isBackgroundShow(false),
	isStartPlay(false),
	isVideoPlay(false),
	dbNowTime(0),
	frameIndex(0),
	firstPointx(0),
	firstPointy(0),
	m_spData(spData),
	backgroundImagePixels(nullptr)
{
	sPage.clear();
	memset(strSuccess, 0, sizeof(strSuccess));
	readVideo = new LedReadVideo();
}


ControlMode::~ControlMode()
{
	delete readVideo;
	readVideo = nullptr;
	delete []backgroundImagePixels;
	sPage.clear();
}

void ControlMode::ModeSelectWindow(float _x, float _y)
{
	firstPointx = _x;
	firstPointy = _y;
	if (ImGui::Begin(ICON_FA_BRAILLE" 点阵操作"))
	{
		ImGui::Combo(u8"模式选择", &nCurrentMode, ICON_FA_USER_COG" 手动选择模式\0" ICON_FA_VIDEO" 动态模式\0\0");
		switch (nCurrentMode)
		{
		case 0:
			if (ImGui::Button(u8"载入背景图片")) {
				std::string open_file = this->SelectFile(new ImageFileDialog);
				
				if (!open_file.empty()) { loadBackgroundImageFileUtf = ToUTF8(open_file); ManualReadImage(open_file); } 
			}
			ImGui::SameLine();
			ImGui::Checkbox(u8"显示背景图片", &isBackgroundShow);
			ImGui::SameLine();
			ImGui::Text(loadBackgroundImageFileUtf.c_str());
			/*添加一页选项页*/
			if (ImGui::Button(u8"添加区间")) {
				nPageCount++;
				InstancePageData singlePage;
				char nbuf[8];
				sprintf(nbuf, "Way%d", nPageCount);
				singlePage.Init(m_spData->whMatrix, nbuf);
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

					if (ImGui::Button(u8"导入内容")) {
						std::string open_file = this->SelectFile(new ImageFileDialog);
						if (!open_file.empty()) {
							sPage[i].readImage.SetFileName(open_file);
							sPage[i].readImage.Init(m_spData->whMatrix, m_spData->rowDict, m_spData->colDict);
							std::fill(sPage[i].vnCanvas.begin(), sPage[i].vnCanvas.end(), 0);
							auto imagePrimitive = sPage[i].readImage.GetImagePrimitive();
							for (auto primitiveIter = imagePrimitive.begin(); primitiveIter != imagePrimitive.end(); primitiveIter++) {
								sPage[i].vnCanvas[std::get<1>(*primitiveIter)*m_spData->whMatrix[0] + std::get<0>(*primitiveIter)] = std::get<2>(*primitiveIter);
							}
						}
					}

					ImGui::Checkbox(u8"选点", &(sPage[i].bCheckMouse)); ImGui::SameLine();
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
						//>=10
						auto effIter = std::find_if(sPage[i].vnCanvas.begin(), sPage[i].vnCanvas.end(), [](unsigned char v) {return (v != 0); });
						while (effIter != sPage[i].vnCanvas.end())
						{
							int dirc = std::distance(sPage[i].vnCanvas.begin(), effIter);
							LedInt2 effectCoordinate(dirc%m_spData->whMatrix[0], dirc / m_spData->whMatrix[0]);
							sPage[i].vEffectPoints.push_back(std::make_pair(effectCoordinate, *effIter));
							effIter = std::find_if(effIter + 1, sPage[i].vnCanvas.end(), [](unsigned char v) {return (v != 0); });
						}
						sPage[i].bModify = false;
					}

					ImGui::EndTabItem();
				}
				if (sPage[i].bCheckMouse) {
					sPage[i].bModify = true;
					ImGui::GetForegroundDrawList()->AddCircleFilled(ImGui::GetMousePos(), m_spData->cicleSize*0.5f, IM_COL32_WHITE, 32);
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
				isStartPlay = true;
			}
			ImGui::SameLine();
			if (ImGui::Button(u8"关闭演示")) isStartPlay = false;

			break;

		case 1:
			
			if (ImGui::Button(u8"导入")) {
				std::string open_file = this->SelectFile(new VideoFileDialog);
				if (!open_file.empty()) {
					readVideo->SetFileName(open_file);
					loadVideoFileUtf = ToUTF8(open_file);
					std::thread videoThread(&LedReadVideo::Init, readVideo, m_spData->whMatrix, m_spData->rowDict, m_spData->colDict);
					
					videoThread.detach();
					memset(strSuccess, 0, sizeof(strSuccess));
					sprintf(strSuccess, "Loading");
				}
				isStartPlay = false;
			}
			ImGui::SameLine();
			if (ImGui::Button(u8"演示播放")) {
				isVideoPlay = true;
				dbNowTime = ImGui::GetTime();
				frameIndex = 0;
			}
			ImGui::SameLine();
			if (ImGui::Button(u8"关闭演示"))
				isVideoPlay = false;

			readVideo->IsCanPlay() ? ImGui::Text(loadVideoFileUtf.c_str()) : ImGui::Text(strSuccess);
			break;
		}

	}
	ImGui::End();//End model control
}
LedReadVideo* ControlMode::GetVideo()
{
	return readVideo;
}

void ControlMode::ManualReadImage(std::string fileName)
{
	if (backgroundImagePixels != nullptr) {
		delete backgroundImagePixels;
		backgroundImagePixels = nullptr;
		glDeleteTextures(1, &imageTexture);
	}
	cv::Mat bgImage = cv::imread(fileName, cv::IMREAD_UNCHANGED);
	cv::resize(bgImage, bgImage, cv::Size(m_spData->whMatrix[0] * m_spData->rowDict, m_spData->whMatrix[1] * m_spData->colDict));
	backgroundImageArg.width = bgImage.cols;
	backgroundImageArg.height = bgImage.rows;
	backgroundImageArg.channels = bgImage.channels();
	backgroundImagePixels = new unsigned char[backgroundImageArg.width * backgroundImageArg.height * backgroundImageArg.channels];

	memcpy(backgroundImagePixels, bgImage.data, backgroundImageArg.width * backgroundImageArg.height * backgroundImageArg.channels * sizeof(unsigned char));
	
	glGenTextures(1, &imageTexture);
	glBindTexture(GL_TEXTURE_2D, imageTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (backgroundImagePixels != nullptr) {
		if (backgroundImageArg.channels == 3)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, backgroundImageArg.width, backgroundImageArg.height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, (void*)backgroundImagePixels);
		if (backgroundImageArg.channels == 4)
			glTexImage2D(GL_TEXTURE_2D, 0, 4, backgroundImageArg.width, backgroundImageArg.height, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, (void*)backgroundImagePixels);
	}
}

unsigned char * ControlMode::GetImagePixels()
{

	return backgroundImagePixels;
}

Led::Image & ControlMode::GetImageArg()
{
	// TODO: 在此处插入 return 语句
	return backgroundImageArg;
}

unsigned int ControlMode::GetImageTexture()
{
	return imageTexture;
}

void ControlMode::MouseClickDraw(int pageindex)
{
	static int cx = 0;
	static int cy = 0;
	if (ImGui::IsMouseClicked(0)) {
		ImVec2 cpoint = ImGui::GetIO().MouseClickedPos[0];
		cx = (int)((cpoint.x - firstPointx + m_spData->cicleSize * 0.5f) / m_spData->rowDict);
		cy = (int)((cpoint.y - firstPointy + m_spData->cicleSize * 0.5f) / m_spData->colDict);
		if (cx < m_spData->whMatrix[0] && cy < m_spData->whMatrix[1])
			sPage[pageindex].vnCanvas[cy*m_spData->whMatrix[0] + cx] = sPage[pageindex].vnCanvas[cy*m_spData->whMatrix[0] + cx] ? 0 : 255;
	}

	if (ImGui::IsMouseDragging(0)) {
		ImVec2 dragArea = ImGui::GetIO().MousePos;
		//ImVec2 dragArea = ImVec2(ImGui::GetMouseDragDelta(0).x + cpoint.x, ImGui::GetMouseDragDelta(0).y + cpoint.y);
		int dragx = (int)((dragArea.x - firstPointx + m_spData->cicleSize * 0.5f) / m_spData->rowDict);
		int dragy = (int)((dragArea.y - firstPointy + m_spData->cicleSize * 0.5f) / m_spData->colDict);
		if (dragx < m_spData->whMatrix[0] && dragy < m_spData->whMatrix[1])
			sPage[pageindex].vnCanvas[dragy*m_spData->whMatrix[0] + dragx] = sPage[pageindex].vnCanvas[cy*m_spData->whMatrix[0] + cx];

	}
}

std::string ControlMode::SelectFile(LedFileDialog *openFile)
{
	//LedFileDialog *videoFile = new VideoFileDialog;
	std::string filenameUtf8 = openFile->SelectFileNameDialog();
	delete openFile;
	return filenameUtf8;
}
