#include "VisionEditor/Panels/Hierarchy.h"
#include "VisionEditor/Core/EditorActions.h"

#include <VisionUI/Widgets/Buttons/Button.h>
#include <VisionUI/Widgets/Selection/CheckBox.h>
#include <VisionUI/Widgets/Visual/Separator.h>
#include <VisionUI/Widgets/Layout/Columns.h>
#include <VisionUI/Plugins/DDSource.h>
#include <VisionUI/Plugins/DDTarget.h>
#include <VisionUI/Plugins/ContextualMenu.h>

#include <VisionDebug/Logger.h>

#include <VisionCore/Global/ServiceLocator.h>

#include <VisionCore/ECS/Components/CCamera.h>
#include <VisionCore/ECS/Components/CPointLight.h>
#include <VisionCore/ECS/Components/CDirectionalLight.h>
#include <VisionCore/ECS/Components/CSpotLight.h>
#include <VisionCore/ECS/Components/CAmbientBoxLight.h>
#include <VisionCore/ECS/Components/CAmbientSphereLight.h>
#include <VisionCore/ECS/Components/CPhysicalBox.h>
#include <VisionCore/ECS/Components/CPhysicalSphere.h>
#include <VisionCore/ECS/Components/CPhysicalCapsule.h>
#include <VisionCore/ECS/Components/CAudioSource.h>
#include <VisionCore/ECS/Components/CAudioListener.h>

#include "VisionEditor/Utils/ActorCreationMenu.h"

class HierarchyContextualMenu : public VisionUI::Plugins::ContextualMenu
{
public:
    HierarchyContextualMenu(VisionCore::ECS::Actor* p_target, VisionUI::Widgets::Layout::TreeNode& p_treeNode, bool p_panelMenu = false) :
        m_target(p_target),
        m_treeNode(p_treeNode)
    {
        using namespace VisionUI::Panels;
        using namespace VisionUI::Widgets;
        using namespace VisionUI::Widgets::Menu;
        using namespace VisionCore::ECS::Components;

        if (m_target)
        {
            auto& focusButton = CreateWidget<VisionUI::Widgets::Menu::MenuItem>("Focus");
            focusButton.ClickedEvent += [this]
            {
                EDITOR_EXEC(MoveToTarget(*m_target));
            };

            auto& duplicateButton = CreateWidget<VisionUI::Widgets::Menu::MenuItem>("Duplicate");
            duplicateButton.ClickedEvent += [this]
            {
                EDITOR_EXEC(DelayAction(EDITOR_BIND(DuplicateActor, std::ref(*m_target), nullptr, true), 0));
            };

            auto& deleteButton = CreateWidget<VisionUI::Widgets::Menu::MenuItem>("Delete");
            deleteButton.ClickedEvent += [this]
            {
                EDITOR_EXEC(DestroyActor(std::ref(*m_target)));
            };
        }

		auto& createActor = CreateWidget<VisionUI::Widgets::Menu::MenuList>("Create...");
        VisionEditor::Utils::ActorCreationMenu::GenerateActorCreationMenu(createActor, m_target, std::bind(&VisionUI::Widgets::Layout::TreeNode::Open, &m_treeNode));
	}

	virtual void Execute() override
	{
		if (m_widgets.size() > 0)
			VisionUI::Plugins::ContextualMenu::Execute();
	}

private:
	VisionCore::ECS::Actor* m_target;
	VisionUI::Widgets::Layout::TreeNode& m_treeNode;
};

void ExpandTreeNode(VisionUI::Widgets::Layout::TreeNode& p_toExpand, const VisionUI::Widgets::Layout::TreeNode* p_root)
{
	p_toExpand.Open();

	if (&p_toExpand != p_root && p_toExpand.HasParent())
	{
		ExpandTreeNode(*static_cast<VisionUI::Widgets::Layout::TreeNode*>(p_toExpand.GetParent()), p_root);
	}
}

std::vector<VisionUI::Widgets::Layout::TreeNode*> nodesToCollapse;
std::vector<VisionUI::Widgets::Layout::TreeNode*> founds;

void ExpandTreeNodeAndEnable(VisionUI::Widgets::Layout::TreeNode& p_toExpand, const VisionUI::Widgets::Layout::TreeNode* p_root)
{
	if (!p_toExpand.IsOpened())
	{
		p_toExpand.Open();
		nodesToCollapse.push_back(&p_toExpand);
	}

	p_toExpand.enabled = true;

	if (&p_toExpand != p_root && p_toExpand.HasParent())
	{
		ExpandTreeNodeAndEnable(*static_cast<VisionUI::Widgets::Layout::TreeNode*>(p_toExpand.GetParent()), p_root);
	}
}

