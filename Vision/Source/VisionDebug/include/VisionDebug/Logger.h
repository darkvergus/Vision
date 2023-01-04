#pragma once

#include <string>
#include <map>

#include <VisionTools/Eventing/Event.h>

#include "VisionDebug/ILogHandler.h"
#include "VisionDebug/ConsoleHandler.h"
#include "VisionDebug/FileHandler.h"
#include "VisionDebug/HistoryHandler.h"

#define LOG(message)			VisionDebug::Logger::Log(message, VisionDebug::ELogLevel::LOG_DEFAULT,	VisionDebug::ELogMode::CONSOLE)
#define LOG_INFO(message)		VisionDebug::Logger::Log(message, VisionDebug::ELogLevel::LOG_INFO,		VisionDebug::ELogMode::CONSOLE)
#define LOG_WARNING(message)	VisionDebug::Logger::Log(message, VisionDebug::ELogLevel::LOG_WARNING,	VisionDebug::ELogMode::CONSOLE)
#define LOG_ERROR(message)	VisionDebug::Logger::Log(message, VisionDebug::ELogLevel::LOG_ERROR,	VisionDebug::ELogMode::CONSOLE)

#define FLOG(message)			VisionDebug::Logger::Log(message, VisionDebug::ELogLevel::LOG_DEFAULT, 	VisionDebug::ELogMode::CONSOLE); VisionDebug::Logger::Log(message, VisionDebug::ELogLevel::LOG_DEFAULT, VisionDebug::ELogMode::FILE)
#define FLOG_INFO(message)	VisionDebug::Logger::Log(message, VisionDebug::ELogLevel::LOG_INFO, 	VisionDebug::ELogMode::CONSOLE); VisionDebug::Logger::Log(message, VisionDebug::ELogLevel::LOG_INFO, 	VisionDebug::ELogMode::FILE)
#define FLOG_WARNING(message)	VisionDebug::Logger::Log(message, VisionDebug::ELogLevel::LOG_WARNING, 	VisionDebug::ELogMode::CONSOLE); VisionDebug::Logger::Log(message, VisionDebug::ELogLevel::LOG_WARNING, VisionDebug::ELogMode::FILE)
#define FLOG_ERROR(message)	VisionDebug::Logger::Log(message, VisionDebug::ELogLevel::LOG_ERROR, 	VisionDebug::ELogMode::CONSOLE); VisionDebug::Logger::Log(message, VisionDebug::ELogLevel::LOG_ERROR, 	VisionDebug::ELogMode::FILE)

namespace VisionDebug
{
	/*
	* Static class to display error messages on console or file
	*/
	class Logger
	{
	public:

		/**
		* Disabled constructor
		*/
		Logger() = delete;

		/**
		* Display error massage to target destination
		* @param p_message
		* @param p_logLevel
		* @param p_logMode
		* @param p_handlerID
		*/
		static void Log(const std::string& p_message, ELogLevel p_logLevel = ELogLevel::LOG_DEFAULT, ELogMode p_logMode = ELogMode::DEFAULT, std::string p_handlerId = "default");

		/**
		* Create console handler
		* @param p_id
		*/
		static ConsoleHandler& CreateConsoleHandler(std::string p_id);

		/**
		* Create console handler
		* @param p_id
		*/
		static FileHandler& CreateFileHandler(std::string p_id);

		/**
		* Create console handler
		* @param p_id
		*/
		static HistoryHandler& CreateHistoryHandler(std::string p_id);

		/**
		* Return target console handler
		* @param p_id
		*/
		static ConsoleHandler& GetConsoleHandler(std::string p_id);

		/**
		* Return target file handler
		* @param p_id
		*/
		static FileHandler& GetFileHandler(std::string p_id);

		/**
		* Return target history handler
		* @param p_id
		*/
		static HistoryHandler& GetHistoryHandler(std::string p_id);

	private:
		template<typename T>
		static void LogToHandlerMap(std::map<std::string, T>& p_map, const LogData& p_data, std::string p_id);

	public:
		static VisionTools::Eventing::Event<const LogData&> LogEvent;

	private:
		static std::map<std::string, ConsoleHandler>	CONSOLE_HANDLER_MAP;
		static std::map<std::string, FileHandler>		FILE_HANDLER_MAP;
		static std::map<std::string, HistoryHandler>	HISTORY_HANDLER_MAP;
	};
}

#include "VisionDebug/Logger.inl"