#pragma once

#include <map>

#include <VisionTools/Eventing/Event.h>

#include "VisionUI/Widgets/DataWidget.h"

namespace VisionUI::Widgets::Selection
{
	/**
	* Widget that can display a list of values that the user can select
	*/
	class ComboBox : public DataWidget<int>
	{
	public:
		/**
		* Constructor
		* @param p_currentChoice
		*/
		ComboBox(int p_currentChoice = 0);

	protected:
		void _Draw_Impl() override;

	public:
		std::map<int, std::string> choices;
		int currentChoice;

	public:
		VisionTools::Eventing::Event<int> ValueChangedEvent;
	};
}