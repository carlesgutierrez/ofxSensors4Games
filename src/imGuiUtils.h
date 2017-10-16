#pragma once

#include "ofMain.h"
#include "ofxImGui.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ImGui Helpers
//
#define IMGUI_WIDGET_SPACING 4
#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))

//---------------------------------------------------------------------
static bool ComboCinder(const char* label, int* current_item, const std::vector<std::string>& items, int height_in_items) {
	// conversion
	bool bSelection = false;
	string itemsNames;
	for (auto item : items)
		itemsNames += item + '\0';
	itemsNames += '\0';

	vector<char> charArray(itemsNames.begin(), itemsNames.end());
	bSelection = ImGui::Combo(label, current_item, (const char*)&charArray[0], height_in_items);//with \0
	return bSelection;
}

/*
void Image_ImGui(ofTexture & texture, const ImVec2& size) {

	ofSetColor(255);
	ImTextureID myId = (ImTextureID)(uintptr_t)texture.texData.textureID;
	ImGui::Image(myId, size);//, uv0, uv1, tint_col, border_col);

							 // getId()
							 // , const ImVec2& uv0, const ImVec2& uv1, const ImVec4& tint_col, const ImVec4& border_col
}
*/

// NB: You can use math functions/operators on ImVec2 if you #define IMGUI_DEFINE_MATH_OPERATORS and       #include "imgui_internal.h"
// Here we only declare simple +/- operators so others don't leak into the demo code.
static inline ImVec2 operator+(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x + rhs.x, lhs.y + rhs.y); }
static inline ImVec2 operator-(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x - rhs.x, lhs.y - rhs.y); }