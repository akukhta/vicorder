#pragma once
#include <string>
#include <type_traits>
#include <Windows.h>

namespace cv
{
	class Size;
}

class CommonUtiles
{
public:

	/// <summary>
	/// Function converts std::stirng to std::wstring
	/// </summary>
	/// <typeparam name="T">Can be std::string, std::string&</typeparam>
	/// <param name="str">String to convert</param>
	/// <returns>converted wstring </returns>
	template <class T> requires std::is_same_v<typename std::remove_cvref<T>::type, std::string>
	static std::wstring inline strToW(T&& str)
	{
		std::wstring w(str.begin(), str.end());
		return w;
	}

	/// <summary>
	/// Function returns monitor's size
	/// </summary>
	/// <param name="monitor">Monitor's handle</param>
	/// <returns>std::par<size_t, size_t>(width, height)</returns>
	static std::pair<size_t, size_t> getMonitorsSize(HMONITOR monitor)
	{
		MONITORINFOEX mi;

		mi.cbSize = sizeof(mi);
		GetMonitorInfo(monitor, &mi);

		return { mi.rcMonitor.right - mi.rcMonitor.left, mi.rcMonitor.bottom - mi.rcMonitor.top };
	}
	/// <summary>
	/// Function returns monitor's size for OpenCV API
	/// </summary>
	/// <param name="monitor">Monitor's handle</param>
	/// <returns>CVSIZE</returns>
	static std::unique_ptr<cv::Size> getMonitorsSizeCV(HMONITOR monitor);

	/// <summary>
	/// Helper function converts megabytes to bytes
	/// </summary>
	/// <param name="mb">Megabytes</param>
	/// <returns>Bytes</returns>
	static size_t mbToBytes(double mb)
	{
		return static_cast<size_t>(mb * 1024 * 1000);
	}

	/// <summary>
	/// Returns randomly generated string
	/// </summary>
	/// <returns></returns>
	static  std::string getRandomString()
	{
		return std::to_string(std::chrono::high_resolution_clock::now().time_since_epoch().count());
	}
};
