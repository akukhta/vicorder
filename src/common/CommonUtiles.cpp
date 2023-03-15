#include <opencv2/opencv.hpp>
#include "CommonUtiles.h"


std::unique_ptr<cv::Size> CommonUtiles::getMonitorsSizeCV(HMONITOR monitor)
{
	MONITORINFOEX mi;

	mi.cbSize = sizeof(mi);
	GetMonitorInfo(monitor, &mi);

	return std::make_unique<cv::Size>(mi.rcMonitor.right - mi.rcMonitor.left, mi.rcMonitor.bottom - mi.rcMonitor.top);
}
