#pragma once
#include <map>
#include <cmath>
#include <algorithm>

double wheel = 1.0;
ImVec2 lastMousePos;
bool none = true; 
bool light = true; 
bool heavy = true;
ImVec4 deltas;
namespace ImGui {

	void TextCenter(std::string text) {
		auto windowWidth = ImGui::GetWindowSize().x;
		auto textWidth = ImGui::CalcTextSize(text.c_str()).x;

		ImGui::SetCursorPosX((ImGui::GetStyle().WindowPadding.x / 2) + (windowWidth - textWidth) * 0.5f);
		ImGui::Text(text.c_str());
	}
	ImVec4 get_async_movements()
	{
		ImVec4 delta_movement=ImVec4(0,0,0,0);
		if (GetAsyncKeyState(VK_UP) & 0x8000)
		{
			delta_movement.z += 0.02;
		}
		if (GetAsyncKeyState(VK_DOWN) & 0x8000)
		{
			delta_movement.z -= 0.02;
		}
		if (GetAsyncKeyState('W') & 0x8000)
		{
			delta_movement.y += 5;
		}
		if (GetAsyncKeyState('S') & 0x8000)
		{
			delta_movement.y -= 5;
		}
		if (GetAsyncKeyState('A') & 0x8000)
		{
			delta_movement.x += 5;
		}
		if (GetAsyncKeyState('D') & 0x8000)
		{
			delta_movement.x -= 5;
		}
		return delta_movement;

	}
	void DrawPoints(std::vector<Map_Color>& res)
	{
		ImGuiWindow* window = GetCurrentWindow();
		ImVec2 pos = window->DC.CursorPos;
		
		wheel += ImGui::GetIO().MouseWheel*2;
		double seed = std::fmod(ImGui::GetTime(), 2.0)/2.0;
		ImVec4 tempDelt = get_async_movements();
		deltas.x += tempDelt.x;
		deltas.y += tempDelt.y;
		wheel += tempDelt.z;
		/*if (wheel < 0.0)
			wheel = 0.0;
		else if (wheel > 0.6)
			wheel = 0.6;*/


		for (auto& point: res)
		{
			for (int i =0; i < point.points.size()-1;i++)
			{
				lastMousePos = ImVec2(pos.x+(ImGui::GetIO().MouseWheelH*2)+deltas.x, pos.y+deltas.y);
				ImVec2 p1 = ImVec2(((point.points[i].x+lastMousePos.x) * wheel), ((point.points[i].y+lastMousePos.y) * wheel));
				ImVec2 p2 = ImVec2(((point.points[i+1].x+lastMousePos.x) * wheel), ((point.points[i+1].y + lastMousePos.y) * wheel));
				switch (point.color_region) {
				case RED:
					if (heavy)
					{
						window->DrawList->AddLine(p1, p2, IM_COL32(255, 0, 0, 255), 4.0);
						if (rand()%(100-1)==50) {
							window->DrawList->AddCircle(p1, (25.0 * seed), IM_COL32(255, 0, 0, 255 * (1.0 - seed)), 30.0, 2.0f);
						}
					}else{ window->DrawList->AddLine(p1, p2, IM_COL32(255, 255, 255, 255), 4.0); }
					break;
				case GREEN:
					if (none)
					{
						window->DrawList->AddLine(p1, p2, IM_COL32(0, 255, 0, 255), 4.0);
					}else{ window->DrawList->AddLine(p1, p2, IM_COL32(255, 255, 255, 255), 4.0); }
					break;
				case YELLOW: 
					if (light)
					{
						window->DrawList->AddLine(p1, p2, IM_COL32(255, 255, 0, 255), 4.0);
					}else{ window->DrawList->AddLine(p1, p2, IM_COL32(255, 255, 255, 255),4.0); }
					break;
				default:
					break;
				}
				
			}
			
		}
	}
	bool Spinner(const char* label, float radius, int thickness, const ImU32& color) {
		ImGuiWindow* window = GetCurrentWindow();
		if (window->SkipItems)
			return false;

		ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;
		const ImGuiID id = window->GetID(label);

		ImVec2 pos = window->DC.CursorPos;
		ImVec2 size((radius) * 2, (radius + style.FramePadding.y) * 2);

		const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
		ItemSize(bb, style.FramePadding.y);
		if (!ItemAdd(bb, id))
			return false;

		// Render
		window->DrawList->PathClear();

		int num_segments = 60;
		int start = abs(ImSin(g.Time * 1.8f) * (num_segments - 5));

		const float a_min = IM_PI * 2.0f * ((float)start) / (float)num_segments;
		const float a_max = IM_PI * 2.0f * ((float)num_segments - 3) / (float)num_segments;

		const ImVec2 centre = ImVec2(pos.x + radius, pos.y + radius + style.FramePadding.y);

		for (int i = 0; i < num_segments; i++) {
			const float a = a_min + ((float)i / (float)num_segments) * (a_max - a_min);
			window->DrawList->PathLineTo(ImVec2(centre.x + ImCos(a + g.Time * 8) * radius,
				centre.y + ImSin(a + g.Time * 8) * radius));
		}

		window->DrawList->PathStroke(color, false, thickness);
	}
	bool DrawLine(const char* label, const ImVec2& size_arg_t)
	{
		
		ImVec2 size_arg = ImVec2(size_arg_t.x * wheel, size_arg_t.y * wheel);
		ImGuiWindow* window = GetCurrentWindow();
		ImVec2 curpos = GetCursorPos();
		const ImGuiID id = window->GetID(label);
		ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;
		ImVec2 pos = window->DC.CursorPos;
		ImVec2 size = size_arg;
		size.x -= style.FramePadding.x * 2;

		const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
		ItemSize(bb, style.FramePadding.y);
		window->DrawList->AddRectFilled(pos, ImVec2(pos.x + size.x, pos.y + size.y),IM_COL32(15,100,200,255),size_arg.y);
		if (ImGui::IsItemHovered())
		{
			window->DrawList->AddRectFilled(pos,ImVec2(ImGui::GetMousePos().x,pos.y+(size.y/2)),10.0*wheel, IM_COL32(255, 255, 255, 255), 30);
		}
		wheel += ImGui::GetIO().MouseWheel;
	}
}