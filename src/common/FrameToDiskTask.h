#pragma once
#include <vector>
#include <string>
#include "../API/ExternalStorageManager.h"
#include "../../include/vicorder/TOTOutputTask.h"
#include "../../include/vicorder/TaskVisitor.h"

class FrameToDiskTask : public TOTOutputTask
{
public:

	explicit FrameToDiskTask(std::shared_ptr<ExternalStorageManager> storageManager = nullptr,
		std::weak_ptr<TOTask> next = std::weak_ptr<TOTask>())
		: storageManager(storageManager), TOTOutputTask(next) {};

	virtual void save(std::vector<unsigned char> buf, [[maybe_unused]] std::string const& name) override final
	{
		storageManager->write(std::move(buf));
		
		if (auto t = next.lock())
		{
			t->notify(NOT_EVENT::FRAME_DUMPED);
		}
	}

	virtual void stopHook() override
	{
		storageManager->finishWrite();
	}

	virtual void pushToNext() override
	{
		TaskVisitor::pushToAvi(*this);
	}

	friend class TaskVisitor;
	std::shared_ptr<ExternalStorageManager> storageManager;
};