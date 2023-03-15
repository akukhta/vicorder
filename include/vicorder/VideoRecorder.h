#pragma once
#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>
#include <Shlwapi.h>
#include <utility>
#include <vector>
#include <atlimage.h>
#include <string>
#include <fstream>
#include <thread>
#include <format>
#include <queue>
#include <mutex>
#include <atomic>
#include <array>
#include <iostream>
#include "../../src/common/dllinterface.h"

class TOTask;

class VideoRecorder
{
public:
	VideoRecorder() = default;

	VICORDER_API void setTask(std::shared_ptr<TOTask> task);

	/// <summary>
	/// Method to start recording
	/// </summary>
	VICORDER_API void startRecord();

	/// <summary>
	/// Method to stop recording
	/// </summary>
	VICORDER_API void stopRecord();
	VICORDER_API ~VideoRecorder();

	/// <summary>
	/// Print info of handled events
	/// </summary>
	VICORDER_API void printDebugInfo();

	/// <summary>
	/// Static function to get descriptors of monitors
	/// </summary>
	/// <returns>Vector of monitors' descrtoptors</returns>
	VICORDER_API static std::vector<HMONITOR> getMonitors();

	/// <summary>
	/// Saves avi file to passed as parameter path
	/// </summary>
	/// <param name="filePath">Path to save</param>
	/// <returns>true if saving was successfull, otherwise false </returns>
	//template <class T> requires (std::is_same_v<std::remove_cvref_t<T>, std::string> || std::is_same_v(std::remove_cvref_t<T>, char))
	VICORDER_API bool save(std::string_view filePath);
private:
	/// <summary>
	/// Chain of responsibility pattern implementation
	/// </summary>
	std::vector<std::shared_ptr<TOTask>> handlers;
	
	static BOOL getMonitorsHandlers(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData);
};

