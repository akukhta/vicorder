#pragma once
#include <Windows.h>
#include "VRBuilder.h"

class VideoRecorder;

/// <summary>
/// Video Recorder Builder Director
/// </summary>
class VRBuilderDefDirector
{
public:
	VICORDER_API VRBuilderDefDirector() = default;

	/// <summary>
	/// Produces default instance of Video Recorder
	/// </summary>
	/// <param name="monitor">Selected for capturing monitor's descriptor</param>
	/// <returns>Defualt contructed VideoRecorder</returns>
	VICORDER_API std::unique_ptr<VideoRecorder> getToAVI(HMONITOR monitor);

	VICORDER_API std::unique_ptr<VideoRecorder> getToJPEGs(HMONITOR monitor);

	VICORDER_API ~VRBuilderDefDirector() = default;

private:
	VRBuilder builder;
};