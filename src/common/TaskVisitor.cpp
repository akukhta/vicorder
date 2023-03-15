#include "CaptureTask.h"
#include "FrameToDiskTask.h"
#include "../../include/vicorder/TaskVisitor.h"

void TaskVisitor::pushImage(CaptureTask& task, HBITMAP& bMap)
{
	if (auto compressingTask = task.next.lock())
	{
		if (auto ptr = dynamic_cast<CompressingTask*>(compressingTask.get()))
		{
			ptr->addToCommpress(bMap);
		}
	}
}

void TaskVisitor::pushCompressingTask(CompressingTask& task)
{
	if (auto t = task.next.lock())
	{
		if (auto ptr = dynamic_cast<TOTOutputTask*>(t.get()))
		{
			ptr->push(std::move(task.compressed));
		}
	}
}

void TaskVisitor::pushToAvi(FrameToDiskTask& task)
{
	if (auto t = task.next.lock())
	{
		t->notify(NOT_EVENT::FRAME_DUMPED);
	}
}
