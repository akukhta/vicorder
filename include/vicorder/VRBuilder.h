#pragma once
#include "VideoRecorder.h"
#include "../../src/common/dllinterface.h"
/// <summary>
/// Builder for VideoRecorded objects
/// </summary>
class VRBuilder 
{
public:

	VICORDER_API VRBuilder() : recorder(std::make_unique<VideoRecorder>()) { ; }

	VICORDER_API void setTask(std::shared_ptr<TOTask> task)
	{
		recorder->setTask(std::move(task));
	}

	/// <summary>
	/// Get instance of video recorder
	/// </summary>
	/// <returns>Created VideoRecorder</returns>
	VICORDER_API std::unique_ptr<VideoRecorder> get()
	{
		return std::move(recorder);
	}

private:
	std::unique_ptr<VideoRecorder> recorder;
};