VisionEditor::Panels::Hierarchy::Hierarchy
(
	const std::string & p_title,
	bool p_opened,
	const VisionUI::Settings::PanelWindowSettings& p_windowSettings
) : PanelWindow(p_title, p_opened, p_windowSettings)
{	
	m_HierarchyHeader = &CreateWidget<VisionUI::Widgets::Layout::Group>();
	m_HierarchyHeader->enabled = true;
	auto& headerColumns = m_HierarchyHeader->CreateWidget<VisionUI::Widgets::Layout::Columns<2>>();

	auto& searchTitle = CreateWidget<VisionUI::Widgets::Texts::Text>();
	searchTitle.content = "Search";
	/* Name field */
	auto& searchBar = CreateWidget<VisionUI::Widgets::InputFields::InputText>();
	searchBar.ContentChangedEvent += [this](const std::string& p_content)
	{
		founds.clear();
		auto content = p_content;
		std::transform(content.begin(), content.end(), content.begin(), ::tolower);

		for (auto& [actor, item] : m_widgetActorLink)
		{
			if (!p_content.empty())
			{
				auto itemName = item->name;
				std::transform(itemName.begin(), itemName.end(), itemName.begin(), ::tolower);

				if (itemName.find(content) != std::string::npos)
				{
					founds.push_back(item);
				}

				item->enabled = false;
			}
			else
			{
				item->enabled = true;
			}
		}

		for (auto node : founds)
		{
			node->enabled = true;

			if (node->HasParent())
			{
				ExpandTreeNodeAndEnable(*static_cast<VisionUI::Widgets::Layout::TreeNode*>(node->GetParent()), m_sceneRoot);
			}
		}

		if (p_content.empty())
		{
			for (auto node : nodesToCollapse)
			{
				node->Close();
			}

			nodesToCollapse.clear();
		}
	};

	m_sceneRoot = &CreateWidget<VisionUI::Widgets::Layout::TreeNode>("Root", true);
	static_cast<VisionUI::Widgets::Layout::TreeNode*>(m_sceneRoot)->Open();
	m_sceneRoot->AddPlugin<VisionUI::Plugins::DDTarget<std::pair<VisionCore::ECS::Actor*, VisionUI::Widgets::Layout::TreeNode*>>>("Actor").DataReceivedEvent += [this](std::pair<VisionCore::ECS::Actor*, VisionUI::Widgets::Layout::TreeNode*> p_element)
	{
		if (p_element.second->HasParent())
			p_element.second->GetParent()->UnconsiderWidget(*p_element.second);

		m_sceneRoot->ConsiderWidget(*p_element.second);

		p_element.first->DetachFromParent();
	};
    m_sceneRoot->AddPlugin<HierarchyContextualMenu>(nullptr, *m_sceneRoot);

	EDITOR_EVENT(ActorUnselectedEvent) += std::bind(&Hierarchy::UnselectActorsWidgets, this);
	EDITOR_CONTEXT(sceneManager).SceneUnloadEvent += std::bind(&Hierarchy::Clear, this);
	VisionCore::ECS::Actor::CreatedEvent += std::bind(&Hierarchy::AddActorByInstance, this, std::placeholders::_1);
	VisionCore::ECS::Actor::DestroyedEvent += std::bind(&Hierarchy::DeleteActorByInstance, this, std::placeholders::_1);
	EDITOR_EVENT(ActorSelectedEvent) += std::bind(&Hierarchy::SelectActorByInstance, this, std::placeholders::_1);
	VisionCore::ECS::Actor::AttachEvent += std::bind(&Hierarchy::AttachActorToParent, this, std::placeholders::_1);
	VisionCore::ECS::Actor::DettachEvent += std::bind(&Hierarchy::DetachFromParent, this, std::placeholders::_1);
}

void VisionEditor::Panels::Hierarchy::Clear()
{
	EDITOR_EXEC(UnselectActor());

	m_sceneRoot->RemoveAllWidgets();
	m_widgetActorLink.clear();
}

void VisionEditor::Panels::Hierarchy::UnselectActorsWidgets()
{
	for (auto& widget : m_widgetActorLink)
		widget.second->selected = false;
}

