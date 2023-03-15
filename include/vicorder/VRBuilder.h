#pragma once
#include <memory>
#include "../../src/common/dllinterface.h"

class VideoRecorder;
class TOTask;

/// <summary>
/// Builder for VideoRecorded objects
/// </summary>
class VRBuilder 
{
public:

	VICORDER_API VRBuilder();

	VICORDER_API void setTask(std::shared_ptr<TOTask> task);

	/// <summary>
	/// Get instance of video recorder
	/// </summary>
	/// <returns>Created VideoRecorder</returns>
	VICORDER_API std::unique_ptr<VideoRecorder> get();

	VICORDER_API ~VRBuilder() = default;

private:
	std::unique_ptr<VideoRecorder> recorder;
};