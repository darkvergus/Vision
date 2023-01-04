#include "VisionEditor/Panels/MaterialEditor.h"
#include "VisionEditor/Panels/AssetView.h"

#include "VisionEditor/Core/EditorActions.h"

#include <VisionCore/Resources/Loaders/MaterialLoader.h>
#include <VisionCore/Helpers/GUIDrawer.h>
#include <VisionUI/Widgets/Layout/Columns.h>
#include <VisionUI/Widgets/Layout/GroupCollapsable.h>
#include <VisionUI/Widgets/Visual/Separator.h>
#include <VisionUI/Widgets/Texts/TextColored.h>
#include <VisionUI/Widgets/Buttons/Button.h>
#include <VisionUI/Widgets/Buttons/ButtonSmall.h>
#include <VisionUI/Widgets/Selection/ColorEdit.h>

using namespace VisionUI::Panels;
using namespace VisionUI::Widgets;
using namespace VisionCore::Helpers;

void DrawHybridVec3(VisionUI::Internal::WidgetContainer& p_root, const std::string& p_name, VisionMaths::FVector3& p_data, float p_step, float p_min, float p_max)
{
	VisionCore::Helpers::GUIDrawer::CreateTitle(p_root, p_name);

	auto& rightSide = p_root.CreateWidget<VisionUI::Widgets::Layout::Group>();

	auto& xyzWidget = rightSide.CreateWidget<VisionUI::Widgets::Drags::DragMultipleScalars<float, 3>>(VisionCore::Helpers::GUIDrawer::GetDataType<float>(), p_min, p_max, 0.f, p_step, "", VisionCore::Helpers::GUIDrawer::GetFormat<float>());
	auto& xyzDispatcher = xyzWidget.AddPlugin<VisionUI::Plugins::DataDispatcher<std::array<float, 3>>>();
	xyzDispatcher.RegisterReference(reinterpret_cast<std::array<float, 3>&>(p_data));
	xyzWidget.lineBreak = false;

	auto& rgbWidget = rightSide.CreateWidget<VisionUI::Widgets::Selection::ColorEdit>(false, VisionUI::Types::Color{ p_data.x, p_data.y, p_data.z });
	auto& rgbDispatcher = rgbWidget.AddPlugin<VisionUI::Plugins::DataDispatcher<VisionUI::Types::Color>>();
	rgbDispatcher.RegisterReference(reinterpret_cast<VisionUI::Types::Color&>(p_data));
	rgbWidget.enabled = false;
	rgbWidget.lineBreak = false;

	auto& xyzButton = rightSide.CreateWidget<VisionUI::Widgets::Buttons::Button>("XYZ");
	xyzButton.idleBackgroundColor = { 0.7f, 0.5f, 0.0f };
	xyzButton.lineBreak = false;

	auto& rgbButton = rightSide.CreateWidget<VisionUI::Widgets::Buttons::Button>("RGB");
	rgbButton.idleBackgroundColor = { 0.7f, 0.5f, 0.0f };

	xyzButton.ClickedEvent += [&]
	{
		xyzWidget.enabled = true;
		rgbWidget.enabled = false;
	};

	rgbButton.ClickedEvent += [&]
	{
		xyzWidget.enabled = false;
		rgbWidget.enabled = true;
	};
}

void DrawHybridVec4(VisionUI::Internal::WidgetContainer& p_root, const std::string& p_name, VisionMaths::FVector4& p_data, float p_step, float p_min, float p_max)
{
	VisionCore::Helpers::GUIDrawer::CreateTitle(p_root, p_name);

	auto& rightSide = p_root.CreateWidget<VisionUI::Widgets::Layout::Group>();

	auto& xyzWidget = rightSide.CreateWidget<VisionUI::Widgets::Drags::DragMultipleScalars<float, 4>>(VisionCore::Helpers::GUIDrawer::GetDataType<float>(), p_min, p_max, 0.f, p_step, "", VisionCore::Helpers::GUIDrawer::GetFormat<float>());
	auto& xyzDispatcher = xyzWidget.AddPlugin<VisionUI::Plugins::DataDispatcher<std::array<float, 4>>>();
	xyzDispatcher.RegisterReference(reinterpret_cast<std::array<float, 4>&>(p_data));
	xyzWidget.lineBreak = false;

	auto& rgbaWidget = rightSide.CreateWidget<VisionUI::Widgets::Selection::ColorEdit>(true, VisionUI::Types::Color{ p_data.x, p_data.y, p_data.z, p_data.w });
	auto& rgbaDispatcher = rgbaWidget.AddPlugin<VisionUI::Plugins::DataDispatcher<VisionUI::Types::Color>>();
	rgbaDispatcher.RegisterReference(reinterpret_cast<VisionUI::Types::Color&>(p_data));
	rgbaWidget.enabled = false;
	rgbaWidget.lineBreak = false;

	auto& xyzwButton = rightSide.CreateWidget<VisionUI::Widgets::Buttons::Button>("XYZW");
	xyzwButton.idleBackgroundColor = { 0.7f, 0.5f, 0.0f };
	xyzwButton.lineBreak = false;

	auto& rgbaButton = rightSide.CreateWidget<VisionUI::Widgets::Buttons::Button>("RGBA");
	rgbaButton.idleBackgroundColor = { 0.7f, 0.5f, 0.0f };

	xyzwButton.ClickedEvent += [&]
	{
		xyzWidget.enabled = true;
		rgbaWidget.enabled = false;
	};

	rgbaButton.ClickedEvent += [&]
	{
		xyzWidget.enabled = false;
		rgbaWidget.enabled = true;
	};
}