void VisionEditor::Panels::Hierarchy::SelectActorByInstance(VisionCore::ECS::Actor& p_actor)
{
	if (auto result = m_widgetActorLink.find(&p_actor); result != m_widgetActorLink.end())
		if (result->second)
			SelectActorByWidget(*result->second);
}

void VisionEditor::Panels::Hierarchy::SelectActorByWidget(VisionUI::Widgets::Layout::TreeNode & p_widget)
{
	UnselectActorsWidgets();

	p_widget.selected = true;

	if (p_widget.HasParent())
	{
		ExpandTreeNode(*static_cast<VisionUI::Widgets::Layout::TreeNode*>(p_widget.GetParent()), m_sceneRoot);
	}
}

void VisionEditor::Panels::Hierarchy::AttachActorToParent(VisionCore::ECS::Actor & p_actor)
{
	auto actorWidget = m_widgetActorLink.find(&p_actor);

	if (actorWidget != m_widgetActorLink.end())
	{
		auto widget = actorWidget->second;

		if (widget->HasParent())
			widget->GetParent()->UnconsiderWidget(*widget);

		if (p_actor.HasParent())
		{
			auto parentWidget = m_widgetActorLink.at(p_actor.GetParent());
			parentWidget->leaf = false;
			parentWidget->ConsiderWidget(*widget);
		}
	}
}

void VisionEditor::Panels::Hierarchy::DetachFromParent(VisionCore::ECS::Actor & p_actor)
{
	if (auto actorWidget = m_widgetActorLink.find(&p_actor); actorWidget != m_widgetActorLink.end())
	{
		if (p_actor.HasParent() && p_actor.GetParent()->GetChildren().size() == 1)
		{
			if (auto parentWidget = m_widgetActorLink.find(p_actor.GetParent()); parentWidget != m_widgetActorLink.end())
			{
				parentWidget->second->leaf = true;
			}
		}

		auto widget = actorWidget->second;

		if (widget->HasParent())
			widget->GetParent()->UnconsiderWidget(*widget);

		m_sceneRoot->ConsiderWidget(*widget);
	}
}

void VisionEditor::Panels::Hierarchy::DeleteActorByInstance(VisionCore::ECS::Actor& p_actor)
{
	if (auto result = m_widgetActorLink.find(&p_actor); result != m_widgetActorLink.end())
	{
		if (result->second)
		{
			result->second->Destroy();
		}

		m_widgetActorLink.erase(result);
	}
}

void VisionEditor::Panels::Hierarchy::AddActorByInstance(VisionCore::ECS::Actor & p_actor)
{
	auto& textSelectable = m_sceneRoot->CreateWidget<VisionUI::Widgets::Layout::TreeNode>(p_actor.GetName(), true);
	textSelectable.leaf = true;
	textSelectable.AddPlugin<HierarchyContextualMenu>(&p_actor, textSelectable);
	textSelectable.AddPlugin<VisionUI::Plugins::DDSource<std::pair<VisionCore::ECS::Actor*, VisionUI::Widgets::Layout::TreeNode*>>>("Actor", "Attach to...", std::make_pair(&p_actor, &textSelectable));
	textSelectable.AddPlugin<VisionUI::Plugins::DDTarget<std::pair<VisionCore::ECS::Actor*, VisionUI::Widgets::Layout::TreeNode*>>>("Actor").DataReceivedEvent += [&p_actor, &textSelectable](std::pair<VisionCore::ECS::Actor*, VisionUI::Widgets::Layout::TreeNode*> p_element)
	{
		if (p_element.second->HasParent())
			p_element.second->GetParent()->UnconsiderWidget(*p_element.second);

		textSelectable.ConsiderWidget(*p_element.second);

		p_element.first->SetParent(p_actor);
	};
	auto& dispatcher = textSelectable.AddPlugin<VisionUI::Plugins::DataDispatcher<std::string>>();

	VisionCore::ECS::Actor* targetPtr = &p_actor;
	dispatcher.RegisterGatherer([targetPtr] { return targetPtr->GetName(); });

	m_widgetActorLink[targetPtr] = &textSelectable;

	textSelectable.ClickedEvent += EDITOR_BIND(SelectActor, std::ref(p_actor));
	textSelectable.DoubleClickedEvent += EDITOR_BIND(MoveToTarget, std::ref(p_actor));
}