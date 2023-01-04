#pragma once

#include <vector>
#include <memory>

#include <VisionMaths/FVector2.h>
#include <VisionTools/Eventing/Event.h>

#include "VisionUI/Panels/APanel.h"
#include "VisionUI/Settings/Alignment.h"

namespace VisionUI::Panels
{
	/**
	* APanelTransformable is a panel that is localized in the canvas
	*/
	class APanelTransformable : public APanel
	{
	public:
		/**
		* Create a APanelTransformable
		* @param p_defaultPosition
		* @param p_defaultSize
		* @param p_defaultHorizontalAlignment
		* @param p_defaultVerticalAlignment
		* @param p_ignoreConfigFile
		*/
		APanelTransformable
		(
			const VisionMaths::FVector2& p_defaultPosition = VisionMaths::FVector2(-1.f, -1.f),
			const VisionMaths::FVector2& p_defaultSize = VisionMaths::FVector2(-1.f, -1.f),
			Settings::EHorizontalAlignment p_defaultHorizontalAlignment = Settings::EHorizontalAlignment::LEFT,
			Settings::EVerticalAlignment p_defaultVerticalAlignment = Settings::EVerticalAlignment::TOP,
			bool p_ignoreConfigFile = false
		);

		/**
		* Defines the position of the panel
		* @param p_position
		*/
		void SetPosition(const VisionMaths::FVector2& p_position);

		/**
		* Defines the size of the panel
		* @param p_size
		*/
		void SetSize(const VisionMaths::FVector2& p_size);

		/**
		* Defines the alignment of the panel
		* @param p_horizontalAlignment
		* @param p_verticalAligment
		*/
		void SetAlignment(Settings::EHorizontalAlignment p_horizontalAlignment, Settings::EVerticalAlignment p_verticalAligment);

		/**
		* Returns the current position of the panel
		*/
		const VisionMaths::FVector2& GetPosition() const;

		/**
		* Returns the current size of the panel
		*/
		const VisionMaths::FVector2& GetSize() const;

		/**
		* Returns the current horizontal alignment of the panel
		*/
		Settings::EHorizontalAlignment GetHorizontalAlignment() const;

		/**
		* Returns the current vertical alignment of the panel
		*/
		Settings::EVerticalAlignment GetVerticalAlignment() const;

	protected:
		void Update();
		virtual void _Draw_Impl() = 0;

	private:
		VisionMaths::FVector2 CalculatePositionAlignmentOffset(bool p_default = false);

		void UpdatePosition();
		void UpdateSize();
		void CopyImGuiPosition();
		void CopyImGuiSize();

	public:
		bool autoSize = true;

	protected:
		VisionMaths::FVector2 m_defaultPosition;
		VisionMaths::FVector2 m_defaultSize;
		Settings::EHorizontalAlignment m_defaultHorizontalAlignment;
		Settings::EVerticalAlignment m_defaultVerticalAlignment;
		bool m_ignoreConfigFile;

		VisionMaths::FVector2 m_position = VisionMaths::FVector2(0.0f, 0.0f);
		VisionMaths::FVector2 m_size = VisionMaths::FVector2(0.0f, 0.0f);

		bool m_positionChanged = false;
		bool m_sizeChanged = false;

		Settings::EHorizontalAlignment m_horizontalAlignment = Settings::EHorizontalAlignment::LEFT;
		Settings::EVerticalAlignment m_verticalAlignment = Settings::EVerticalAlignment::TOP;

		bool m_alignmentChanged = false;
		bool m_firstFrame = true;
	};
}