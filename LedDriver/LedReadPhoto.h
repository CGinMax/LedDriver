#pragma once
#include "CustomTypes.h"
#include <string>
#include <vector>
#include <opencv2/opencv.hpp>

class LedReadPhoto
{
public:
	LedReadPhoto();
	LedReadPhoto(std::string strFileName);
	~LedReadPhoto();

	virtual void Init(std::string initFile, int area[2]);
	virtual bool ReadImageFile(std::string destFile);
	virtual void ResizeImage(int imageWidth, int imageHeight);
	virtual void CutImage(int nCol, int nRow);
	virtual void MakePrimitiveInfo(int nCols);
	virtual std::vector<cv::Mat> GetCeilImageSet();
	virtual std::vector<LedInt2> GetPrimitiveSet();
protected:
	std::vector<LedInt2> vCoordinate;
	cv::Mat m_sourceImage;
	std::vector<cv::Mat> m_ceilImage;
	std::string m_fileName;

};

