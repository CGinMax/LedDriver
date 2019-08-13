#pragma once
#include "CustomTypes.h"
#include <string>
#include <vector>
#include <list>
#include <deque>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>

class LedReadVideo
{
public:
	LedReadVideo();
	~LedReadVideo();

	void Init(int area[2]);
	void ResizeFrameThreshold(cv::Mat frameImage, int frameWidth, int frameHeight);
	std::deque<cv::Mat> CutFrameImage(cv::Mat tmpFrame, int nCol, int nRow);
	std::list<LedInt2> MakePrimitiveInfo(cv::Mat tmpFrame, int nCol, int nRow);
	void SetVideoFileName(std::string sFilename);
	std::string GetVideoFileName();
	double GetFrameTime();
	int GetFrameCount();

public:
	std::vector<std::list<LedInt2>> m_videoPrimitiveData;
	bool m_isInit;
private:
	std::string m_fileName;
	cv::VideoCapture m_video;
	std::list<cv::Mat> m_videoFrameList;
	double m_frameTime;
	int m_frameCount;
};

