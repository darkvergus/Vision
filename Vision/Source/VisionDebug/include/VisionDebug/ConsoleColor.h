#pragma once

#include <iostream>
#include <windows.h>

#define COLOR_BLUE VisionDebug::blue
#define COLOR_RED VisionDebug::red
#define COLOR_GREEN VisionDebug::green
#define COLOR_YELLOW VisionDebug::yellow
#define COLOR_WHITE VisionDebug::white
#define COLOR_DEFAULT VisionDebug::grey

namespace VisionDebug
{
	inline std::ostream& blue(std::ostream& s)
	{
		HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hStdout,
			FOREGROUND_BLUE | FOREGROUND_GREEN |
			FOREGROUND_INTENSITY);
		return s;
	}

	inline std::ostream& red(std::ostream& s)
	{
		HANDLE hStdout = GetStdHandle(STD_ERROR_HANDLE);
		SetConsoleTextAttribute(hStdout,
			FOREGROUND_RED | FOREGROUND_INTENSITY);
		return s;
	}

	inline std::ostream& green(std::ostream& s)
	{
		HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hStdout,
			FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		return s;
	}

	inline std::ostream& yellow(std::ostream& s)
	{
		HANDLE hStdout = GetStdHandle(STD_ERROR_HANDLE);
		SetConsoleTextAttribute(hStdout,
			FOREGROUND_GREEN | FOREGROUND_RED |
			FOREGROUND_INTENSITY);
		return s;
	}

	inline std::ostream& white(std::ostream& s)
	{
		HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hStdout,
			FOREGROUND_RED | FOREGROUND_GREEN |
			FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		return s;
	}

	inline std::ostream& grey(std::ostream& s)
	{
		HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hStdout,
			FOREGROUND_RED | FOREGROUND_GREEN |
			FOREGROUND_BLUE);
		return s;
	}

	struct color
	{
		color(WORD attribute) : m_color(attribute)
		{
		}

		WORD m_color;
	};

	template <class _Elem, class _Traits>
	std::basic_ostream<_Elem, _Traits>&
		operator<<(std::basic_ostream<_Elem, _Traits>& i, color& c)
	{
		HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hStdout, c.m_color);
		return i;
	}
}