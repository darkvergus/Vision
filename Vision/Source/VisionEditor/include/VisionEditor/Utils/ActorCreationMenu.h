#pragma once

#include <functional>

namespace VisionUI::Widgets::Menu
{
    class MenuList;
}

namespace VisionCore::ECS
{
    class Actor;
}

namespace VisionEditor::Utils
{
    /**
    * Class exposing tools to generate an actor creation menu
    */
    class ActorCreationMenu
    {
    public:
        /**
        * Disabled constructor
        */
        ActorCreationMenu() = delete;

        /**
        * Generates an actor creation menu under the given MenuList item.
        * Also handles custom additionnal OnClick callback
        * @param p_menuList
        * @param p_parent
        * @param p_onItemClicked
        */
        static void GenerateActorCreationMenu(VisionUI::Widgets::Menu::MenuList& p_menuList, VisionCore::ECS::Actor* p_parent = nullptr, std::optional<std::function<void()>> p_onItemClicked = {});
    };
}
