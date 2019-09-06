#pragma once

#include "CustomTypes.h"
#include "LedManualLayout.h"
#include "ControlMode.h"
#include <stack>
#include <list>
#include <mutex>

class DrawManage
{
public:
	DrawManage();
	DrawManage(std::shared_ptr<CommonData> commonData, ControlMode *controlMode);
	~DrawManage();

public:
	void InitControlWindow(float x, float y, float c_size);
	std::list<LedInt2> GetRoute();
	void ProjectUpdate();
	bool IsDrawFinish();
	void InitDataLinePixels();
public:
	bool isInitVertex;
private:
	int inputSize[2];
	float row_dist;
	float col_dist;
	float firstPointx;
	float firstPointy;
	const char *item_current;
	bool isConcern;
	float zoomProportion;
	unsigned int dataLineTexture[16];
	unsigned char *dataLinePixels[16];
	LedManualLayout *manual;
	ControlMode *m_controlMode;
	std::shared_ptr<CommonData> m_commonData;
	std::mutex m_mutex;
	

private:
	void InitMode();
	void ClearMode();
	void RouteMoveLeftRight(int & x, int & y, bool & d, bool &z, int incrse, std::list<LedInt2> &my_list);
	void RouteMoveUpDown(int & x, int & y, bool & d, bool &z, int incrse, std::list<LedInt2> &my_list);
	std::list<LedInt2> MatrixStartTraverseUpDown(int xStart, int yStart, bool isPstDirection, bool isZDirection);
	std::list<LedInt2> MatrixStartTraverseLeftRight(int xStart, int yStart, bool isPstDirection, bool isZDirection);

};



