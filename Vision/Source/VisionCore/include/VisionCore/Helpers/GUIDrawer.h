#pragma once

#include <VisionMaths/FVector2.h>
#include <VisionMaths/FVector3.h>
#include <VisionMaths/FVector4.h>
#include <VisionMaths/FQuaternion.h>

#include <VisionUI/Internal/WidgetContainer.h>
#include <VisionUI/Widgets/Texts/Text.h>
#include <VisionUI/Widgets/Drags/DragSingleScalar.h>
#include <VisionUI/Widgets/Drags/DragMultipleScalars.h>
#include <VisionUI/Widgets/InputFields/InputText.h>
#include <VisionUI/Widgets/Visual/Image.h>
#include <VisionUI/Types/Color.h>

namespace VisionCore::Resources
{
	class Material;
}

namespace VisionAudio::Resources
{
	class Sound;
}

namespace VisionRendering::Resources
{
	class Model;
	class Shader;
	class Texture;
}

namespace VisionCore::Helpers
{
	/**
	* Provide some helpers to draw UI elements
	*/
	class GUIDrawer
	{
	public:
		static const VisionUI::Types::Color TitleColor;
		static const VisionUI::Types::Color ClearButtonColor;

		static const float _MIN_FLOAT;
		static const float _MAX_FLOAT;

		/**
		* Defines the texture to use when there is no texture in a texture resource field
		* @param p_emptyTexture
		*/
		static void ProvideEmptyTexture(VisionRendering::Resources::Texture& p_emptyTexture);

		/**
		* Draw a title with the title color
		* @param p_root
		* @param p_name
		*/
		static void CreateTitle(VisionUI::Internal::WidgetContainer& p_root, const std::string& p_name);
	
		template <typename T>
		static void DrawScalar(VisionUI::Internal::WidgetContainer& p_root, const std::string& p_name, T& p_data, float p_step = 1.f, T p_min = std::numeric_limits<T>::min(), T p_max = std::numeric_limits<T>::max());
		static void DrawBoolean(VisionUI::Internal::WidgetContainer& p_root, const std::string& p_name, bool& p_data);
		static void DrawVec2(VisionUI::Internal::WidgetContainer& p_root, const std::string& p_name, VisionMaths::FVector2& p_data, float p_step = 1.f, float p_min = _MIN_FLOAT, float p_max = _MAX_FLOAT);
		static void DrawVec3(VisionUI::Internal::WidgetContainer& p_root, const std::string& p_name, VisionMaths::FVector3& p_data, float p_step = 1.f, float p_min = _MIN_FLOAT, float p_max = _MAX_FLOAT);
		static void DrawVec4(VisionUI::Internal::WidgetContainer& p_root, const std::string& p_name, VisionMaths::FVector4& p_data, float p_step = 1.f, float p_min = _MIN_FLOAT, float p_max = _MAX_FLOAT);
		static void DrawQuat(VisionUI::Internal::WidgetContainer& p_root, const std::string& p_name, VisionMaths::FQuaternion& p_data, float p_step = 1.f, float p_min = _MIN_FLOAT, float p_max = _MAX_FLOAT);
		static void DrawString(VisionUI::Internal::WidgetContainer& p_root, const std::string& p_name, std::string& p_data);
		static void DrawColor(VisionUI::Internal::WidgetContainer& p_root, const std::string& p_name, VisionUI::Types::Color& p_color, bool p_hasAlpha = false);
		static VisionUI::Widgets::Texts::Text& DrawMesh(VisionUI::Internal::WidgetContainer& p_root, const std::string& p_name, VisionRendering::Resources::Model*& p_data, VisionTools::Eventing::Event<>* p_updateNotifier = nullptr);
		static VisionUI::Widgets::Visual::Image& DrawTexture(VisionUI::Internal::WidgetContainer& p_root, const std::string& p_name, VisionRendering::Resources::Texture*& p_data, VisionTools::Eventing::Event<>* p_updateNotifier = nullptr);
		static VisionUI::Widgets::Texts::Text& DrawShader(VisionUI::Internal::WidgetContainer& p_root, const std::string& p_name, VisionRendering::Resources::Shader*& p_data, VisionTools::Eventing::Event<>* p_updateNotifier = nullptr);
		static VisionUI::Widgets::Texts::Text& DrawMaterial(VisionUI::Internal::WidgetContainer& p_root, const std::string& p_name, VisionCore::Resources::Material*& p_data, VisionTools::Eventing::Event<>* p_updateNotifier = nullptr);
		static VisionUI::Widgets::Texts::Text& DrawSound(VisionUI::Internal::WidgetContainer& p_root, const std::string& p_name, VisionAudio::Resources::Sound*& p_data, VisionTools::Eventing::Event<>* p_updateNotifier = nullptr);
		static VisionUI::Widgets::Texts::Text& DrawAsset(VisionUI::Internal::WidgetContainer& p_root, const std::string& p_name, std::string& p_data, VisionTools::Eventing::Event<>* p_updateNotifier = nullptr);

