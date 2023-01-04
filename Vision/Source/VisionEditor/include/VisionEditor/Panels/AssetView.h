#pragma once

#include <variant>

#include <VisionCore/Resources/Material.h>

#include "VisionEditor/Panels/AViewControllable.h"

namespace VisionEditor::Panels
{
	/**
	* Provide a view for assets
	*/
	class AssetView : public VisionEditor::Panels::AViewControllable
	{
	public:
		using ViewableResource = std::variant<VisionRendering::Resources::Model*, VisionRendering::Resources::Texture*, VisionCore::Resources::Material*>;

		/**
		* Constructor
		* @param p_title
		* @param p_opened
		* @param p_windowSettings
		*/
		AssetView
		(
			const std::string& p_title,
			bool p_opened,
			const VisionUI::Settings::PanelWindowSettings& p_windowSettings
		);

		/**
		* Custom implementation of the render method
		*/
		virtual void _Render_Impl() override;

		/**
		* Defines the resource to preview
		* @parma p_resource
		*/
		void SetResource(ViewableResource p_resource);

		/**
		* Return the currently previewed resource
		*/
		ViewableResource GetResource() const;

	private:
		ViewableResource m_resource;
	};
}