#include "DrawManage.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "IconsFontAwesome5.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include <Psapi.h>
#pragma comment(lib, "psapi.lib")

DrawManage::DrawManage(std::shared_ptr<CommonData> commonData, ControlMode *controlMode) :
	row_dist(0),
	col_dist(0),
	firstPointx(0),
	firstPointy(0),
	isInitVertex(false),
	isConcern(false),
	item_current(nullptr),
	zoomProportion(15.0f),
	m_commonData(commonData),
	m_controlMode(controlMode)
{
	inputSize[0] = inputSize[1] = 0;
	manual = LedManualLayout::CreateManualLayout();
	glGenTextures(sizeof(dataLineTexture)/sizeof(unsigned int), dataLineTexture);

	
}


DrawManage::DrawManage()
{
}

DrawManage::~DrawManage()
{
	m_controlMode = nullptr;
	for (int i = 0; i < 16; i++) {
		delete []dataLinePixels[i];
	
	}
	glDeleteTextures(sizeof(dataLineTexture) / sizeof(unsigned int), dataLineTexture);
}

void DrawManage::InitControlWindow(float x, float y, float c_size)
{
	m_commonData->cicleSize = c_size;
	firstPointx = x;
	firstPointy = y;
	if (!ImGui::Begin(ICON_FA_SLIDERS_H" 初始化设置")) {
		ImGui::End();
		return;
	}
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 6));
	ImGui::InputInt2(u8"长X宽", inputSize);
	ImGui::InputFloat(u8"行间距", &row_dist, 0.0f, 0.0f, 2);
	ImGui::InputFloat(u8"列间距", &col_dist, 0.0f, 0.0f, 2);
	if (ImGui::Button(u8"绘制点阵")) {
		m_commonData->lineList.clear();
		m_commonData->whMatrix[0] = inputSize[0];
		m_commonData->whMatrix[1] = inputSize[1];
		m_commonData->rowDict = row_dist * zoomProportion;
		m_commonData->colDict = col_dist * zoomProportion;
		isInitVertex = true;
		isConcern = false;
		m_commonData->comboSelect = 0;
		ClearMode();
	}
	ImGui::SameLine();
	if (ImGui::Button(u8"清除点阵")) {
		m_commonData->whMatrix[0] = inputSize[0] = 0;
		m_commonData->whMatrix[1] = inputSize[1] = 0; 
		m_commonData->rowDict = row_dist = 0.0f;
		m_commonData->colDict = col_dist = 0.0f;
		isInitVertex = false;
		isConcern = false;
		m_commonData->comboSelect = 0;
		m_commonData->lineList.clear();
		ClearMode();
	}

	ImGui::Separator();
	//ImGui::Text(u8"方向选择");
	const char* items[] = { u8"无", u8"上左纵向", u8"上左纵向Z形", u8"上左横向", u8"上左横向Z形", u8"下左纵向", u8"下左纵向Z形", u8"下左横向", u8"下左横向Z形", u8"上右纵向", u8"上右纵向Z形", u8"上右横向", u8"上右横向Z形", u8"下右纵向", u8"下右纵向Z形", u8"下右横向", u8"下右横向Z形", u8"自定义" };

	
	item_current = items[m_commonData->comboSelect];
	if (ImGui::BeginCombo(u8"灯布局方向", item_current))
	{
		for (int n = 0; n < IM_ARRAYSIZE(items); n++)
		{
			bool selected = (item_current == items[n]);
			if (ImGui::Selectable(items[n], selected))
				//item_current = items[n];
				m_commonData->comboSelect = n;
			if (selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}

	if (ImGui::Button(u8"绘制确认")) {
		//Bug?
		//m_commonData->lineList.clear();
		m_commonData->lineList = GetRoute();
		isConcern = true;
		isInitVertex = false;
		this->InitMode();
	}
	if (isConcern) {
		ImGui::SameLine();
		ImGui::Text(u8"设置完成");
	}

	if (m_commonData->comboSelect != 0 && m_commonData->comboSelect != 17)
	ImGui::Image((ImTextureID)dataLineTexture[m_commonData->comboSelect-1], ImVec2(100.0f, 100.0f));

	ImGui::PopStyleVar();
	ImGui::End();
}

void DrawManage::InitMode()
{
	//std::vector<InstancePageData> *manPage = m_controlMode->GetPage();
	if (!m_controlMode->sPage.empty()) {
		m_controlMode->sPage.clear();
	}
	InstancePageData new_obj;
	new_obj.Init(m_commonData->whMatrix, "Way1");
	m_controlMode->sPage.push_back(new_obj);

	m_controlMode->nPageCount = 1;
	m_controlMode->nWhichPage = 0;
}

void DrawManage::ClearMode()
{
	m_controlMode->sPage.clear();
	//m_controlMode->GetVideo()
}

/* 获取路径*/
std::list<LedInt2> DrawManage::GetRoute()
{
	std::list<LedInt2> route_points;

	switch (m_commonData->comboSelect)
	{
	case 0:
		break;
	case 1:
		route_points = MatrixStartTraverseUpDown(0, 0, true, false);
		break;
	case 2:
		route_points = MatrixStartTraverseUpDown(0, 0, true, true);
		break;
	case 3:
		route_points = MatrixStartTraverseLeftRight(0, 0, true, false);
		break;
	case 4:
		route_points = MatrixStartTraverseLeftRight(0, 0, true, true);
		break;
	case 5:
		route_points = MatrixStartTraverseUpDown(0, m_commonData->whMatrix[1] - 1, false, false);
		break;
	case 6:
		route_points = MatrixStartTraverseUpDown(0, m_commonData->whMatrix[1] - 1, false, true);
		break;
	case 7:
		route_points = MatrixStartTraverseLeftRight(0, m_commonData->whMatrix[1] - 1, true, false);
		break;
	case 8:
		route_points = MatrixStartTraverseLeftRight(0, m_commonData->whMatrix[1] - 1, true, true);
		break;
	case 9:
		route_points = MatrixStartTraverseUpDown(m_commonData->whMatrix[0] - 1, 0, true, false);
		break;
	case 10:
		route_points = MatrixStartTraverseUpDown(m_commonData->whMatrix[0] - 1, 0, true, true);
		break;
	case 11:
		route_points = MatrixStartTraverseLeftRight(m_commonData->whMatrix[0] - 1, 0, false, false);
		break;
	case 12:
		route_points = MatrixStartTraverseLeftRight(m_commonData->whMatrix[0] - 1, 0, false, true);
		break;
	case 13:
		route_points = MatrixStartTraverseUpDown(m_commonData->whMatrix[0] - 1, m_commonData->whMatrix[1] - 1, false, false);
		break;
	case 14:
		route_points = MatrixStartTraverseUpDown(m_commonData->whMatrix[0] - 1, m_commonData->whMatrix[1] - 1, false, true);
		break;
	case 15:
		route_points = MatrixStartTraverseLeftRight(m_commonData->whMatrix[0] - 1, m_commonData->whMatrix[1] - 1, false, false);
		break;
	case 16:
		route_points = MatrixStartTraverseLeftRight(m_commonData->whMatrix[0] - 1, m_commonData->whMatrix[1] - 1, false, true);
		break;
	case 17:
		route_points = m_commonData->lineList;
		//ImGui::GetWindowDrawList()->AddCircleFilled(ImVec2(firstPointx + (*route_points.begin()).x*m_commonData->cicleSize, firstPointy + (*route_points.begin()).y*m_commonData->cicleSize), m_commonData->cicleSize*0.5f, IM_COL32_WHITE, 32);
		break;
	}

	return route_points;
}

void DrawManage::ProjectUpdate()
{
	inputSize[0] = m_commonData->whMatrix[0];
	inputSize[1] = m_commonData->whMatrix[1];
	row_dist = m_commonData->rowDict / zoomProportion;
	col_dist = m_commonData->colDict / zoomProportion;
	isConcern = false;
	ClearMode();
}

bool DrawManage::IsDrawFinish()
{
	return isConcern;
}

void DrawManage::InitDataLinePixels()
{
	char runPath[512], runProcess[512];
	memset(runPath, '\0', sizeof(runPath));
	GetModuleFileName(NULL, runProcess, 512);
	memcpy(runPath, runProcess, strlen(runProcess) - 14);
	char aFileName[512];
	
	std::vector<std::string> imageName{".\\resources\\1.png", ".\\resources\\2.png" , ".\\resources\\3.png" , ".\\resources\\4.png" , ".\\resources\\5.png" , ".\\resources\\6.png" , ".\\resources\\7.png" ,".\\resources\\8.png",
		".\\resources\\9.png", ".\\resources\\10.png" , ".\\resources\\11.png" , ".\\resources\\12.png" , ".\\resources\\13.png" , ".\\resources\\14.png" , ".\\resources\\15.png" ,".\\resources\\16.png" };
	
	for (int i = 0; i < 16; i++) {

		sprintf(aFileName, "%s\\resources\\%d.png", runPath, i+1);

		cv::Mat bgImage = cv::imread(aFileName, cv::IMREAD_UNCHANGED);
		cv::resize(bgImage, bgImage, cv::Size(50, 50));
		
		dataLinePixels[i] = new unsigned char[bgImage.cols * bgImage.rows * bgImage.channels()];

		memcpy(dataLinePixels[i], bgImage.data, bgImage.cols * bgImage.rows * bgImage.channels() * sizeof(unsigned char));

		
		glBindTexture(GL_TEXTURE_2D, dataLineTexture[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		if (dataLinePixels[i] != nullptr) {
			if (bgImage.channels() == 3)
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, bgImage.cols, bgImage.rows, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, (void*)dataLinePixels[i]);
			if (bgImage.channels() == 4)
				glTexImage2D(GL_TEXTURE_2D, 0, 4, bgImage.cols, bgImage.rows, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, (void*)dataLinePixels[i]);
		}
	}
}

/* 左右方向移动*/
void DrawManage::RouteMoveLeftRight(int & x, int & y, bool & d, bool &z, int incrse, std::list<LedInt2> &my_list)
{
	if (d) {
		if (x < m_commonData->whMatrix[0] - 1) {
			my_list.push_back(LedInt2(x, y));
			x += 1;
		}
		else {
			my_list.push_back(LedInt2(x, y));
			y += incrse;
			z ? x = 0 : d = !d;
		}
	}
	else {
		if (x > 0) {
			my_list.push_back(LedInt2(x, y));
			x -= 1;
		}
		else {
			my_list.push_back(LedInt2(x, y));
			y += incrse;
			z ? x = m_commonData->whMatrix[0] - 1 : d = !d;
		}
	}
}
/* 上下方向移动*/
void DrawManage::RouteMoveUpDown(int & x, int & y, bool & d, bool & z, int incrse, std::list<LedInt2> &my_list)
{
	if (d) {
		if (y < m_commonData->whMatrix[1] - 1) {
			my_list.push_back(LedInt2(x, y));
			y += 1;
		}
		else {
			my_list.push_back(LedInt2(x, y));
			x += incrse;
			z ? y = 0 : d = !d;
		}
	}
	else {
		if (y > 0) {
			my_list.push_back(LedInt2(x, y));
			y -= 1;
		}
		else {
			my_list.push_back(LedInt2(x, y));
			x += incrse;
			z ? y = m_commonData->whMatrix[1] - 1 : d = !d;
		}
	}


}

std::list<LedInt2> DrawManage::MatrixStartTraverseUpDown(int xStart, int yStart, bool isPstDirection, bool isZDirection)
{
	//ImDrawList *draw_start = ImGui::GetWindowDrawList();
	//draw_start->AddCircleFilled(ImVec2(firstPointx + xStart * m_commonData->cicleSize, firstPointy + yStart * m_commonData->cicleSize), m_commonData->cicleSize*0.5f, IM_COL32_WHITE);
	std::list<LedInt2> tmp_list;

	if (xStart == 0) {
		while (xStart < m_commonData->whMatrix[0]) {
			RouteMoveUpDown(xStart, yStart, isPstDirection, isZDirection, 1, tmp_list);
		}
	}
	else {
		while (xStart >= 0) {
			RouteMoveUpDown(xStart, yStart, isPstDirection, isZDirection, -1, tmp_list);
		}
	}
	return tmp_list;
}

std::list<LedInt2> DrawManage::MatrixStartTraverseLeftRight(int xStart, int yStart, bool isPstDirection, bool isZDirection)
{
	//ImDrawList *draw_start = ImGui::GetWindowDrawList();
	//draw_start->AddCircleFilled(ImVec2(firstPointx + xStart * m_commonData->cicleSize, firstPointy + yStart * m_commonData->cicleSize), m_commonData->cicleSize*0.5f, IM_COL32_WHITE);
	std::list<LedInt2> tmp_list;

	if (!yStart) {
		while (yStart < m_commonData->whMatrix[1]) {
			RouteMoveLeftRight(xStart, yStart, isPstDirection, isZDirection, 1, tmp_list);
		}
	}
	else {
		while (yStart >= 0) {
			RouteMoveLeftRight(xStart, yStart, isPstDirection, isZDirection, -1, tmp_list);
		}
	}
	return tmp_list;
}


