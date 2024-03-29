#include "VisionUI/Types/Color.h"

const VisionUI::Types::Color VisionUI::Types::Color::Red		= { 1.f, 0.f, 0.f };
const VisionUI::Types::Color VisionUI::Types::Color::Green		= { 0.f, 1.f, 0.f };
const VisionUI::Types::Color VisionUI::Types::Color::Blue		= { 0.f, 0.f, 1.f };
const VisionUI::Types::Color VisionUI::Types::Color::White		= { 1.f, 1.f, 1.f };
const VisionUI::Types::Color VisionUI::Types::Color::Black		= { 0.f, 0.f, 0.f };
const VisionUI::Types::Color VisionUI::Types::Color::Grey		= { 0.5f, 0.5f, 0.5f };
const VisionUI::Types::Color VisionUI::Types::Color::Yellow		= { 1.f, 1.f, 0.f };
const VisionUI::Types::Color VisionUI::Types::Color::Cyan		= { 0.f, 1.f, 1.f };
const VisionUI::Types::Color VisionUI::Types::Color::Magenta	= { 1.f, 0.f, 1.f };

VisionUI::Types::Color::Color(float p_r, float p_g, float p_b, float p_a) : r(p_r), g(p_g), b(p_b), a(p_a)
{
}

bool VisionUI::Types::Color::operator==(const Color & p_other)
{
	return this->r == p_other.r && this->g == p_other.g && this->b == p_other.b && this->a == p_other.a;
}

bool VisionUI::Types::Color::operator!=(const Color & p_other)
{
	return !operator==(p_other);
}
