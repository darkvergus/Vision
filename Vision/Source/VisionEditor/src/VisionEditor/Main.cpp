#include <filesystem>

#include <VisionTools/Utils/String.h>

#include <VisionRendering/Utils/Defines.h>

#include "VisionEditor/Core/ProjectHub.h"
#include "VisionEditor/Core/Application.h"

#undef APIENTRY
#include "Windows.h"

FORCE_DEDICATED_GPU

/**
* When Vision is launched from a project file, we should consider the executable path as
* the current working directory
* @param p_executablePath
*/
void UpdateWorkingDirectory(const std::string& p_executablePath)
{
	if (!IsDebuggerPresent())
	{
		std::filesystem::current_path(VisionTools::Utils::PathParser::GetContainingFolder(p_executablePath));
	}
}

int main(int argc, char** argv);
static void TryRun(const std::string& projectPath, const std::string& projectName);

#ifndef _DEBUG
INT WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow)
{
	main(__argc, __argv);
}
#endif

int main(int argc, char** argv)
{
	UpdateWorkingDirectory(argv[0]);

	bool ready = false;
	std::string projectPath;
	std::string projectName;

	{
		VisionEditor::Core::ProjectHub hub;

		if (argc < 2)
		{
			// No project file given as argument ==> Open the ProjectHub
			std::tie(ready, projectPath, projectName) = hub.Run();
		}
		else
		{
			// Project file given as argument ==> Open the project
			std::string projectFile = argv[1];

			if (VisionTools::Utils::PathParser::GetExtension(projectFile) == "project")
			{
				ready = true;
				projectPath = VisionTools::Utils::PathParser::GetContainingFolder(projectFile);
				projectName = VisionTools::Utils::PathParser::GetElementName(projectFile);
				VisionTools::Utils::String::Replace(projectName, ".project", "");
			}

			hub.RegisterProject(projectPath);
		}
	}

	if (ready)
		TryRun(projectPath, projectName);

	return EXIT_SUCCESS;
}

static void TryRun(const std::string& projectPath, const std::string& projectName)
{
	auto errorEvent =
		[](VisionWindowing::Context::EDeviceError, std::string errMsg)
		{
			errMsg = "Vision requires OpenGL 4.6 or newer.\r\n" + errMsg;
			MessageBox(0, errMsg.c_str(), "Vision", MB_OK | MB_ICONSTOP);
		};

	std::unique_ptr<VisionEditor::Core::Application> app;

	try
	{
		auto listenerId = VisionWindowing::Context::Device::ErrorEvent += errorEvent;
		app = std::make_unique<VisionEditor::Core::Application>(projectPath, projectName);
		VisionWindowing::Context::Device::ErrorEvent -= listenerId;
	}
	catch (...) {}

	if (app)
		app->Run();
}
