#include "LedReadPhoto.h"
#include <algorithm>
#include <exception>
using namespace cv;
LedReadPhoto::LedReadPhoto()
{

}

LedReadPhoto::LedReadPhoto(std::string strFileName)
	: m_fileName(strFileName)
{
	
}

LedReadPhoto::~LedReadPhoto()
{
}

void LedReadPhoto::Init(std::string initFile, int area[2])
{
	ReadImageFile(initFile);
	ResizeImage(area[0]*16, area[1]*16);
	CutImage(area[0], area[1]);
	MakePrimitiveInfo(area[0]);
}

void LedReadPhoto::ResizeImage(int imageWidth, int imageHeight)
{
	if (imageWidth <= 0 || imageHeight <= 0)
		throw std::invalid_argument("The Size of image is not zero!");
	
	if (m_sourceImage.data) {
		threshold(m_sourceImage, m_sourceImage, 125, 255, CV_THRESH_BINARY);
		resize(m_sourceImage, m_sourceImage, Size(imageWidth, imageHeight));
	}
	//imwrite("C:\\Users\\CGinMax\\Desktop\\darkearth.jpg", m_sourceImage);
	
}

void LedReadPhoto::CutImage(int nCol, int nRow)
{
	if (nCol <= 0 || nRow <= 0)
		throw std::invalid_argument("The Size of image is not zero!");

	int nCeilWidth = m_sourceImage.cols / nCol;
	int nCeilHeight = m_sourceImage.rows / nRow;
	Mat roi_img, cuted_img;
	for (int j = 0; j < nRow; j++) {
		for (int i = 0; i < nCol; i++) {
			//m_sourceImage(tmprect).copyTo(roi_img);
			cv::Rect tmprect(i*nCeilWidth, j*nCeilHeight, nCeilWidth, nCeilHeight);
			cuted_img = cv::Mat(m_sourceImage, tmprect);
			roi_img = cuted_img.clone();
			m_ceilImage.push_back(roi_img);
		}
	}
	//cv::imwrite("C:\\Users\\CGinMax\\Desktop\\ledimage\\0.jpg", m_ceilImage[0]);
}

std::vector<Mat> LedReadPhoto::GetCeilImageSet()
{
	/*if (!m_ceilImage.empty())
		return m_ceilImage;

	return NULL;*/
	return m_ceilImage;
}

std::vector<LedInt2> LedReadPhoto::GetPrimitiveSet()
{
	return vCoordinate;
}

bool LedReadPhoto::ReadImageFile(std::string destFile)
{
	m_fileName = destFile;
	m_sourceImage = imread(m_fileName, 0);
	if (m_sourceImage.data) {
		return true;
		
	}
	return false;
}

void LedReadPhoto::MakePrimitiveInfo(int nCols)
{
	for (size_t i = 0; i < m_ceilImage.size(); i++) {
		int mc = 0;
		for (cv::Mat_<uchar>::iterator point_iter = m_ceilImage.at(i).begin<uchar>(); point_iter != m_ceilImage.at(i).end<uchar>(); point_iter++) {
			if ((int)(*point_iter) > 125)
				mc++;
		}

		int nEff = mc > ((m_ceilImage[i].rows*m_ceilImage[i].cols) / 4) ? 1 : 0;
		if (nEff) {
			vCoordinate.push_back(LedInt2(i / nCols, i % nCols));
		}
	}

}