VisionEditor::Panels::MaterialEditor::MaterialEditor
(
	const std::string& p_title,
	bool p_opened,
	const VisionUI::Settings::PanelWindowSettings& p_windowSettings
) :
	PanelWindow(p_title, p_opened, p_windowSettings)
{
	CreateHeaderButtons();
	CreateWidget<VisionUI::Widgets::Visual::Separator>();
	CreateMaterialSelector();
	m_settings = &CreateWidget<Layout::Group>();
	CreateShaderSelector();
	CreateMaterialSettings();
	CreateShaderSettings();

	m_settings->enabled = false;
	m_shaderSettings->enabled = false;

	m_materialDroppedEvent	+= std::bind(&MaterialEditor::OnMaterialDropped, this);
	m_shaderDroppedEvent	+= std::bind(&MaterialEditor::OnShaderDropped, this);
}

void VisionEditor::Panels::MaterialEditor::Refresh()
{
	if (m_target)
		SetTarget(*m_target);
}

void VisionEditor::Panels::MaterialEditor::SetTarget(VisionCore::Resources::Material & p_newTarget)
{
	m_target = &p_newTarget;
	m_targetMaterialText->content = m_target->path;
	OnMaterialDropped();
}

VisionCore::Resources::Material * VisionEditor::Panels::MaterialEditor::GetTarget() const
{
	return m_target;
}

void VisionEditor::Panels::MaterialEditor::RemoveTarget()
{
	m_target = nullptr;
	m_targetMaterialText->content = "Empty";
	OnMaterialDropped();
}

void VisionEditor::Panels::MaterialEditor::Preview()
{
	auto& assetView = EDITOR_PANEL(Panels::AssetView, "Asset View");

	if (m_target)
		assetView.SetResource(m_target);

	assetView.Open();
}

void VisionEditor::Panels::MaterialEditor::Reset()
{
	if (m_target && m_shader)
	{
		m_target->SetShader(nullptr);
		OnShaderDropped();
	}
}

void VisionEditor::Panels::MaterialEditor::OnMaterialDropped()
{
	m_settings->enabled = m_target; // Enable m_settings group if the target material is non-null

	if (m_settings->enabled)
	{
		GenerateMaterialSettingsContent();
		m_shaderText->content = m_target->GetShader() ? m_target->GetShader()->path : "Empty";
		m_shader = m_target->GetShader();
	}
	else
	{
		m_materialSettingsColumns->RemoveAllWidgets();
	}

	m_shaderSettings->enabled = false;
	m_shaderSettingsColumns->RemoveAllWidgets();

	if (m_target && m_target->GetShader())
		OnShaderDropped();
}

void VisionEditor::Panels::MaterialEditor::OnShaderDropped()
{
	m_shaderSettings->enabled = m_shader; // Enable m_shaderSettings group if the shader of the target material is non-null

	if (m_shader != m_target->GetShader())
		m_target->SetShader(m_shader);

	if (m_shaderSettings->enabled)
	{
		GenerateShaderSettingsContent();
	}
	else
	{
		m_shaderSettingsColumns->RemoveAllWidgets();
	}
}

