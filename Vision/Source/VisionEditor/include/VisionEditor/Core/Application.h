#pragma once

#include "VisionEditor/Core/Context.h"
#include "VisionEditor/Core/Editor.h"

namespace VisionEditor::Core
{
	/**
	* Entry point of OvEditor
	*/
	class Application
	{
	public:
		/**
		* Constructor
		* @param p_projectPath
		* @param p_projectName
		*/
		Application(const std::string& p_projectPath, const std::string& p_projectName);

		/**
		* Destructor
		*/
		~Application();

		/**
		* Run the app
		*/
		void Run();

		/**
		* Returns true if the app is running
		*/
		bool IsRunning() const;

	private:
		Context m_context;
		Editor m_editor;
	};
}