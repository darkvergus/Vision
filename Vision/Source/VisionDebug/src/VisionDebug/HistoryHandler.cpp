#include "VisionDebug/HistoryHandler.h"

std::queue<VisionDebug::LogData> VisionDebug::HistoryHandler::LOG_QUEUE;

void VisionDebug::HistoryHandler::Log(const LogData& p_logData)
{
	LOG_QUEUE.push(p_logData);
}

std::queue<VisionDebug::LogData>& VisionDebug::HistoryHandler::GetLogQueue()
{
	return LOG_QUEUE;
}