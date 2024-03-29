#include "LedReadVideo.h"

using namespace cv;

LedReadMedia::~LedReadMedia()
{
}

cv::Mat LedReadMedia::Resize2Single(cv::Mat & frameImage, int frameWidth, int frameHeight)
{
	
	std::vector<Mat> single_channel;
	resize(frameImage, frameImage, Size(frameWidth, frameHeight));
	split(frameImage, single_channel);
	Mat singleProcessImage;
	frameImage.channels() == 4 ? singleProcessImage = single_channel[2].clone() : singleProcessImage = single_channel[2].clone();
	
	return singleProcessImage;
	
}

std::deque<cv::Mat> LedReadMedia::CutFrameImage(cv::Mat tmpFrame, int nCol, int nRow)
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

std::list<vdpoint> LedReadMedia::MakePrimitiveInfo(cv::Mat tmpFrame, int nCol, int nRow)
{
	std::list<vdpoint> frameCoordinate;
	std::deque<Mat> vCutedFrame = CutFrameImage(tmpFrame, nCol, nRow);
	for (size_t i = 0; i < vCutedFrame.size(); i++) {

		if (cv::countNonZero(vCutedFrame[i]) > ((nCol/2) * (nRow/2))) {
			//���
			int frame_rows = vCutedFrame[i].rows;
			int frame_cols = vCutedFrame[i].cols;
			//auto p = std::make_tuple(int(i % nCol), int(i / nCol), vCutedFrame[i].ptr<unsigned char>(frame_rows / 2)[frame_cols / 2]);
			auto p = std::make_tuple(int(i % nCol), int(i / nCol), vCutedFrame[i].ptr<unsigned char>(12)[12]);
			frameCoordinate.push_back(p);
			//frameCoordinate.push_back(LedInt2(i % nCol, i / nCol));
		}

	}

	return frameCoordinate;
}

LedReadVideo::LedReadVideo()
{
	m_frameTime = 0.0;
	m_frameCount = 0;
	m_isInit = false;
}


LedReadVideo::~LedReadVideo()
{
	
}

void LedReadVideo::Init(int area[2], float row_dict, float col_dict)
{
	m_isInit = false;
	if (!m_videoPrimitiveData.empty()) {
		//m_videoFrameList.clear();
		m_videoPrimitiveData.clear();
	}
	m_frameCount = 0;
	cv::Mat videoFrame;
	if (!m_video.open(m_fileName)) return;
	int frame_width = area[0] * int(row_dict);
	int frame_height = area[1] * int(col_dict);
	m_frameTime = m_video.get(CV_CAP_PROP_FPS);
	m_frameTime = 1000.0 / m_video.get(CV_CAP_PROP_FPS);
	//m_frameCount = static_cast<int>(m_video.get(CV_CAP_PROP_FRAME_COUNT));
	
	while (m_video.read(videoFrame)) {
		
		++m_frameCount;
		std::lock_guard<std::mutex> thread_lock(m_mutex);
		Mat resizeFrame = Resize2Single(videoFrame, frame_width, frame_height).clone();
		std::list<vdpoint> framePriDt = MakePrimitiveInfo(resizeFrame, area[0], area[1]);
		m_videoPrimitiveData.push_back(framePriDt);
	}
	
	//m_frameCount = static_cast<int>(m_video.get(CV_CAP_PROP_FRAME_COUNT));
	if (m_video.get(CV_CAP_PROP_FRAME_COUNT) > 0) m_frameCount = static_cast<int>(m_video.get(CV_CAP_PROP_FRAME_COUNT));
	m_video.release();

	
	m_isInit = true;
}

void LedReadVideo::SetFileName(std::string sFilename)
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

bool LedReadVideo::IsCanPlay()
{
	if (m_frameCount > 200)
		return m_videoPrimitiveData.size() > 200;

	return m_isInit;
}

LedReadImage::LedReadImage()
{
	
}

LedReadImage::LedReadImage(std::string file) : 
	m_fileName(file)
{
}

LedReadImage::~LedReadImage()
{
}

void LedReadImage::Init(int area[2], float row_dict, float col_dict)
{
	if (!m_usefulPrimitive.empty())
		m_usefulPrimitive.clear();

	Mat srcImage = imread(m_fileName, IMREAD_UNCHANGED);

	std::vector<Mat> single_channel;
	resize(srcImage, srcImage, Size(area[0] * int(row_dict), area[1] * int(col_dict)));
	split(srcImage, single_channel);

	Mat singleProcessImage;
	srcImage.channels() == 4 ? singleProcessImage = single_channel[2].clone() : singleProcessImage = single_channel[0].clone();

	m_usefulPrimitive = MakePrimitiveInfo(singleProcessImage, area[0], area[1]);
	//m_usefulPrimitive = MakePrimitiveInfo(Resize2Single(srcImage, area[0] * int(cicleSize), area[1] * int(cicleSize)), area[0], area[1]);
}

//cv::Mat LedReadImage::ResizeFrameImage(cv::Mat &frameImage, int frameWidth, int frameHeight)
//{
//	if (frameImage.data) {
//		resize(frameImage, frameImage, Size(frameWidth, frameHeight));
//	}
//}

void LedReadImage::SetFileName(std::string sFilename)
{
	m_fileName = sFilename;
}

std::list<vdpoint>& LedReadImage::GetImagePrimitive()
{
	return m_usefulPrimitive;
}