void VisionEditor::Panels::MaterialEditor::CreateHeaderButtons()
{
	auto& saveButton = CreateWidget<Buttons::Button>("Save to file");
	saveButton.idleBackgroundColor = { 0.0f, 0.5f, 0.0f };
	saveButton.ClickedEvent += [this]
	{
		if (m_target)
			VisionCore::Resources::Loaders::MaterialLoader::Save(*m_target, EDITOR_EXEC(GetRealPath(m_target->path)));
	};

	saveButton.lineBreak = false;

	auto& reloadButton = CreateWidget<Buttons::Button>("Reload from file");
	reloadButton.idleBackgroundColor = { 0.7f, 0.5f, 0.0f };
	reloadButton.ClickedEvent += [this]
	{
		if (m_target)
			VisionCore::Resources::Loaders::MaterialLoader::Reload(*m_target, EDITOR_EXEC(GetRealPath(m_target->path)));

		OnMaterialDropped();
	};

	reloadButton.lineBreak = false;

	auto& previewButton = CreateWidget<Buttons::Button>("Preview");
	previewButton.idleBackgroundColor = { 0.7f, 0.5f, 0.0f };
	previewButton.ClickedEvent += std::bind(&MaterialEditor::Preview, this);
	previewButton.lineBreak = false;

	auto& resetButton = CreateWidget<Buttons::Button>("Reset to default");
	resetButton.idleBackgroundColor = { 0.5f, 0.0f, 0.0f };
	resetButton.ClickedEvent += std::bind(&MaterialEditor::Reset, this);
}

void VisionEditor::Panels::MaterialEditor::CreateMaterialSelector()
{
	auto& columns = CreateWidget<VisionUI::Widgets::Layout::Columns<2>>();
	columns.widths[0] = 150;
	m_targetMaterialText = &GUIDrawer::DrawMaterial(columns, "Material", m_target, &m_materialDroppedEvent);
}

void VisionEditor::Panels::MaterialEditor::CreateShaderSelector()
{
	auto& columns = m_settings->CreateWidget<VisionUI::Widgets::Layout::Columns<2>>();
	columns.widths[0] = 150;
	m_shaderText = &GUIDrawer::DrawShader(columns, "Shader", m_shader, &m_shaderDroppedEvent);
}

void VisionEditor::Panels::MaterialEditor::CreateMaterialSettings()
{
	m_materialSettings = &m_settings->CreateWidget<Layout::GroupCollapsable>("Material Settings");
	m_materialSettingsColumns = &m_materialSettings->CreateWidget<VisionUI::Widgets::Layout::Columns<2>>();
	m_materialSettingsColumns->widths[0] = 150;
}

void VisionEditor::Panels::MaterialEditor::CreateShaderSettings()
{
	m_shaderSettings = &m_settings->CreateWidget<Layout::GroupCollapsable>("Shader Settings");
	m_shaderSettingsColumns = &m_shaderSettings->CreateWidget<VisionUI::Widgets::Layout::Columns<2>>();
	m_shaderSettingsColumns->widths[0] = 150;
}

std::string UniformFormat(const std::string& p_string)
{
	std::string result;
	std::string formattedInput = p_string;

	if (formattedInput.rfind("u_", 0) == 0 || formattedInput.rfind("U_", 0) == 0)
	{
		formattedInput = formattedInput.substr(2);
	}

	std::string capsChars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	
	bool first = true;
	bool capsNext = false;

	for (char c : formattedInput)
	{
		if (first || capsNext)
		{
			c = toupper(c);
			first = false;
			capsNext = false;
		}

		if (c == '_')
		{
			c = ' ';
			capsNext = true;
		}

		if (std::find(capsChars.begin(), capsChars.end(), c) != capsChars.end())
			result.push_back(' ');

		result.push_back(c);
	}

	return result;
}

