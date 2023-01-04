#include <array>

#include <VisionTools/Utils/PathParser.h>

#include <VisionUI/Widgets/Texts/TextColored.h>
#include <VisionUI/Widgets/Drags/DragSingleScalar.h>
#include <VisionUI/Widgets/Drags/DragMultipleScalars.h>
#include <VisionUI/Widgets/InputFields/InputText.h>
#include <VisionUI/Widgets/Selection/ColorEdit.h>
#include <VisionUI/Widgets/Layout/Group.h>
#include <VisionUI/Widgets/Layout/Columns.h>
#include <VisionUI/Widgets/Selection/CheckBox.h>
#include <VisionUI/Widgets/Buttons/Button.h>
#include <VisionUI/Widgets/Buttons/ButtonSmall.h>
#include <VisionUI/Plugins/DDTarget.h>

#include <VisionCore/Global/ServiceLocator.h>
#include <VisionCore/ResourceManagement/ModelManager.h>
#include <VisionCore/ResourceManagement/TextureManager.h>
#include <VisionCore/ResourceManagement/ShaderManager.h>
#include <VisionCore/ResourceManagement/MaterialManager.h>
#include <VisionCore/ResourceManagement/SoundManager.h>

#include "VisionCore/Helpers/GUIDrawer.h"

const VisionUI::Types::Color VisionCore::Helpers::GUIDrawer::TitleColor = { 0.85f, 0.65f, 0.0f };
const VisionUI::Types::Color VisionCore::Helpers::GUIDrawer::ClearButtonColor = { 0.5f, 0.0f, 0.0f };
const float VisionCore::Helpers::GUIDrawer::_MIN_FLOAT = -999999999.f;
const float VisionCore::Helpers::GUIDrawer::_MAX_FLOAT = +999999999.f;
VisionRendering::Resources::Texture* VisionCore::Helpers::GUIDrawer::__EMPTY_TEXTURE = nullptr;

void VisionCore::Helpers::GUIDrawer::ProvideEmptyTexture(VisionRendering::Resources::Texture& p_emptyTexture)
{
	__EMPTY_TEXTURE = &p_emptyTexture;
}

void VisionCore::Helpers::GUIDrawer::CreateTitle(VisionUI::Internal::WidgetContainer& p_root, const std::string & p_name)
{
	p_root.CreateWidget<VisionUI::Widgets::Texts::TextColored>(p_name, TitleColor);
}

void VisionCore::Helpers::GUIDrawer::DrawBoolean(VisionUI::Internal::WidgetContainer & p_root, const std::string & p_name, bool & p_data)
{
	CreateTitle(p_root, p_name);
	auto& widget = p_root.CreateWidget<VisionUI::Widgets::Selection::CheckBox>();
	auto& dispatcher = widget.AddPlugin<VisionUI::Plugins::DataDispatcher<bool>>();
	dispatcher.RegisterReference(reinterpret_cast<bool&>(p_data));
}

void VisionCore::Helpers::GUIDrawer::DrawVec2(VisionUI::Internal::WidgetContainer & p_root, const std::string & p_name, VisionMaths::FVector2 & p_data, float p_step, float p_min, float p_max)
{
	CreateTitle(p_root, p_name);
	auto& widget = p_root.CreateWidget<VisionUI::Widgets::Drags::DragMultipleScalars<float, 2>>(GetDataType<float>(), p_min, p_max, 0.f, p_step, "", GetFormat<float>());
	auto& dispatcher = widget.AddPlugin<VisionUI::Plugins::DataDispatcher<std::array<float, 2>>>();
	dispatcher.RegisterReference(reinterpret_cast<std::array<float, 2>&>(p_data));
}

void VisionCore::Helpers::GUIDrawer::DrawVec3(VisionUI::Internal::WidgetContainer & p_root, const std::string & p_name, VisionMaths::FVector3 & p_data, float p_step, float p_min, float p_max)
{
	CreateTitle(p_root, p_name);
	auto& widget = p_root.CreateWidget<VisionUI::Widgets::Drags::DragMultipleScalars<float, 3>>(GetDataType<float>(), p_min, p_max, 0.f, p_step, "", GetFormat<float>());
	auto& dispatcher = widget.AddPlugin<VisionUI::Plugins::DataDispatcher<std::array<float, 3>>>();
	dispatcher.RegisterReference(reinterpret_cast<std::array<float, 3>&>(p_data));
}

