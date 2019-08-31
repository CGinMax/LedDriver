#include "CustomTypes.h"

CommonData::CommonData()
{
	this->Init();
}

CommonData::~CommonData()
{
	if (!lineList.empty())
		lineList.clear();
}

void CommonData::Init()
{
	cicleSize = 12.0f;
	rowDict = 0.0f;
	colDict = 0.0f;
	comboSelect = 0;
	lineList.clear();
	whMatrix[0] = whMatrix[1] = 0;
}
