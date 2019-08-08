#pragma once
#include "CustomTypes.h"
#include <string>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>

class LedReadVideo
{
public:
	LedReadVideo();
	~LedReadVideo();

	void Init(std::string initFile, int area[2]);
	void ResizeFrameThreshold(int frameWidth, int frameHeight);
	std::vector<cv::Mat> CutFrameImage(cv::Mat tmpFrame, int nCol, int nRow);
	std::vector<LedInt2> MakePrimitiveInfo(cv::Mat tmpFrame, int nCol, int nRow);
	double GetFrameTime();

public:
	std::vector<cv::Mat> videoFrameVector;

private:
	std::string m_fileName;
	cv::VideoCapture m_video;
	cv::Mat m_videoFrame;
	std::vector<cv::Mat> m_ceilImage;
	std::vector<LedInt2> m_vCoordinate;
	double m_frameTime;
};