void VisionCore::Helpers::GUIDrawer::DrawVec4(VisionUI::Internal::WidgetContainer & p_root, const std::string & p_name, VisionMaths::FVector4& p_data, float p_step, float p_min, float p_max)
{
	CreateTitle(p_root, p_name);
	auto& widget = p_root.CreateWidget<VisionUI::Widgets::Drags::DragMultipleScalars<float, 4>>(GetDataType<float>(), p_min, p_max, 0.f, p_step, "", GetFormat<float>());
	auto& dispatcher = widget.AddPlugin<VisionUI::Plugins::DataDispatcher<std::array<float, 4>>>();
	dispatcher.RegisterReference(reinterpret_cast<std::array<float, 4>&>(p_data));
}

void VisionCore::Helpers::GUIDrawer::DrawQuat(VisionUI::Internal::WidgetContainer & p_root, const std::string & p_name, VisionMaths::FQuaternion & p_data, float p_step, float p_min, float p_max)
{
	CreateTitle(p_root, p_name);
	auto& widget = p_root.CreateWidget<VisionUI::Widgets::Drags::DragMultipleScalars<float, 4>>(GetDataType<float>(), p_min, p_max, 0.f, p_step, "", GetFormat<float>());
	auto& dispatcher = widget.AddPlugin<VisionUI::Plugins::DataDispatcher<std::array<float, 4>>>();
	dispatcher.RegisterReference(reinterpret_cast<std::array<float, 4>&>(p_data));
}

void VisionCore::Helpers::GUIDrawer::DrawString(VisionUI::Internal::WidgetContainer & p_root, const std::string & p_name, std::string & p_data)
{
	CreateTitle(p_root, p_name);
	auto& widget = p_root.CreateWidget<VisionUI::Widgets::InputFields::InputText>("");
	auto& dispatcher = widget.AddPlugin<VisionUI::Plugins::DataDispatcher<std::string>>();
	dispatcher.RegisterReference(p_data);
}

void VisionCore::Helpers::GUIDrawer::DrawColor(VisionUI::Internal::WidgetContainer & p_root, const std::string & p_name, VisionUI::Types::Color & p_color, bool p_hasAlpha)
{
	CreateTitle(p_root, p_name);
	auto& widget = p_root.CreateWidget<VisionUI::Widgets::Selection::ColorEdit>(p_hasAlpha);
	auto& dispatcher = widget.AddPlugin<VisionUI::Plugins::DataDispatcher<VisionUI::Types::Color>>();
	dispatcher.RegisterReference(p_color);
}

VisionUI::Widgets::Texts::Text& VisionCore::Helpers::GUIDrawer::DrawMesh(VisionUI::Internal::WidgetContainer & p_root, const std::string & p_name, VisionRendering::Resources::Model *& p_data, VisionTools::Eventing::Event<>* p_updateNotifier)
{
	CreateTitle(p_root, p_name);

	std::string displayedText = (p_data ? p_data->path : std::string("Empty"));
	auto& rightSide = p_root.CreateWidget<VisionUI::Widgets::Layout::Group>();

	auto& widget = rightSide.CreateWidget<VisionUI::Widgets::Texts::Text>(displayedText);

	widget.AddPlugin<VisionUI::Plugins::DDTarget<std::pair<std::string, VisionUI::Widgets::Layout::Group*>>>("File").DataReceivedEvent += [&widget, &p_data, p_updateNotifier](auto p_receivedData)
	{
		if (VisionTools::Utils::PathParser::GetFileType(p_receivedData.first) == VisionTools::Utils::PathParser::EFileType::MODEL)
		{
			if (auto resource = SERVICE(VisionCore::ResourceManagement::ModelManager).GetResource(p_receivedData.first); resource)
			{
				p_data = resource;
				widget.content = p_receivedData.first;
				if (p_updateNotifier)
					p_updateNotifier->Invoke();
			}
		}
	};

	widget.lineBreak = false;

	auto& resetButton = rightSide.CreateWidget<VisionUI::Widgets::Buttons::ButtonSmall>("Clear");
	resetButton.idleBackgroundColor = ClearButtonColor;
	resetButton.ClickedEvent += [&widget, &p_data, p_updateNotifier]
	{
		p_data = nullptr;
		widget.content = "Empty";
		if (p_updateNotifier)
			p_updateNotifier->Invoke();
	};

	return widget;
}

