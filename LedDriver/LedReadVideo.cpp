#include "LedReadVideo.h"
using namespace cv;


LedReadVideo::LedReadVideo()
{
	m_frameTime = 0.0;
	m_frameCount = 0;
	m_isInit = false;
}


LedReadVideo::~LedReadVideo()
{
	
}

void LedReadVideo::Init(int area[2])
{
	m_isInit = false;
	if (!m_videoFrameList.empty()) {
		m_videoFrameList.clear();
		m_videoPrimitiveData.clear();
	}
	cv::Mat videoFrame;
	m_video.open(m_fileName);
	while (m_video.read(videoFrame)) {
		ResizeFrameThreshold(videoFrame, area[0] * 16, area[1] * 16);				
	}
	m_frameTime = 1000.0 / m_video.get(CV_CAP_PROP_FPS);
	m_frameCount = (int)m_video.get(CV_CAP_PROP_FRAME_COUNT);
	m_video.release();

	std::list<cv::Mat>::iterator frameiter = m_videoFrameList.begin();
	while (frameiter != m_videoFrameList.end()) {
		std::list<LedInt2> framePriDt = MakePrimitiveInfo(*frameiter, area[0], area[1]);
		m_videoPrimitiveData.push_back(framePriDt);
		frameiter++;
	}
	m_videoPrimitiveData.shrink_to_fit();
	m_isInit = true;
}

void LedReadVideo::ResizeFrameThreshold(cv::Mat frameImage, int frameWidth, int frameHeight)
{
	if (frameImage.data) {
		cvtColor(frameImage, frameImage, COLOR_BGR2GRAY);
		cv::threshold(frameImage, frameImage, 125, 255, CV_THRESH_BINARY);
		resize(frameImage, frameImage, Size(frameWidth, frameHeight));
		m_videoFrameList.push_back(frameImage);
	}
}

std::deque<cv::Mat> LedReadVideo::CutFrameImage(cv::Mat tmpFrame, int nCol, int nRow)
{
	int nCeilWidth = tmpFrame.cols / nCol;
	int nCeilHeight = tmpFrame.rows / nRow;
	Mat roi_img, cuted_img;
	std::deque<Mat> ceilFrame;
	for (int j = 0; j < nRow; j++) {
		for (int i = 0; i < nCol; i++) {

			cv::Rect tmprect(i*nCeilWidth, j*nCeilHeight, nCeilWidth, nCeilHeight);
			cuted_img = cv::Mat(tmpFrame, tmprect);
			roi_img = cuted_img.clone();
			ceilFrame.push_back(roi_img);
		}
	}
	return ceilFrame;
}

std::list<LedInt2> LedReadVideo::MakePrimitiveInfo(cv::Mat tmpFrame, int nCol, int nRow)
{
	std::list<LedInt2> frameCoordinate;
	std::deque<Mat> vCutedFrame = CutFrameImage(tmpFrame, nCol, nRow);
	for (size_t i = 0; i < vCutedFrame.size(); i++) {
		
		if (cv::countNonZero(vCutedFrame[i]) > 64) {
			frameCoordinate.push_back(LedInt2(i % nCol, i / nCol));
		}
		
	}

	return frameCoordinate;
}

void LedReadVideo::SetVideoFileName(std::string sFilename)
{
	m_fileName = sFilename;
}

std::string LedReadVideo::GetVideoFileName()
{
	return m_fileName;
}

double LedReadVideo::GetFrameTime()
{
	return m_frameTime;
}

int LedReadVideo::GetFrameCount()
{
	
	return m_frameCount;
}
