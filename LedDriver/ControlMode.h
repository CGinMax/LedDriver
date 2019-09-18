#pragma once
#include "InstancePageData.h"
#include "CustomTypes.h"
#include "LedReadVideo.h"
#include "LedFileDialog.h"
#include "LanguageSetting.h"
#include <string>
class ControlMode
{
	
public:
	ControlMode();
	ControlMode(std::shared_ptr<CommonData> m_spData);
	~ControlMode();
	void ModeSelectWindow(float _x, float _y);
	LedReadVideo* GetVideo();
	void ManualReadImage(std::string fileName);
	unsigned char* GetImagePixels();
	Led::Image &GetImageArg();
	unsigned int GetImageTexture();
private:
	void MouseClickDraw(int pageindex);
	//std::string SelectFileNameDialog(std::string &fileType);
	std::string SelectFile(LedFileDialog *openFile);
	std::string ToUTF8(const std::string str);
	void InitDefaultEffect();
	void WayConcern(size_t idx);
public:
	int nCurrentMode; 
	int nPageCount;
	int nWhichPage;
	size_t nIntervalNum;
	bool isBackgroundShow;
	bool isStartPlay;
	bool isVideoPlay;
	double dbNowTime;
	size_t frameIndex;

	std::vector<InstancePageData> sPage;
private:
	float firstPointx;
	float firstPointy;
	unsigned char *backgroundImagePixels;
	char strSuccess[10];
	unsigned int imageTexture;
	LedReadVideo *readVideo;
	std::shared_ptr<CommonData> m_spData;

	std::string loadBackgroundImageFileUtf;
	std::string loadVideoFileUtf;
	Led::Image backgroundImageArg;
	//const char* effectNames[7] = { u8"球运动", u8"扩散跳球", u8"X运动", u8"左向右拉幕", u8"上向下拉幕", u8"正方形扩散", u8"渐变" };
	std::vector<std::string> effectPath;
	LanguageSetting *cm_language;
};