VisionUI::Widgets::Visual::Image& VisionCore::Helpers::GUIDrawer::DrawTexture(VisionUI::Internal::WidgetContainer & p_root, const std::string & p_name, VisionRendering::Resources::Texture *& p_data, VisionTools::Eventing::Event<>* p_updateNotifier)
{
	CreateTitle(p_root, p_name);

	std::string displayedText = (p_data ? p_data->path : std::string("Empty"));
	auto& rightSide = p_root.CreateWidget<VisionUI::Widgets::Layout::Group>();

	auto& widget = rightSide.CreateWidget<VisionUI::Widgets::Visual::Image>(p_data ? p_data->id : (__EMPTY_TEXTURE ? __EMPTY_TEXTURE->id : 0), VisionMaths::FVector2{ 75, 75 });

	widget.AddPlugin<VisionUI::Plugins::DDTarget<std::pair<std::string, VisionUI::Widgets::Layout::Group*>>>("File").DataReceivedEvent += [&widget, &p_data, p_updateNotifier](auto p_receivedData)
	{
		if (VisionTools::Utils::PathParser::GetFileType(p_receivedData.first) == VisionTools::Utils::PathParser::EFileType::TEXTURE)
		{
			if (auto resource = SERVICE(VisionCore::ResourceManagement::TextureManager).GetResource(p_receivedData.first); resource)
			{
				p_data = resource;
				widget.textureID.id = resource->id;
				if (p_updateNotifier)
					p_updateNotifier->Invoke();
			}
		}
	};

	widget.lineBreak = false;

	auto& resetButton = rightSide.CreateWidget<VisionUI::Widgets::Buttons::Button>("Clear");
	resetButton.idleBackgroundColor = ClearButtonColor;
	resetButton.ClickedEvent += [&widget, &p_data, p_updateNotifier]
	{
		p_data = nullptr;
		widget.textureID.id = (__EMPTY_TEXTURE ? __EMPTY_TEXTURE->id : 0);
		if (p_updateNotifier)
			p_updateNotifier->Invoke();
	};

	return widget;
}

VisionUI::Widgets::Texts::Text& VisionCore::Helpers::GUIDrawer::DrawShader(VisionUI::Internal::WidgetContainer & p_root, const std::string & p_name, VisionRendering::Resources::Shader *& p_data, VisionTools::Eventing::Event<>* p_updateNotifier)
{
	CreateTitle(p_root, p_name);

	std::string displayedText = (p_data ? p_data->path : std::string("Empty"));
	auto& rightSide = p_root.CreateWidget<VisionUI::Widgets::Layout::Group>();

	auto& widget = rightSide.CreateWidget<VisionUI::Widgets::Texts::Text>(displayedText);

	widget.AddPlugin<VisionUI::Plugins::DDTarget<std::pair<std::string, VisionUI::Widgets::Layout::Group*>>>("File").DataReceivedEvent += [&widget, &p_data, p_updateNotifier](auto p_receivedData)
	{
		if (VisionTools::Utils::PathParser::GetFileType(p_receivedData.first) == VisionTools::Utils::PathParser::EFileType::SHADER)
		{
			if (auto resource = SERVICE(VisionCore::ResourceManagement::ShaderManager).GetResource(p_receivedData.first); resource)
			{
				p_data = resource;
				widget.content = p_receivedData.first;
				if (p_updateNotifier)
					p_updateNotifier->Invoke();
			}
		}
	};

	widget.lineBreak = false;

	auto& resetButton = rightSide.CreateWidget<VisionUI::Widgets::Buttons::ButtonSmall>("Clear");
	resetButton.idleBackgroundColor = ClearButtonColor;
	resetButton.ClickedEvent += [&widget, &p_data, p_updateNotifier]
	{
		p_data = nullptr;
		widget.content = "Empty";
		if (p_updateNotifier)
			p_updateNotifier->Invoke();
	};

	return widget;
}

