#pragma once
#include <mutex>
#include <condition_variable>
#include "../../include/vicorder/TOTask.h"
#include "../API/ExternalStorageManager.h"

class AviCreateTask : public TOTask
{
public:
	explicit AviCreateTask(cv::Size size, std::shared_ptr<ExternalStorageManager> storageManager = nullptr,
		std::weak_ptr<TOTask> next = std::weak_ptr<TOTask>()) : TOTask(next), outFileName(CommonUtiles::getRandomString() + ".avi"),
		videoWriter(std::make_unique<cv::VideoWriter>(outFileName,
			cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 30, size, TRUE)), size(size), 
			storageManager(storageManager)
	{};

	std::string getFileName()
	{
		return outFileName;
	}

private:
	std::string outFileName;
	std::shared_ptr<ExternalStorageManager> storageManager;
	std::unique_ptr<cv::VideoWriter> videoWriter;
	cv::Size size;

	virtual void pushToNext() override final
	{
		;
	}

	virtual void handle() override final
	{
		if (storageManager)
		{
			if (!isRunning)
			{
				return;
			}

			if (storageManager->hasToRead())
			{
				write();
			}
			else
			{
				std::this_thread::yield();
			}
		}
	}

	void write()
	{
		if (storageManager)
		{
			videoWriter->write(storageManager->read());
		}
	}

	virtual void notify(NOT_EVENT event) override final
	{
		switch (event)
		{

			case NOT_EVENT::STOP:
			{
				isRunning = false;
				TOTask::stop();

				if (storageManager)
				{
					while (!storageManager->isReadAllFrames())
					{
						write();
					}
				}

				videoWriter->release();

				break;
			}

			default:
			{
				break;
			}
		}
	}
};