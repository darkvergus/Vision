#include <GL/glew.h>

#include "VisionEditor/Panels/AView.h"
#include "VisionEditor/Core/EditorActions.h"

VisionEditor::Panels::AView::AView
(
	const std::string& p_title,
	bool p_opened,
	const VisionUI::Settings::PanelWindowSettings& p_windowSettings
) : PanelWindow(p_title, p_opened, p_windowSettings), m_editorRenderer(EDITOR_RENDERER())
{
	m_cameraPosition = { -10.0f, 3.0f, 10.0f };
	m_cameraRotation = VisionMaths::FQuaternion({0.0f, 135.0f, 0.0f});

	m_image = &CreateWidget<VisionUI::Widgets::Visual::Image>(m_fbo.GetTextureID(), VisionMaths::FVector2{ 0.f, 0.f });

    scrollable = false;
}

void VisionEditor::Panels::AView::Update(float p_deltaTime)
{
	auto[winWidth, winHeight] = GetSafeSize();

	m_image->size = VisionMaths::FVector2(static_cast<float>(winWidth), static_cast<float>(winHeight));

	m_fbo.Resize(winWidth, winHeight);
}

void VisionEditor::Panels::AView::_Draw_Impl()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

	VisionUI::Panels::PanelWindow::_Draw_Impl();

	ImGui::PopStyleVar();
}

void VisionEditor::Panels::AView::Render()
{
	FillEngineUBO();

	auto [winWidth, winHeight] = GetSafeSize();

	EDITOR_CONTEXT(shapeDrawer)->SetViewProjection(m_camera.GetProjectionMatrix() * m_camera.GetViewMatrix());

	EDITOR_CONTEXT(renderer)->SetViewPort(0, 0, winWidth, winHeight);

	_Render_Impl();
}

void VisionEditor::Panels::AView::SetCameraPosition(const VisionMaths::FVector3 & p_position)
{
	m_cameraPosition = p_position;
}

void VisionEditor::Panels::AView::SetCameraRotation(const VisionMaths::FQuaternion& p_rotation)
{
	m_cameraRotation = p_rotation;
}

const VisionMaths::FVector3 & VisionEditor::Panels::AView::GetCameraPosition() const
{
	return m_cameraPosition;
}

const VisionMaths::FQuaternion& VisionEditor::Panels::AView::GetCameraRotation() const
{
	return m_cameraRotation;
}

VisionRendering::LowRenderer::Camera & VisionEditor::Panels::AView::GetCamera()
{
	return m_camera;
}

std::pair<uint16_t, uint16_t> VisionEditor::Panels::AView::GetSafeSize() const
{
	auto result = GetSize() - VisionMaths::FVector2{ 0.f, 25.f }; // 25 == title bar height
	return { static_cast<uint16_t>(result.x), static_cast<uint16_t>(result.y) };
}

const VisionMaths::FVector3& VisionEditor::Panels::AView::GetGridColor() const
{
	return m_gridColor;
}

void VisionEditor::Panels::AView::SetGridColor(const VisionMaths::FVector3& p_color)
{
	m_gridColor = p_color;
}

void VisionEditor::Panels::AView::FillEngineUBO()
{
	auto& engineUBO = *EDITOR_CONTEXT(engineUBO);

	auto[winWidth, winHeight] = GetSafeSize();

	size_t offset = sizeof(VisionMaths::FMatrix4); // We skip the model matrix (Which is a special case, modified every draw calls)
	engineUBO.SetSubData(VisionMaths::FMatrix4::Transpose(m_camera.GetViewMatrix()), std::ref(offset));
	engineUBO.SetSubData(VisionMaths::FMatrix4::Transpose(m_camera.GetProjectionMatrix()), std::ref(offset));
	engineUBO.SetSubData(m_cameraPosition, std::ref(offset));
}

void VisionEditor::Panels::AView::PrepareCamera()
{
	auto [winWidth, winHeight] = GetSafeSize();
	m_camera.CacheMatrices(winWidth, winHeight, m_cameraPosition, m_cameraRotation);
}
