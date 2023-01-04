#include "VisionUI/Plugins/ContextualMenu.h"

void VisionUI::Plugins::ContextualMenu::Execute()
{
	if (ImGui::BeginPopupContextItem())
	{
		DrawWidgets();
		ImGui::EndPopup();
	}
}

void VisionUI::Plugins::ContextualMenu::Close()
{
	ImGui::CloseCurrentPopup();
}
