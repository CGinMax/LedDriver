#include "LedManualLayout.h"
#include <cstdio>
#include <functional>
#include "imgui.h"
#include "IconsFontAwesome5.h"
#include "imgui_internal.h"

//LedManualLayout *LedManualLayout::ledmanuallayout = nullptr;
LedManualLayout *LedManualLayout::ledmanuallayout = new LedManualLayout;

LedManualLayout::LedManualLayout() :
	bAddingLine(false),
	bInitCanvas(false),
	bCheckLine(false),
	bSettingDone(false),
	fLatticeSize(16.0f),
	rowDict(0.0f),
	colDict(0.0f)
{
	area[0] = area[1] = 2;
}


LedManualLayout::~LedManualLayout()
{
	if (ledmanuallayout != nullptr) {
		delete ledmanuallayout;
		ledmanuallayout = NULL;
	}
}

LedManualLayout * LedManualLayout::CreateManualLayout()
{
	/*if (ledmanuallayout == nullptr)
		ledmanuallayout = new LedManualLayout();*/

	return ledmanuallayout;
}

void LedManualLayout::DrawWindow(bool * p_open)
{
	ImGuiIO& io = ImGui::GetIO();
	if (io.KeysDown[341]) {
		if (io.MouseWheel == 1) fLatticeSize += 2.0f; else if (io.MouseWheel == -1) fLatticeSize -= 2.0f;
	}
	ImGui::SetNextWindowSize(ImVec2(700, 500), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowPos(ImVec2(200, 200), ImGuiCond_FirstUseEver);
	ImGui::Begin(ICON_FA_HAND_PAPER" 手工布局窗口", p_open, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_MenuBar);
	

	ImDrawList *man_draw_list = ImGui::GetWindowDrawList();
	
	ImGui::Checkbox(u8"检查连线", &bCheckLine);
	ImGui::SameLine();
	if (ImGui::Button(u8"完成布局")) {
		bSettingDone = true;
	}

	ImVec2 canvas_pos = ImGui::GetCursorScreenPos();
	ImVec2 canvas_size = ImGui::GetContentRegionAvail();        // Resize canvas to what's available
	if (canvas_size.x < 50.0f) canvas_size.x = 50.0f;
	if (canvas_size.y < 50.0f) canvas_size.y = 50.0f;
	man_draw_list->AddRectFilledMultiColor(canvas_pos, ImVec2(canvas_pos.x + rowDict * area[0] + 30.0f, canvas_pos.y + colDict * area[1] + 30.0f), IM_COL32(50, 50, 50, 255), IM_COL32(50, 50, 60, 255), IM_COL32(60, 60, 70, 255), IM_COL32(50, 50, 60, 255));
	man_draw_list->AddRect(canvas_pos, ImVec2(canvas_pos.x + rowDict * area[0] + 30.0f, canvas_pos.y + colDict * area[1] + 30.0f), IM_COL32_WHITE);

	ImVec2 first_point(canvas_pos.x + fLatticeSize*0.5f + 3.5f, canvas_pos.y + fLatticeSize * 0.5f + 3.5f);
	
	if (!bCheckLine) {
		for (int i = 0; i < area[1]; i++) {
			for (int j = 0; j < area[0]; j++) {
				man_draw_list->AddCircle(
					ImVec2(first_point.x + j * rowDict, first_point.y + i * colDict), fLatticeSize*0.5f, IM_COL32_WHITE, 32, 2.0f);
			}
		}
		//画第一个点
		man_draw_list->AddCircleFilled(ImVec2(first_point.x + (*lCoordinate.begin()).x *rowDict, first_point.y + (*lCoordinate.begin()).y*colDict), fLatticeSize*0.5f, IM_COL32_WHITE, 32);
		std::list<LedInt2>::iterator line_iter1, line_iter2;
		line_iter1 = line_iter2 = lCoordinate.begin();

		line_iter2++;
		while (line_iter2 != lCoordinate.end()) {
			man_draw_list->AddLine(ImVec2(first_point.x + (*line_iter1).x*rowDict, first_point.y + (*line_iter1).y*colDict),
				ImVec2(first_point.x + (*line_iter2).x*rowDict, first_point.y + (*line_iter2).y*colDict), IM_COL32(90, 250, 250, 255), 2.0f);
			line_iter1++;
			line_iter2++;
		}
	}
	else {
		for (auto check_iter = lCoordinate.begin(); check_iter != lCoordinate.end(); check_iter++) {
			man_draw_list->AddCircleFilled(ImVec2(first_point.x + (*check_iter).x*rowDict, first_point.y + (*check_iter).y*colDict), fLatticeSize*0.5f, IM_COL32_WHITE, 32);
		}
	}

	ImGui::InvisibleButton("canvas", ImVec2(rowDict * area[0] + 30.0f, colDict * area[1] + 30.0f));
	LedInt2 mouse_point((int)((io.MousePos.x - first_point.x + fLatticeSize*0.5f) / rowDict), (int)((io.MousePos.y - first_point.y + fLatticeSize * 0.5f) / colDict));
	if (bAddingLine)
	{
		liRectPoints[1] = mouse_point;

		if (ImGui::IsMouseReleased(0)) {
			auto line_point_iter1 = std::find(lCoordinate.begin(), lCoordinate.end(), liRectPoints[0]);
			auto line_point_iter2 = std::find(lCoordinate.begin(), lCoordinate.end(), liRectPoints[1]);
			if (line_point_iter1 != lCoordinate.end() && line_point_iter2 != lCoordinate.end())
			{
				//iter1 小于 iter2，删除iter1->iter2，否则相反
				int point_dist1 = std::distance(lCoordinate.begin(), line_point_iter1);
				int point_dist2 = std::distance(lCoordinate.begin(), line_point_iter2);
				if (point_dist1 < point_dist2) {
					if ((point_dist2 - point_dist1) > 1) lCoordinate.erase(++line_point_iter1, line_point_iter2);
				}
				else {
					if((point_dist1 - point_dist2) > 1) lCoordinate.erase(++line_point_iter2, line_point_iter1);
				}
				
			}
			else if (line_point_iter1 != lCoordinate.end() && line_point_iter2 == lCoordinate.end())
			{
				line_point_iter1++;
				lCoordinate.insert(line_point_iter1, liRectPoints[1]);
			}
			else if (line_point_iter1 == lCoordinate.end() && line_point_iter2 != lCoordinate.end())
			{
				lCoordinate.insert(line_point_iter2, liRectPoints[0]);
			}
		}
		if (!ImGui::IsMouseDown(0)) {
			bAddingLine = false;

		}
	}
	if (ImGui::IsItemHovered())
	{
		ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
		if (!bAddingLine && ImGui::IsMouseClicked(0))
		{
			liRectPoints[0] = liRectPoints[1] = mouse_point;
			bAddingLine = true;
		}
	}
	if (bAddingLine) man_draw_list->AddLine(ImVec2(first_point.x + liRectPoints[0].x*rowDict, first_point.y + liRectPoints[0].y*colDict), ImVec2(first_point.x + liRectPoints[1].x*rowDict, first_point.y + liRectPoints[1].y*colDict), IM_COL32(255, 255, 0, 255), 2.0f);

	

	ImGui::End();//End ManualLayout

}

void LedManualLayout::SetCoordinate(int area[2], float cicle_size, float row_dict, float col_dict, std::list<LedInt2> tlist)
{
	if (!lCoordinate.empty())
		lCoordinate.clear();
	lCoordinate.assign(tlist.begin(), tlist.end());
	this->area[0] = area[0];
	this->area[1] = area[1];
	this->fLatticeSize = cicle_size;
	this->rowDict = row_dict;
	this->colDict = col_dict;
	bSettingDone = false;
}

std::list<LedInt2> LedManualLayout::GetLineDirection()
{
	return this->lCoordinate;
}

bool LedManualLayout::IsInitCanvas()
{
	return bInitCanvas;
}

bool LedManualLayout::IsSettingDone()
{
	return bSettingDone;
}

void LedManualLayout::SetStatus(bool status)
{
	bInitCanvas = status;
}
