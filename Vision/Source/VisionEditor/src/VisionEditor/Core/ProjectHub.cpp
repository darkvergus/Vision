#define _CRT_SECURE_NO_WARNINGS

#include <filesystem>
#include <fstream>

#include "VisionEditor/Core/ProjectHub.h"

#include <VisionUI/Widgets/Texts/Text.h>
#include <VisionUI/Widgets/Visual/Separator.h>
#include <VisionUI/Widgets/Layout/Columns.h>
#include <VisionUI/Widgets/Layout/Spacing.h>
#include <VisionUI/Widgets/Layout/Group.h>
#include <VisionUI/Widgets/Buttons/Button.h>
#include <VisionUI/Widgets/InputFields/InputText.h>

#include <VisionTools/Utils/PathParser.h>

#include <VisionWindowing/Dialogs/SaveFileDialog.h>
#include <VisionWindowing/Dialogs/OpenFileDialog.h>
#include <VisionWindowing/Dialogs/MessageBox.h>

#define PROJECTS_FILE std::string(std::string(getenv("APPDATA")) + "\\Vision\\VisionEditor\\projects.ini")

class ProjectHubPanel : public VisionUI::Panels::PanelWindow
{
public:
	ProjectHubPanel(bool& p_readyToGo, std::string& p_path, std::string& p_projectName) :
		PanelWindow("Vision - Project Hub", true),
		m_readyToGo(p_readyToGo),
		m_path(p_path),
		m_projectName(p_projectName)
	{
		resizable = false;
		movable = false;
		titleBar = false;

		std::filesystem::create_directories(std::string(getenv("APPDATA")) + "\\Vision\\VisionEditor\\");

		SetSize({ 1000, 580 });
		SetPosition({ 0.f, 0.f });

		auto& openProjectButton = CreateWidget<VisionUI::Widgets::Buttons::Button>("Open Project");
		auto& newProjectButton = CreateWidget<VisionUI::Widgets::Buttons::Button>("New Project");
		auto& pathField = CreateWidget<VisionUI::Widgets::InputFields::InputText>("");
		m_goButton = &CreateWidget<VisionUI::Widgets::Buttons::Button>("GO");

		pathField.ContentChangedEvent += [this, &pathField](std::string p_content)
		{
			pathField.content = VisionTools::Utils::PathParser::MakeWindowsStyle(p_content);

			if (pathField.content != "" && pathField.content.back() != '\\')
				pathField.content += '\\';

			UpdateGoButton(pathField.content);
		};

		UpdateGoButton("");

		openProjectButton.idleBackgroundColor = { 0.7f, 0.5f, 0.f };
		newProjectButton.idleBackgroundColor = { 0.f, 0.5f, 0.0f };

		openProjectButton.ClickedEvent += [this]
		{
			VisionWindowing::Dialogs::OpenFileDialog dialog("Open project");
			dialog.AddFileType("Vision Project", "*.project");
			dialog.Show();

			std::string ovProjectPath = dialog.GetSelectedFilePath();
			std::string rootFolderPath = VisionTools::Utils::PathParser::GetContainingFolder(ovProjectPath);

			if (dialog.HasSucceeded())
			{
				RegisterProject(rootFolderPath);
				OpenProject(rootFolderPath);
			}
		};

		newProjectButton.ClickedEvent += [this, &pathField]
		{
			VisionWindowing::Dialogs::SaveFileDialog dialog("New project location");
			dialog.DefineExtension("Vision Project", "..");
			dialog.Show();
			if (dialog.HasSucceeded())
			{
				std::string result = dialog.GetSelectedFilePath();
				pathField.content = std::string(result.data(), result.data() + result.size() - std::string("..").size()); // remove auto extension
				pathField.content += "\\";
				UpdateGoButton(pathField.content);
			}
		};

		m_goButton->ClickedEvent += [this, &pathField]
		{
			CreateProject(pathField.content);
			RegisterProject(pathField.content);
			OpenProject(pathField.content);
		};

		openProjectButton.lineBreak = false;
		newProjectButton.lineBreak = false;
		pathField.lineBreak = false;

		for (uint8_t i = 0; i < 4; ++i)
			CreateWidget<VisionUI::Widgets::Layout::Spacing>();

		CreateWidget<VisionUI::Widgets::Visual::Separator>();

		for (uint8_t i = 0; i < 4; ++i)
			CreateWidget<VisionUI::Widgets::Layout::Spacing>();

		auto& columns = CreateWidget<VisionUI::Widgets::Layout::Columns<2>>();

		columns.widths = { 750, 500 };

		std::string line;
		std::ifstream myfile(PROJECTS_FILE);
		if (myfile.is_open())
		{
			while (getline(myfile, line))
			{
				if (std::filesystem::exists(line)) // TODO: Delete line from the file
				{
					auto& text = columns.CreateWidget<VisionUI::Widgets::Texts::Text>(line);
					auto& actions = columns.CreateWidget<VisionUI::Widgets::Layout::Group>();
					auto& openButton = actions.CreateWidget<VisionUI::Widgets::Buttons::Button>("Open");
					auto& deleteButton = actions.CreateWidget<VisionUI::Widgets::Buttons::Button>("Delete");

					openButton.idleBackgroundColor = { 0.7f, 0.5f, 0.f };
					deleteButton.idleBackgroundColor = { 0.5f, 0.f, 0.f };

					openButton.ClickedEvent += [this, line]
					{
						OpenProject(line);
					};

					std::string toErase = line;
					deleteButton.ClickedEvent += [this, &text, &actions, toErase]
					{
						text.Destroy();
						actions.Destroy();

						std::string line;
						std::ifstream fin(PROJECTS_FILE);
						std::ofstream temp("temp");

						while (getline(fin, line))
							if (line != toErase)
								temp << line << std::endl;

						temp.close();
						fin.close();

						std::filesystem::remove(PROJECTS_FILE);
						std::filesystem::rename("temp", PROJECTS_FILE);
					};

					openButton.lineBreak = false;
					deleteButton.lineBreak;
				}
			}
			myfile.close();
		}
	}

