#include "VisionUI/Internal/Converter.h"

ImVec4 VisionUI::Internal::Converter::ToImVec4(const Types::Color & p_value)
{
	return ImVec4(p_value.r, p_value.g, p_value.b, p_value.a);
}

VisionUI::Types::Color VisionUI::Internal::Converter::ToColor(const ImVec4 & p_value)
{
	return Types::Color(p_value.x, p_value.y, p_value.z, p_value.w);
}

ImVec2 VisionUI::Internal::Converter::ToImVec2(const VisionMaths::FVector2 & p_value)
{
	return ImVec2(p_value.x, p_value.y);
}

VisionMaths::FVector2 VisionUI::Internal::Converter::ToFVector2(const ImVec2 & p_value)
{
	return VisionMaths::FVector2(p_value.x, p_value.y);
}