VisionUI::Widgets::Texts::Text& VisionCore::Helpers::GUIDrawer::DrawMaterial(VisionUI::Internal::WidgetContainer & p_root, const std::string & p_name, VisionCore::Resources::Material *& p_data, VisionTools::Eventing::Event<>* p_updateNotifier)
{
	CreateTitle(p_root, p_name);

	std::string displayedText = (p_data ? p_data->path : std::string("Empty"));
	auto& rightSide = p_root.CreateWidget<VisionUI::Widgets::Layout::Group>();

	auto& widget = rightSide.CreateWidget<VisionUI::Widgets::Texts::Text>(displayedText);

	widget.AddPlugin<VisionUI::Plugins::DDTarget<std::pair<std::string, VisionUI::Widgets::Layout::Group*>>>("File").DataReceivedEvent += [&widget, &p_data, p_updateNotifier](auto p_receivedData)
	{
		if (VisionTools::Utils::PathParser::GetFileType(p_receivedData.first) == VisionTools::Utils::PathParser::EFileType::MATERIAL)
		{
			if (auto resource = SERVICE(VisionCore::ResourceManagement::MaterialManager).GetResource(p_receivedData.first); resource)
			{
				p_data = resource;
				widget.content = p_receivedData.first;
				if (p_updateNotifier)
					p_updateNotifier->Invoke();
			}
		}
	};

	widget.lineBreak = false;

	auto& resetButton = rightSide.CreateWidget<VisionUI::Widgets::Buttons::ButtonSmall>("Clear");
	resetButton.idleBackgroundColor = ClearButtonColor;
	resetButton.ClickedEvent += [&widget, &p_data, p_updateNotifier]
	{
		p_data = nullptr;
		widget.content = "Empty";
		if (p_updateNotifier)
			p_updateNotifier->Invoke();
	};

	return widget;
}

VisionUI::Widgets::Texts::Text& VisionCore::Helpers::GUIDrawer::DrawSound(VisionUI::Internal::WidgetContainer& p_root, const std::string& p_name, VisionAudio::Resources::Sound*& p_data, VisionTools::Eventing::Event<>* p_updateNotifier)
{
	CreateTitle(p_root, p_name);

	std::string displayedText = (p_data ? p_data->path : std::string("Empty"));
	auto & rightSide = p_root.CreateWidget<VisionUI::Widgets::Layout::Group>();

	auto & widget = rightSide.CreateWidget<VisionUI::Widgets::Texts::Text>(displayedText);

	widget.AddPlugin<VisionUI::Plugins::DDTarget<std::pair<std::string, VisionUI::Widgets::Layout::Group*>>>("File").DataReceivedEvent += [&widget, &p_data, p_updateNotifier](auto p_receivedData)
	{
		if (VisionTools::Utils::PathParser::GetFileType(p_receivedData.first) == VisionTools::Utils::PathParser::EFileType::SOUND)
		{
			if (auto resource = SERVICE(VisionCore::ResourceManagement::SoundManager).GetResource(p_receivedData.first); resource)
			{
				p_data = resource;
				widget.content = p_receivedData.first;
				if (p_updateNotifier)
					p_updateNotifier->Invoke();
			}
		}
	};

	widget.lineBreak = false;

	auto & resetButton = rightSide.CreateWidget<VisionUI::Widgets::Buttons::ButtonSmall>("Clear");
	resetButton.idleBackgroundColor = ClearButtonColor;
	resetButton.ClickedEvent += [&widget, &p_data, p_updateNotifier]
	{
		p_data = nullptr;
		widget.content = "Empty";
		if (p_updateNotifier)
			p_updateNotifier->Invoke();
	};

	return widget;
}