	void UpdateGoButton(const std::string& p_path)
	{
		bool validPath = p_path != "";
		m_goButton->idleBackgroundColor = validPath ? VisionUI::Types::Color{ 0.f, 0.5f, 0.0f } : VisionUI::Types::Color{ 0.1f, 0.1f, 0.1f };
		m_goButton->disabled = !validPath;
	}

	void CreateProject(const std::string& p_path)
	{
		if (!std::filesystem::exists(p_path))
		{
			std::filesystem::create_directory(p_path);
			std::filesystem::create_directory(p_path + "Assets\\");
			std::filesystem::create_directory(p_path + "Scripts\\");
			std::ofstream projectFile(p_path + '\\' + VisionTools::Utils::PathParser::GetElementName(std::string(p_path.data(), p_path.data() + p_path.size() - 1)) + ".project");
		}
	}

	void RegisterProject(const std::string& p_path)
	{
		bool pathAlreadyRegistered = false;

		{
			std::string line;
			std::ifstream myfile(PROJECTS_FILE);
			if (myfile.is_open())
			{
				while (getline(myfile, line))
				{
					if (line == p_path)
					{
						pathAlreadyRegistered = true;
						break;
					}
				}
				myfile.close();
			}
		}

		if (!pathAlreadyRegistered)
		{
			std::ofstream projectsFile(PROJECTS_FILE, std::ios::app);
			projectsFile << p_path << std::endl;
		}
	}
	
	void OpenProject(const std::string& p_path)
	{
		m_readyToGo = std::filesystem::exists(p_path);
		if (!m_readyToGo)
		{
			using namespace VisionWindowing::Dialogs;
			MessageBox errorMessage("Project not found", "The selected project does not exists", MessageBox::EMessageType::ERROR, MessageBox::EButtonLayout::OK);
		}
		else
		{
			m_path = p_path;
			m_projectName = VisionTools::Utils::PathParser::GetElementName(m_path);
			Close();
		}
	}

	void Draw() override
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 50.f, 50.f });
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.f);

		VisionUI::Panels::PanelWindow::Draw();

		ImGui::PopStyleVar(2);
	}

private:
	bool& m_readyToGo;
	std::string& m_path;
	std::string& m_projectName;
	VisionUI::Widgets::Buttons::Button* m_goButton = nullptr;
};

VisionEditor::Core::ProjectHub::ProjectHub()
{
	SetupContext();
	m_mainPanel = std::make_unique<ProjectHubPanel>(m_readyToGo, m_projectPath, m_projectName);

	m_uiManager->SetCanvas(m_canvas);
	m_canvas.AddPanel(*m_mainPanel);
}

std::tuple<bool, std::string, std::string> VisionEditor::Core::ProjectHub::Run()
{
	m_renderer->SetClearColor(0.f, 0.f, 0.f, 1.f);

	while (!m_window->ShouldClose())
	{
		m_renderer->Clear();
		m_device->PollEvents();
		m_uiManager->Render();
		m_window->SwapBuffers();

		if (!m_mainPanel->IsOpened())
			m_window->SetShouldClose(true);
	}

	return { m_readyToGo, m_projectPath, m_projectName };
}

void VisionEditor::Core::ProjectHub::SetupContext()
{
	/* Settings */
	VisionWindowing::Settings::DeviceSettings deviceSettings;
	VisionWindowing::Settings::WindowSettings windowSettings;
	windowSettings.title = "Vision - Project Hub";
	windowSettings.width = 1000;
	windowSettings.height = 580;
	windowSettings.maximized = false;
	windowSettings.resizable = false;
	windowSettings.decorated = true;

	/* Window creation */
	m_device = std::make_unique<VisionWindowing::Context::Device>(deviceSettings);
	m_window = std::make_unique<VisionWindowing::Window>(*m_device, windowSettings);
	m_window->MakeCurrentContext();

	auto[monWidth, monHeight] = m_device->GetMonitorSize();
	auto[winWidth, winHeight] = m_window->GetSize();
	m_window->SetPosition(monWidth / 2 - winWidth / 2, monHeight / 2 - winHeight / 2);

	/* Graphics context creation */
	m_driver = std::make_unique<VisionRendering::Context::Driver>(VisionRendering::Settings::DriverSettings{ false });
	m_renderer = std::make_unique<VisionRendering::Core::Renderer>(*m_driver);
	m_renderer->SetCapability(VisionRendering::Settings::ERenderingCapability::MULTISAMPLE, true);

	m_uiManager = std::make_unique<VisionUI::Core::UIManager>(m_window->GetGlfwWindow(), VisionUI::Styling::EStyle::ALTERNATIVE_DARK);
	m_uiManager->LoadFont("Ruda_Big", "Data\\Editor\\Fonts\\Ruda-Bold.ttf", 18);
	m_uiManager->UseFont("Ruda_Big");
	m_uiManager->EnableEditorLayoutSave(false);
	m_uiManager->EnableDocking(false);
}

void VisionEditor::Core::ProjectHub::RegisterProject(const std::string& p_path)
{
	static_cast<ProjectHubPanel*>(m_mainPanel.get())->RegisterProject(p_path);
}
