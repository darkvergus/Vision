#pragma once


#include <VisionUI/Panels/PanelWindow.h>
#include <VisionUI/Widgets/Visual/Image.h>
#include <VisionRendering/Buffers/UniformBuffer.h>
#include <VisionRendering/Buffers/Framebuffer.h>
#include <VisionRendering/LowRenderer/Camera.h>

namespace VisionEditor::Core { class EditorRenderer; }

namespace VisionEditor::Panels
{
	/**
	* Base class for any view
	*/
	class AView : public VisionUI::Panels::PanelWindow
	{
	public:
		/**
		* Constructor
		* @param p_title
		* @param p_opened
		* @param p_windowSettings
		*/
		AView
		(
			const std::string& p_title,
			bool p_opened,
			const VisionUI::Settings::PanelWindowSettings& p_windowSettings
		);

		/**
		* Update the view
		* @param p_deltaTime
		*/
		virtual void Update(float p_deltaTime);

		/**
		* Custom implementation of the draw method
		*/
		void _Draw_Impl() override;

		/**
		* Custom implementation of the render method to define in dervied classes
		*/
		virtual void _Render_Impl() = 0;

		/**
		* Render the view
		*/
		void Render();

		/**
		* Defines the camera position
		* @param p_position
		*/
		void SetCameraPosition(const VisionMaths::FVector3& p_position);

		/**
		* Defines the camera rotation
		* @param p_rotation
		*/
		void SetCameraRotation(const VisionMaths::FQuaternion& p_rotation);

		/**
		* Returns the camera position
		*/
		const VisionMaths::FVector3& GetCameraPosition() const;

		/**
		* Returns the camera rotation
		*/
		const VisionMaths::FQuaternion& GetCameraRotation() const;

		/**
		* Returns the camera used by this view
		*/
		VisionRendering::LowRenderer::Camera& GetCamera();

		/**
		* Returns the size of the panel ignoring its titlebar height
		*/
		std::pair<uint16_t, uint16_t> GetSafeSize() const;

		/**
		* Returns the grid color of the view
		*/
		const VisionMaths::FVector3& GetGridColor() const;

		/**
		* Defines the grid color of the view
		* @param p_color
		*/
		void SetGridColor(const VisionMaths::FVector3& p_color);

		/**
		* Fill the UBO using the view settings
		*/
		void FillEngineUBO();

	protected:
		/**
		* Update camera matrices
		*/
		void PrepareCamera();

	protected:
		VisionEditor::Core::EditorRenderer& m_editorRenderer;
		VisionRendering::LowRenderer::Camera m_camera;
		VisionMaths::FVector3 m_cameraPosition;
		VisionMaths::FQuaternion m_cameraRotation;
		VisionUI::Widgets::Visual::Image* m_image;

        VisionMaths::FVector3 m_gridColor = VisionMaths::FVector3 { 0.176f, 0.176f, 0.176f };

		VisionRendering::Buffers::Framebuffer m_fbo;
	};
}