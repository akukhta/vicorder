#pragma once
#include "VRBuilder.h"
#include "../../src/common/CaptureTask.h"
#include "../../src/common/CompressingTask.h"
#include "../../include/vicorder/OutputTask.h"
#include "../../src/common/VideoOutput.h"
#include "../../src/common/FrameToDiskTask.h"
#include "../../src/common/AviCreateTask.h"
#include "../../src/common/dllinterface.h"

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
	VICORDER_API std::unique_ptr<VideoRecorder> getToAVI(HMONITOR monitor)
	{
		std::shared_ptr<ExternalStorageManager> manager = std::make_shared<ExternalStorageManager>();
		std::shared_ptr<TOTask> outputTask = std::make_shared<AviCreateTask>(CommonUtiles::getMonitorsSizeCV(monitor), manager);
		std::shared_ptr<TOTask> frameDumpTask = std::make_shared<FrameToDiskTask>(manager, outputTask);
		std::shared_ptr<TOTask> compressingTask = std::make_shared<CompressingTask>(frameDumpTask);
		std::shared_ptr<TOTask> screeningTask = std::make_shared<CaptureTask>(monitor, compressingTask);

		builder.setTask(std::move(screeningTask));
		builder.setTask(std::move(compressingTask));
		builder.setTask(std::move(frameDumpTask));
		builder.setTask(std::move(outputTask));

		return builder.get();
	}
	
	VICORDER_API std::unique_ptr<VideoRecorder> getToJPEGs(HMONITOR monitor)
	{
		std::shared_ptr<TOTask> outputTask = std::make_shared<OutputTask>();
		std::shared_ptr<TOTask> compressingTask = std::make_shared<CompressingTask>(outputTask);
		std::shared_ptr<TOTask> screeningTask = std::make_shared<CaptureTask>(monitor, compressingTask);

		builder.setTask(std::move(screeningTask));
		builder.setTask(std::move(compressingTask));
		builder.setTask(std::move(outputTask));

		return builder.get();
	}

	

private:
	VRBuilder builder;
};