void VisionEditor::Panels::MaterialEditor::GenerateShaderSettingsContent()
{
	using namespace VisionRendering::Resources;

	m_shaderSettingsColumns->RemoveAllWidgets(); // Ensure that the m_shaderSettingsColumns is empty

	std::multimap<int, std::pair<std::string, std::any*>> sortedUniformsData;

	for (auto&[name, value] : m_target->GetUniformsData())
	{
		int orderID = 0;

		auto uniformData = m_target->GetShader()->GetUniformInfo(name);

		if (uniformData)
		{
			switch (uniformData->type)
			{
			case UniformType::UNIFORM_SAMPLER_2D:	orderID = 0; break;
			case UniformType::UNIFORM_FLOAT_VEC4:	orderID = 1; break;
			case UniformType::UNIFORM_FLOAT_VEC3:	orderID = 2; break;
			case UniformType::UNIFORM_FLOAT_VEC2:	orderID = 3; break;
			case UniformType::UNIFORM_FLOAT:		orderID = 4; break;
			case UniformType::UNIFORM_INT:			orderID = 5; break;
			case UniformType::UNIFORM_BOOL:			orderID = 6; break;
			}

			sortedUniformsData.emplace(orderID, std::pair<std::string, std::any*>{ name, & value });
		}
	}

	for (auto& [order, info] : sortedUniformsData)
	{
		auto uniformData = m_target->GetShader()->GetUniformInfo(info.first);
		
		if (uniformData)
		{
			switch (uniformData->type)
			{
			case UniformType::UNIFORM_BOOL:			GUIDrawer::DrawBoolean(*m_shaderSettingsColumns, UniformFormat(info.first), reinterpret_cast<bool&>(*info.second));																	break;
			case UniformType::UNIFORM_INT:			GUIDrawer::DrawScalar<int>(*m_shaderSettingsColumns, UniformFormat(info.first), reinterpret_cast<int&>(*info.second));																break;
			case UniformType::UNIFORM_FLOAT:		GUIDrawer::DrawScalar<float>(*m_shaderSettingsColumns, UniformFormat(info.first), reinterpret_cast<float&>(*info.second), 0.01f, GUIDrawer::_MIN_FLOAT, GUIDrawer::_MAX_FLOAT);		break;
			case UniformType::UNIFORM_FLOAT_VEC2:	GUIDrawer::DrawVec2(*m_shaderSettingsColumns, UniformFormat(info.first), reinterpret_cast<VisionMaths::FVector2&>(*info.second), 0.01f, GUIDrawer::_MIN_FLOAT, GUIDrawer::_MAX_FLOAT);	break;
			case UniformType::UNIFORM_FLOAT_VEC3:	DrawHybridVec3(*m_shaderSettingsColumns, UniformFormat(info.first), reinterpret_cast<VisionMaths::FVector3&>(*info.second), 0.01f, GUIDrawer::_MIN_FLOAT, GUIDrawer::_MAX_FLOAT);			break;
			case UniformType::UNIFORM_FLOAT_VEC4:	DrawHybridVec4(*m_shaderSettingsColumns, UniformFormat(info.first), reinterpret_cast<VisionMaths::FVector4&>(*info.second), 0.01f, GUIDrawer::_MIN_FLOAT, GUIDrawer::_MAX_FLOAT);			break;
			case UniformType::UNIFORM_SAMPLER_2D:	GUIDrawer::DrawTexture(*m_shaderSettingsColumns, UniformFormat(info.first), reinterpret_cast<Texture * &>(*info.second));																break;
			}
		}
	}
}

void VisionEditor::Panels::MaterialEditor::GenerateMaterialSettingsContent()
{
	m_materialSettingsColumns->RemoveAllWidgets(); // Ensure that the m_shaderSettingsColumns is empty

	GUIDrawer::DrawBoolean(*m_materialSettingsColumns, "Blendable", std::bind(&VisionCore::Resources::Material::IsBlendable, m_target), std::bind(&VisionCore::Resources::Material::SetBlendable, m_target, std::placeholders::_1));
	GUIDrawer::DrawBoolean(*m_materialSettingsColumns, "Back-face Culling", std::bind(&VisionCore::Resources::Material::HasBackfaceCulling, m_target), std::bind(&VisionCore::Resources::Material::SetBackfaceCulling, m_target, std::placeholders::_1));
	GUIDrawer::DrawBoolean(*m_materialSettingsColumns, "Front-face Culling", std::bind(&VisionCore::Resources::Material::HasFrontfaceCulling, m_target), std::bind(&VisionCore::Resources::Material::SetFrontfaceCulling, m_target, std::placeholders::_1));
	GUIDrawer::DrawBoolean(*m_materialSettingsColumns, "Depth Test", std::bind(&VisionCore::Resources::Material::HasDepthTest, m_target), std::bind(&VisionCore::Resources::Material::SetDepthTest, m_target, std::placeholders::_1));
	GUIDrawer::DrawBoolean(*m_materialSettingsColumns, "Depth Writing", std::bind(&VisionCore::Resources::Material::HasDepthWriting, m_target), std::bind(&VisionCore::Resources::Material::SetDepthWriting, m_target, std::placeholders::_1));
	GUIDrawer::DrawBoolean(*m_materialSettingsColumns, "Color Writing", std::bind(&VisionCore::Resources::Material::HasColorWriting, m_target), std::bind(&VisionCore::Resources::Material::SetColorWriting, m_target, std::placeholders::_1));
	GUIDrawer::DrawScalar<int>(*m_materialSettingsColumns, "GPU Instances", std::bind(&VisionCore::Resources::Material::GetGPUInstances, m_target), std::bind(&VisionCore::Resources::Material::SetGPUInstances, m_target, std::placeholders::_1), 1.0f, 0, 100000);
}
