#include "../../include/vicorder/VideoRecorder.h"
#include "../../include/vicorder/TOTask.h"
#include "../../src/common/AviCreateTask.h"

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

#ifdef _DEBUG
void VideoRecorder::printDebugInfo()
{
	std::cout << std::format("Frames taken: {}, Frames compressed: {}, Frames written: {}", handlers[0]->getStat(), handlers[1]->getStat(), handlers[2]->getStat());
}
#endif

std::vector<size_t> VideoRecorder::getHandledEventCounts() const noexcept
{
	std::vector<size_t> counts;
	counts.reserve(handlers.size());

	std::for_each(handlers.begin(), handlers.end(), [&counts](auto x) { counts.push_back(x->getStat()); });
	
	return counts;
}

std::vector<HMONITOR> VideoRecorder::getMonitors()
{
	SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE);
	std::vector<HMONITOR> monitors;
	EnumDisplayMonitors(NULL, NULL, &VideoRecorder::getMonitorsHandlers, reinterpret_cast<LPARAM>(&monitors));
	return monitors;
}

bool VideoRecorder::save(std::string_view filePath) const noexcept
{
	if (auto aviOutput = dynamic_cast<AviCreateTask*>(handlers.back().get()))
	{
		std::filesystem::rename(aviOutput->getFileName(), filePath);
		return true;
	}

	return false;
}

BOOL VideoRecorder::getMonitorsHandlers(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData) noexcept
{
	reinterpret_cast<std::vector<HMONITOR>*>(dwData)->push_back(hMonitor);

	return true;
}