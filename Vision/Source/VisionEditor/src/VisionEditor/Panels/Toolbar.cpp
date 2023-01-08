#include <VisionUI/Widgets/Layout/Spacing.h>

#include "VisionEditor/Panels/Toolbar.h"
#include "VisionEditor/Core/EditorActions.h"

#include <VisionCore/Global/ServiceLocator.h>
#include <VisionCore/ResourceManagement/TextureManager.h>

VisionEditor::Panels::Toolbar::Toolbar
(
	const std::string& p_title,
	bool p_opened,
	const VisionUI::Settings::PanelWindowSettings& p_windowSettings
) : PanelWindow(p_title, p_opened, p_windowSettings)
{
	std::string iconFolder = ":Textures/Icons/";

	auto& textureManager = VisionCore::Global::ServiceLocator::Get<VisionCore::ResourceManagement::TextureManager>();

	m_playButton	= &CreateWidget<VisionUI::Widgets::Buttons::ButtonImage>(EDITOR_CONTEXT(editorResources)->GetTexture("Button_Play")->id, VisionMaths::FVector2{ 20, 20 });
	m_pauseButton	= &CreateWidget<VisionUI::Widgets::Buttons::ButtonImage>(EDITOR_CONTEXT(editorResources)->GetTexture("Button_Pause")->id, VisionMaths::FVector2{ 20, 20 });
	m_stopButton	= &CreateWidget<VisionUI::Widgets::Buttons::ButtonImage>(EDITOR_CONTEXT(editorResources)->GetTexture("Button_Stop")->id, VisionMaths::FVector2{ 20, 20 });
	m_nextButton	= &CreateWidget<VisionUI::Widgets::Buttons::ButtonImage>(EDITOR_CONTEXT(editorResources)->GetTexture("Button_Next")->id, VisionMaths::FVector2{ 20, 20 });

	CreateWidget<VisionUI::Widgets::Layout::Spacing>(0).lineBreak = false;
	auto& refreshButton	= CreateWidget<VisionUI::Widgets::Buttons::ButtonImage>(EDITOR_CONTEXT(editorResources)->GetTexture("Button_Refresh")->id, VisionMaths::FVector2{ 20, 20 });

	m_playButton->lineBreak		= false;
	m_pauseButton->lineBreak	= false;
	m_stopButton->lineBreak		= false;
	m_nextButton->lineBreak		= false;
	refreshButton.lineBreak		= false;

	m_playButton->ClickedEvent	+= EDITOR_BIND(StartPlaying);
	m_pauseButton->ClickedEvent	+= EDITOR_BIND(PauseGame);
	m_stopButton->ClickedEvent	+= EDITOR_BIND(StopPlaying);
	m_nextButton->ClickedEvent	+= EDITOR_BIND(NextFrame);
	refreshButton.ClickedEvent	+= EDITOR_BIND(RefreshScripts);

	EDITOR_EVENT(EditorModeChangedEvent) += [this](VisionEditor::Core::EditorActions::EEditorMode p_newMode)
	{
		auto enable = [](VisionUI::Widgets::Buttons::ButtonImage* p_button, bool p_enable)
		{
			p_button->disabled = !p_enable;
			p_button->tint = p_enable ? VisionUI::Types::Color{ 1.0f, 1.0f, 1.0f, 1.0f} : VisionUI::Types::Color{1.0f, 1.0f, 1.0f, 0.15f};
		};

		switch (p_newMode)
		{
		case VisionEditor::Core::EditorActions::EEditorMode::EDIT:
			enable(m_playButton, true);
			enable(m_pauseButton, false);
			enable(m_stopButton, false);
			enable(m_nextButton, false);
			break;
		case VisionEditor::Core::EditorActions::EEditorMode::PLAY:
			enable(m_playButton, false);
			enable(m_pauseButton, true);
			enable(m_stopButton, true);
			enable(m_nextButton, true);
			break;
		case VisionEditor::Core::EditorActions::EEditorMode::PAUSE:
			enable(m_playButton, true);
			enable(m_pauseButton, false);
			enable(m_stopButton, true);
			enable(m_nextButton, true);
			break;
		case VisionEditor::Core::EditorActions::EEditorMode::FRAME_BY_FRAME:
			enable(m_playButton, true);
			enable(m_pauseButton, false);
			enable(m_stopButton, true);
			enable(m_nextButton, true);
			break;
		}
	};

	EDITOR_EXEC(SetEditorMode(VisionEditor::Core::EditorActions::EEditorMode::EDIT));
}

void VisionEditor::Panels::Toolbar::_Draw_Impl()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10, 10));

	VisionUI::Panels::PanelWindow::_Draw_Impl();
	ImGui::PopStyleVar();
}