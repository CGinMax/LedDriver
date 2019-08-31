#pragma once
#include "CustomTypes.h"
#include <string>
#include <vector>
#include <list>
#include <deque>
#include <mutex>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include <tuple>
using vdpoint = std::tuple<int, int, unsigned char>;

class LedReadMedia
{
public:
	//virtual Init(in area[2]);
	virtual ~LedReadMedia();
	virtual cv::Mat Resize2Single(cv::Mat &frameImage, int frameWidth, int frameHeight);
	virtual std::deque<cv::Mat> CutFrameImage(cv::Mat tmpFrame, int nCol, int nRow);
	virtual std::list<vdpoint> MakePrimitiveInfo(cv::Mat tmpFrame, int nCol, int nRow);
	virtual void SetFileName(std::string sFilename) = 0;
private:
};


class LedReadVideo : public LedReadMedia
{
public:
	LedReadVideo();
	~LedReadVideo();

	void Init(int area[2], float row_dict, float col_dict);
	/*cv::Mat ResizeFrameImage(cv::Mat &frameImage, int frameWidth, int frameHeight);
	std::deque<cv::Mat> CutFrameImage(cv::Mat tmpFrame, int nCol, int nRow);
	std::list<vdpoint> MakePrimitiveInfo(cv::Mat tmpFrame, int nCol, int nRow);*/
	void SetFileName(std::string sFilename);
	std::string GetVideoFileName();
	double GetFrameTime();
	int GetFrameCount();

public:
	std::vector<std::list<vdpoint>> m_videoPrimitiveData;
	bool m_isInit;
private:
	std::string m_fileName;
	cv::VideoCapture m_video;
	std::list<cv::Mat> m_videoFrameList;
	double m_frameTime;
	int m_frameCount;
	std::mutex m_mutex;
};

class LedReadImage : public LedReadMedia {
public:
	LedReadImage();
	LedReadImage(std::string file);
	~LedReadImage();
	void Init(int area[2], float row_dict, float col_dict);
	void SetFileName(std::string sFilename);
	std::list<vdpoint>& GetImagePrimitive();
private:
	std::string m_fileName;
	std::list<vdpoint> m_usefulPrimitive;
};

