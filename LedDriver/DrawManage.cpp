#include "DrawManage.h"



DrawManage::DrawManage()
{
	nMatrixArea[0] = nMatrixArea[1] = 0;
}


DrawManage::~DrawManage()
{
}

std::stack<LedInt2> DrawManage::GetOrder()
{
	return sOutputOrder;
}

void DrawManage::SetOrder(int rs)
{
}