VisionUI::Widgets::Texts::Text& VisionCore::Helpers::GUIDrawer::DrawAsset(VisionUI::Internal::WidgetContainer& p_root, const std::string& p_name, std::string& p_data, VisionTools::Eventing::Event<>* p_updateNotifier)
{
    CreateTitle(p_root, p_name);

    const std::string displayedText = (p_data.empty() ? std::string("Empty") : p_data);
    auto& rightSide = p_root.CreateWidget<VisionUI::Widgets::Layout::Group>();

    auto& widget = rightSide.CreateWidget<VisionUI::Widgets::Texts::Text>(displayedText);

    widget.AddPlugin<VisionUI::Plugins::DDTarget<std::pair<std::string, VisionUI::Widgets::Layout::Group*>>>("File").DataReceivedEvent += [&widget, &p_data, p_updateNotifier](auto p_receivedData)
    {
        p_data = p_receivedData.first;
        widget.content = p_receivedData.first;
        if (p_updateNotifier)
            p_updateNotifier->Invoke();
    };

    widget.lineBreak = false;

    auto& resetButton = rightSide.CreateWidget<VisionUI::Widgets::Buttons::ButtonSmall>("Clear");
    resetButton.idleBackgroundColor = ClearButtonColor;
    resetButton.ClickedEvent += [&widget, &p_data, p_updateNotifier]
    {
        p_data = "";
        widget.content = "Empty";
        if (p_updateNotifier)
            p_updateNotifier->Invoke();
    };

    return widget;
}

void VisionCore::Helpers::GUIDrawer::DrawBoolean(VisionUI::Internal::WidgetContainer & p_root, const std::string & p_name, std::function<bool(void)> p_gatherer, std::function<void(bool)> p_provider)
{
	CreateTitle(p_root, p_name);
	auto& widget = p_root.CreateWidget<VisionUI::Widgets::Selection::CheckBox>();
	auto& dispatcher = widget.AddPlugin<VisionUI::Plugins::DataDispatcher<bool>>();

	dispatcher.RegisterGatherer([p_gatherer]()
	{
		bool value = p_gatherer();
		return reinterpret_cast<bool&>(value);
	});

	dispatcher.RegisterProvider([p_provider](bool p_value)
	{
		p_provider(reinterpret_cast<bool&>(p_value));
	});
}

void VisionCore::Helpers::GUIDrawer::DrawVec2(VisionUI::Internal::WidgetContainer & p_root, const std::string & p_name, std::function<VisionMaths::FVector2(void)> p_gatherer, std::function<void(VisionMaths::FVector2)> p_provider, float p_step, float p_min, float p_max)
{
	CreateTitle(p_root, p_name);
	auto& widget = p_root.CreateWidget<VisionUI::Widgets::Drags::DragMultipleScalars<float, 2>>(GetDataType<float>(), p_min, p_max, 0.f, p_step, "", GetFormat<float>());
	auto& dispatcher = widget.AddPlugin<VisionUI::Plugins::DataDispatcher<std::array<float, 2>>>();
	
	dispatcher.RegisterGatherer([p_gatherer]()
	{
		VisionMaths::FVector2 value = p_gatherer();
		return reinterpret_cast<const std::array<float, 2>&>(value);
	});

	dispatcher.RegisterProvider([p_provider](std::array<float, 2> p_value)
	{
		p_provider(reinterpret_cast<VisionMaths::FVector2&>(p_value));
	});
}

void VisionCore::Helpers::GUIDrawer::DrawVec3(VisionUI::Internal::WidgetContainer & p_root, const std::string & p_name, std::function<VisionMaths::FVector3(void)> p_gatherer, std::function<void(VisionMaths::FVector3)> p_provider, float p_step, float p_min, float p_max)
{
	CreateTitle(p_root, p_name);
	auto& widget = p_root.CreateWidget<VisionUI::Widgets::Drags::DragMultipleScalars<float, 3>>(GetDataType<float>(), p_min, p_max, 0.f, p_step, "", GetFormat<float>());
	auto& dispatcher = widget.AddPlugin<VisionUI::Plugins::DataDispatcher<std::array<float, 3>>>();

	dispatcher.RegisterGatherer([p_gatherer]()
	{
		VisionMaths::FVector3 value = p_gatherer();
		return reinterpret_cast<const std::array<float, 3>&>(value);
	});

	dispatcher.RegisterProvider([p_provider](std::array<float, 3> p_value)
	{
		p_provider(reinterpret_cast<VisionMaths::FVector3&>(p_value));
	});
}

