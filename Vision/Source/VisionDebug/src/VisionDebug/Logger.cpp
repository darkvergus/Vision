#include "VisionDebug/Logger.h"
#include "VisionTools/Time/Date.h"

VisionTools::Eventing::Event<const VisionDebug::LogData&> VisionDebug::Logger::LogEvent;

std::map<std::string, VisionDebug::ConsoleHandler>	VisionDebug::Logger::CONSOLE_HANDLER_MAP;
std::map<std::string, VisionDebug::FileHandler>		VisionDebug::Logger::FILE_HANDLER_MAP;
std::map<std::string, VisionDebug::HistoryHandler>	VisionDebug::Logger::HISTORY_HANDLER_MAP;

void VisionDebug::Logger::Log(const std::string& p_message, ELogLevel p_logLevel, ELogMode p_logMode, std::string p_handlerId)
{
	LogData logData{ p_message, p_logLevel, VisionTools::Time::Date::GetDateAsString() };

	switch (p_logMode)
	{
	case ELogMode::DEFAULT:
	case ELogMode::CONSOLE: LogToHandlerMap<ConsoleHandler>(CONSOLE_HANDLER_MAP, logData, p_handlerId); break;
	case ELogMode::FILE:	LogToHandlerMap<FileHandler>(FILE_HANDLER_MAP, logData, p_handlerId);		break;
	case ELogMode::HISTORY: LogToHandlerMap<HistoryHandler>(HISTORY_HANDLER_MAP, logData, p_handlerId);	break;
	case ELogMode::ALL:
		LogToHandlerMap<ConsoleHandler>(CONSOLE_HANDLER_MAP, logData, p_handlerId);
		LogToHandlerMap<FileHandler>(FILE_HANDLER_MAP, logData, p_handlerId);
		LogToHandlerMap<HistoryHandler>(HISTORY_HANDLER_MAP, logData, p_handlerId);
		break;
	}

	LogEvent.Invoke(logData);
}

VisionDebug::ConsoleHandler& VisionDebug::Logger::CreateConsoleHandler(std::string p_id)
{
	CONSOLE_HANDLER_MAP.emplace(p_id, VisionDebug::ConsoleHandler());
	return CONSOLE_HANDLER_MAP[p_id];
}

VisionDebug::FileHandler& VisionDebug::Logger::CreateFileHandler(std::string p_id)
{
	FILE_HANDLER_MAP.emplace(p_id, VisionDebug::FileHandler());
	return FILE_HANDLER_MAP[p_id];
}

VisionDebug::HistoryHandler& VisionDebug::Logger::CreateHistoryHandler(std::string p_id)
{
	HISTORY_HANDLER_MAP.emplace(p_id, VisionDebug::HistoryHandler());
	return HISTORY_HANDLER_MAP[p_id];
}

VisionDebug::ConsoleHandler& VisionDebug::Logger::GetConsoleHandler(std::string p_id)
{
	return CONSOLE_HANDLER_MAP[p_id];
}

VisionDebug::FileHandler& VisionDebug::Logger::GetFileHandler(std::string p_id)
{
	return FILE_HANDLER_MAP[p_id];
}

VisionDebug::HistoryHandler& VisionDebug::Logger::GetHistoryHandler(std::string p_id)
{
	return HISTORY_HANDLER_MAP[p_id];
}
