#include "../../include/vicorder/VideoRecorder.h"

void VideoRecorder::setTask(std::shared_ptr<TOTask> task)
{
	handlers.push_back(std::move(task));
}

void VideoRecorder::startRecord()
{
	for (auto& task : handlers)
	{
		if (task)
		{
			task->run();
		}
	}
}

void VideoRecorder::stopRecord()
{
	for (auto& task : handlers)
	{
		if (task)
		{
			task->notify(NOT_EVENT::STOP);
		}
	}
}

VideoRecorder::~VideoRecorder()
{
	;
}

void VideoRecorder::printDebugInfo()
{
	std::cout << std::format("Frames taken: {}, Frames compressed: {}, Frames written: {}", handlers[0]->getStat(), handlers[1]->getStat(), handlers[2]->getStat());
}

std::vector<HMONITOR> VideoRecorder::getMonitors()
{
	SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE);
	std::vector<HMONITOR> monitors;
	EnumDisplayMonitors(NULL, NULL, &VideoRecorder::getMonitorsHandlers, reinterpret_cast<LPARAM>(&monitors));
	return monitors;
}

BOOL VideoRecorder::getMonitorsHandlers(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData)
{
	reinterpret_cast<std::vector<HMONITOR>*>(dwData)->push_back(hMonitor);

	return true;
}