#include "LedManualLayout.h"
#include <cstdio>

#include "imgui_internal.h"

LedManualLayout *LedManualLayout::ledmanuallayout = nullptr;

LedManualLayout::LedManualLayout() :
	bAddingLine(false),
	fLatticeSize(16.0f)
{
}


LedManualLayout::~LedManualLayout()
{
}

LedManualLayout * LedManualLayout::CreateManualLayout()
{
	if (ledmanuallayout == nullptr)
		ledmanuallayout = new LedManualLayout();

	return ledmanuallayout;
}

void LedManualLayout::DrawWindow(bool * p_open)
{
	ImGuiWindowFlags background_dockspace_flag = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	ImGui::Begin("Manual Background", p_open, background_dockspace_flag);

	ImGuiID dockspace_id = ImGui::GetID("Manual Dockspace");
	if (ImGui::DockBuilderGetNode(dockspace_id) == NULL)
	{
		ImGui::DockBuilderRemoveNode(dockspace_id);
		ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);
		ImGui::DockBuilderSetNodeSize(dockspace_id, ImVec2(500.0f, 500.0f));

		ImGuiID dock_main_id = dockspace_id;
		ImGuiID dock_id_left = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left, 0.35f, NULL, &dock_main_id);

		ImGui::DockBuilderDockWindow(u8"手工布局窗口", dock_main_id);
		ImGui::DockBuilderDockWindow(u8"布局选项", dock_id_left);
		ImGui::DockBuilderFinish(dockspace_id);
	}

	ImGui::Begin(u8"布局选项", false);
	ImGui::Text("Hello");

	ImGui::End();//End LayoutOption

	ImGui::Begin(u8"手工布局窗口", false, ImGuiWindowFlags_HorizontalScrollbar);
	ImDrawList *man_draw_list = ImGui::GetWindowDrawList();
	
	//static ImVector<ImVec2> points;

	ImVec2 canvas_pos = ImGui::GetCursorScreenPos();
	ImVec2 canvas_size = ImGui::GetContentRegionAvail();        // Resize canvas to what's available
	if (canvas_size.x < 50.0f) canvas_size.x = 50.0f;
	if (canvas_size.y < 50.0f) canvas_size.y = 50.0f;
	man_draw_list->AddRectFilledMultiColor(canvas_pos, ImVec2(canvas_pos.x + canvas_size.x, canvas_pos.y + canvas_size.y), IM_COL32(50, 50, 50, 255), IM_COL32(50, 50, 60, 255), IM_COL32(60, 60, 70, 255), IM_COL32(50, 50, 60, 255));
	man_draw_list->AddRect(canvas_pos, ImVec2(canvas_pos.x + canvas_size.x, canvas_pos.y + canvas_size.y), IM_COL32(255, 255, 255, 255));


	ImGui::InvisibleButton("canvas", canvas_size);
	ImVec2 mouse_pos_in_canvas = ImVec2(ImGui::GetIO().MousePos.x - canvas_pos.x, ImGui::GetIO().MousePos.y - canvas_pos.y);
	if (bAddingLine)
	{
		
		//points.push_back(mouse_pos_in_canvas);
		ivRectPoints[1] = mouse_pos_in_canvas;

		ImVec2 firstcentralpoint = ImVec2(canvas_pos.x + ivRectPoints[0].x + fLatticeSize * 0.5f, canvas_pos.y + ivRectPoints[0].y + fLatticeSize * 0.5f);
		int cols = (ivRectPoints[1].x - ivRectPoints[0].x) / fLatticeSize;
		int rows = (ivRectPoints[1].y - ivRectPoints[0].y) / fLatticeSize;
		for (int j = 0; j < rows; j++) {
			for (int i = 0; i < cols; i++) {
				man_draw_list->AddCircle(ImVec2(firstcentralpoint.x + i * fLatticeSize, firstcentralpoint.y + j * fLatticeSize), fLatticeSize*0.5f, IM_COL32_WHITE, 32, 2.0f);
			}
		}
		char textcoord[10];
		sprintf(textcoord, "(%d,%d)", cols, rows);
		man_draw_list->AddText(ImVec2(ImGui::GetIO().MousePos.x + 20.0f, ImGui::GetIO().MousePos.y - 20.0f), IM_COL32_WHITE, textcoord);


		if (ImGui::IsMouseReleased(0)) {
			
		}
		if (!ImGui::IsMouseDown(0)) {
			bAddingLine = false;

		}

	}
	if (ImGui::IsItemHovered())
	{
		if (!bAddingLine && ImGui::IsMouseClicked(0))
		{
			//points.push_back(mouse_pos_in_canvas);
			ivRectPoints[0] = ivRectPoints[1] = mouse_pos_in_canvas;
			bAddingLine = true;
		}
	}
	//draw_list->PushClipRect(canvas_pos, ImVec2(canvas_pos.x + canvas_size.x, canvas_pos.y + canvas_size.y), true);      // clip lines within the canvas (if we resize it, etc.)
	//for (int i = 0; i < points.Size - 1; i += 2)
	//	draw_list->AddRect(ImVec2(canvas_pos.x + points[i].x, canvas_pos.y + points[i].y), ImVec2(canvas_pos.x + points[i + 1].x, canvas_pos.y + points[i + 1].y), IM_COL32(255, 255, 0, 255), 0.0f, ImDrawCornerFlags_All, 2.0f);
	//draw_list->PopClipRect();
	//if (adding_preview)
	//	points.pop_back();
	if (bAddingLine) man_draw_list->AddRect(ImVec2(canvas_pos.x + ivRectPoints[0].x, canvas_pos.y + ivRectPoints[0].y), ImVec2(canvas_pos.x + ivRectPoints[1].x, canvas_pos.y + ivRectPoints[1].y), IM_COL32(255, 255, 0, 255), 0.0f, ImDrawCornerFlags_All, 2.0f);



	ImGui::End();//End ManualLayout

	ImGui::End();//End Manual Background
}
