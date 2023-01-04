#include <VisionTools/Utils/PathParser.h>
#include <VisionUI/Plugins/DDTarget.h>

#include "VisionEditor/Core/EditorRenderer.h"
#include "VisionEditor/Core/EditorActions.h"
#include "VisionEditor/Panels/AssetView.h"


VisionEditor::Panels::AssetView::AssetView
(
	const std::string& p_title,
	bool p_opened,
	const VisionUI::Settings::PanelWindowSettings& p_windowSettings
) : AViewControllable(p_title, p_opened, p_windowSettings)
{
	m_camera.SetClearColor({ 0.098f, 0.098f, 0.098f });
	m_camera.SetFar(5000.0f);

	m_resource = static_cast<VisionRendering::Resources::Model*>(nullptr);
	m_image->AddPlugin<VisionUI::Plugins::DDTarget<std::pair<std::string, VisionUI::Widgets::Layout::Group*>>>("File").DataReceivedEvent += [this](auto p_data)
	{
		std::string path = p_data.first;

		switch (VisionTools::Utils::PathParser::GetFileType(path))
		{
		case VisionTools::Utils::PathParser::EFileType::MODEL:
			if (auto resource = VisionCore::Global::ServiceLocator::Get<VisionCore::ResourceManagement::ModelManager>().GetResource(path); resource)
				m_resource = resource;
			break;
		case VisionTools::Utils::PathParser::EFileType::TEXTURE:
			if (auto resource = VisionCore::Global::ServiceLocator::Get<VisionCore::ResourceManagement::TextureManager>().GetResource(path); resource)
				m_resource = resource;
			break;

		case VisionTools::Utils::PathParser::EFileType::MATERIAL:
			if (auto resource = VisionCore::Global::ServiceLocator::Get<VisionCore::ResourceManagement::MaterialManager>().GetResource(path); resource)
				m_resource = resource;
			break;
		}
	};
}

void VisionEditor::Panels::AssetView::_Render_Impl()
{
	PrepareCamera();

	auto& baseRenderer = *EDITOR_CONTEXT(renderer).get();

	m_fbo.Bind();

	baseRenderer.SetStencilMask(0xFF);
	baseRenderer.Clear(m_camera);
	baseRenderer.SetStencilMask(0x00);

	uint8_t glState = baseRenderer.FetchGLState();
	baseRenderer.ApplyStateMask(glState);

	m_editorRenderer.RenderGrid(m_cameraPosition, m_gridColor);

	if (auto pval = std::get_if<VisionRendering::Resources::Model*>(&m_resource); pval && *pval)
		m_editorRenderer.RenderModelAsset(**pval);
	
	if (auto pval = std::get_if<VisionRendering::Resources::Texture*>(&m_resource); pval && *pval)
		m_editorRenderer.RenderTextureAsset(**pval);
	
	if (auto pval = std::get_if<VisionCore::Resources::Material*>(&m_resource); pval && *pval)
		m_editorRenderer.RenderMaterialAsset(**pval);

	baseRenderer.ApplyStateMask(glState);

	m_fbo.Unbind();
}

void VisionEditor::Panels::AssetView::SetResource(ViewableResource p_resource)
{
	m_resource = p_resource;
}

VisionEditor::Panels::AssetView::ViewableResource VisionEditor::Panels::AssetView::GetResource() const
{
	return m_resource;
}
