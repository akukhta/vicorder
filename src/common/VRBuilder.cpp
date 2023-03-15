#include "../../include/vicorder/VRBuilder.h"
#include "../../include/vicorder/VideoRecorder.h"

VRBuilder::VRBuilder() : recorder(std::make_unique<VideoRecorder>())
{
	;
}

VICORDER_API void VRBuilder::setTask(std::shared_ptr<TOTask> task)
{
	recorder->setTask(std::move(task));
}

VICORDER_API std::unique_ptr<VideoRecorder> VRBuilder::get()
{
	return std::move(recorder);
}
