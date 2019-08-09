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

	void Init(std::string initFile, int area[2]);
	void ResizeFrameThreshold(int frameWidth, int frameHeight);
	std::deque<cv::Mat> CutFrameImage(cv::Mat tmpFrame, int nCol, int nRow);
	std::list<LedInt2> MakePrimitiveInfo(cv::Mat tmpFrame, int nCol, int nRow);
	std::list<cv::Mat> GetFrameList();
	double GetFrameTime();

public:
	std::list<cv::Mat> videoFrameList;

private:
	std::string m_fileName;
	cv::VideoCapture m_video;
	cv::Mat m_videoFrame;
	std::list<LedInt2> m_vCoordinate;
	double m_frameTime;
};