void VisionCore::Helpers::GUIDrawer::DrawVec4(VisionUI::Internal::WidgetContainer & p_root, const std::string & p_name, std::function<VisionMaths::FVector4(void)> p_gatherer, std::function<void(VisionMaths::FVector4)> p_provider, float p_step, float p_min, float p_max)
{
	CreateTitle(p_root, p_name);
	auto& widget = p_root.CreateWidget<VisionUI::Widgets::Drags::DragMultipleScalars<float, 4>>(GetDataType<float>(), p_min, p_max, 0.f, p_step, "", GetFormat<float>());
	auto& dispatcher = widget.AddPlugin<VisionUI::Plugins::DataDispatcher<std::array<float, 4>>>();
	
	dispatcher.RegisterGatherer([p_gatherer]()
	{
		VisionMaths::FVector4 value = p_gatherer();
		return reinterpret_cast<const std::array<float, 4>&>(value);
	});

	dispatcher.RegisterProvider([p_provider](std::array<float, 4> p_value)
	{
		p_provider(reinterpret_cast<VisionMaths::FVector4&>(p_value));
	});
}

void VisionCore::Helpers::GUIDrawer::DrawQuat(VisionUI::Internal::WidgetContainer & p_root, const std::string & p_name, std::function<VisionMaths::FQuaternion(void)> p_gatherer, std::function<void(VisionMaths::FQuaternion)> p_provider, float p_step, float p_min, float p_max)
{
	CreateTitle(p_root, p_name);
	auto& widget = p_root.CreateWidget<VisionUI::Widgets::Drags::DragMultipleScalars<float, 4>>(GetDataType<float>(), p_min, p_max, 0.f, p_step, "", GetFormat<float>());
	auto& dispatcher = widget.AddPlugin<VisionUI::Plugins::DataDispatcher<std::array<float, 4>>>();
	
	dispatcher.RegisterGatherer([p_gatherer]()
	{
		VisionMaths::FQuaternion value = p_gatherer();
		return reinterpret_cast<const std::array<float, 4>&>(value);
	});

	dispatcher.RegisterProvider([&dispatcher, p_provider](std::array<float, 4> p_value)
	{
		p_provider(VisionMaths::FQuaternion::Normalize(reinterpret_cast<VisionMaths::FQuaternion&>(p_value)));
	});
}

void VisionCore::Helpers::GUIDrawer::DrawDDString(VisionUI::Internal::WidgetContainer& p_root, const std::string& p_name,
	std::function<std::string()> p_gatherer, std::function<void(std::string)> p_provider,
	const std::string& p_identifier)
{
	CreateTitle(p_root, p_name);
	auto& widget = p_root.CreateWidget<VisionUI::Widgets::InputFields::InputText>("");
	auto& dispatcher = widget.AddPlugin<VisionUI::Plugins::DataDispatcher<std::string>>();
	dispatcher.RegisterGatherer(p_gatherer);
	dispatcher.RegisterProvider(p_provider);

	auto& ddTarget = widget.AddPlugin<VisionUI::Plugins::DDTarget<std::pair<std::string, VisionUI::Widgets::Layout::Group*>>>(p_identifier);
	ddTarget.DataReceivedEvent += [&widget, &dispatcher](std::pair<std::string, VisionUI::Widgets::Layout::Group*> p_data)
	{
		widget.content = p_data.first;
		dispatcher.NotifyChange();
	};
}

void VisionCore::Helpers::GUIDrawer::DrawString(VisionUI::Internal::WidgetContainer & p_root, const std::string & p_name, std::function<std::string(void)> p_gatherer, std::function<void(std::string)> p_provider)
{
	CreateTitle(p_root, p_name);
	auto& widget = p_root.CreateWidget<VisionUI::Widgets::InputFields::InputText>("");
	auto& dispatcher = widget.AddPlugin<VisionUI::Plugins::DataDispatcher<std::string>>();
	dispatcher.RegisterGatherer(p_gatherer);
	dispatcher.RegisterProvider(p_provider);
}

void VisionCore::Helpers::GUIDrawer::DrawColor(VisionUI::Internal::WidgetContainer & p_root, const std::string & p_name, std::function<VisionUI::Types::Color(void)> p_gatherer, std::function<void(VisionUI::Types::Color)> p_provider, bool p_hasAlpha)
{
	CreateTitle(p_root, p_name);
	auto& widget = p_root.CreateWidget<VisionUI::Widgets::Selection::ColorEdit>(p_hasAlpha);
	auto& dispatcher = widget.AddPlugin<VisionUI::Plugins::DataDispatcher<VisionUI::Types::Color>>();
	dispatcher.RegisterGatherer(p_gatherer);
	dispatcher.RegisterProvider(p_provider);
}