		template <typename T>
		static void DrawScalar(VisionUI::Internal::WidgetContainer& p_root, const std::string& p_name, std::function<T(void)> p_gatherer, std::function<void(T)> p_provider, float p_step = 1.f, T p_min = std::numeric_limits<T>::min(), T p_max = std::numeric_limits<T>::max());
		static void DrawBoolean(VisionUI::Internal::WidgetContainer& p_root, const std::string& p_name, std::function<bool(void)> p_gatherer, std::function<void(bool)> p_provider);
		static void DrawVec2(VisionUI::Internal::WidgetContainer& p_root, const std::string& p_name, std::function<VisionMaths::FVector2(void)> p_gatherer, std::function<void(VisionMaths::FVector2)> p_provider, float p_step = 1.f, float p_min = _MIN_FLOAT, float p_max = _MAX_FLOAT);
		static void DrawVec3(VisionUI::Internal::WidgetContainer& p_root, const std::string& p_name, std::function<VisionMaths::FVector3(void)> p_gatherer, std::function<void(VisionMaths::FVector3)> p_provider, float p_step = 1.f, float p_min = _MIN_FLOAT, float p_max = _MAX_FLOAT);
		static void DrawVec4(VisionUI::Internal::WidgetContainer& p_root, const std::string& p_name, std::function<VisionMaths::FVector4(void)> p_gatherer, std::function<void(VisionMaths::FVector4)> p_provider, float p_step = 1.f, float p_min = _MIN_FLOAT, float p_max = _MAX_FLOAT);
		static void DrawQuat(VisionUI::Internal::WidgetContainer& p_root, const std::string& p_name, std::function<VisionMaths::FQuaternion(void)> p_gatherer, std::function<void(VisionMaths::FQuaternion)> p_provider, float p_step = 1.f, float p_min = _MIN_FLOAT, float p_max = _MAX_FLOAT);
		static void DrawDDString(VisionUI::Internal::WidgetContainer& p_root, const std::string& p_name, std::function<std::string(void)> p_gatherer, std::function<void(std::string)> p_provider, const std::string& p_identifier);
		static void DrawString(VisionUI::Internal::WidgetContainer& p_root, const std::string& p_name, std::function<std::string(void)> p_gatherer, std::function<void(std::string)> p_provider);
		static void DrawColor(VisionUI::Internal::WidgetContainer& p_root, const std::string& p_name, std::function<VisionUI::Types::Color(void)> p_gatherer, std::function<void(VisionUI::Types::Color)> p_provider, bool p_hasAlpha = false);

		template <typename T>
		static ImGuiDataType_ GetDataType();

		template <typename T>
		static std::string GetFormat();

	private:
		static VisionRendering::Resources::Texture* __EMPTY_TEXTURE;
	};
}

#include "VisionCore/Helpers/GUIDrawer.inl"