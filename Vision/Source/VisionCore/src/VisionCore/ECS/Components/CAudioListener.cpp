#include "VisionCore/ECS/Components/CAudioListener.h"
#include "VisionCore/ECS/Actor.h"

VisionCore::ECS::Components::CAudioListener::CAudioListener(ECS::Actor& p_owner) :
	AComponent(p_owner),
	m_audioListener(owner.transform.GetFTransform())
{
	m_audioListener.SetEnabled(false);
}

std::string VisionCore::ECS::Components::CAudioListener::GetName()
{
	return "Audio Listener";
}

void VisionCore::ECS::Components::CAudioListener::OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node)
{
}

void VisionCore::ECS::Components::CAudioListener::OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node)
{
}

void VisionCore::ECS::Components::CAudioListener::OnInspector(VisionUI::Internal::WidgetContainer& p_root)
{

}

void VisionCore::ECS::Components::CAudioListener::OnEnable()
{
	m_audioListener.SetEnabled(true);
}

void VisionCore::ECS::Components::CAudioListener::OnDisable()
{
	m_audioListener.SetEnabled(false);
}
