#pragma once
#include <queue>
#include <vector>
#include <mutex>
#include "TOTask.h"

class TOTOutputTask : public TOTask
{
public:

	TOTOutputTask(std::weak_ptr<TOTask> next) : TOTask(next) {};

	void push(std::vector<unsigned char> buf)
	{
		std::scoped_lock l(imagesMut);
		images.push(std::move(buf));
	}

	/// <summary>
	/// Change limit of buffer and run parent templated execute method
	/// </summary>
	virtual void execute() override
	{
		currentLimit = maxLimit;

		TOTask::execute();
	}

	/// <summary>
	/// Handle events from observable object
	/// </summary>
	/// <param name="event">Event</param>
	virtual void notify(NOT_EVENT event) override
	{
		switch (event)
		{
		case NOT_EVENT::STOP:
		{
			TOTask::stop();

			currentLimit = 0;

			while (!images.empty())
			{
				handle();
			}

			stopHook();
			
			break;
		}

		default:
		{
			break;
		}
		}
	}

	/// <summary>
	/// Main activity (TM pattern)
	/// </summary>
	virtual void handle() override
	{
		bool isReached = false;

		{
			std::scoped_lock l(imagesMut);

			if (images.size() > currentLimit)
			{
				isReached = true;
			}
		}

		if (isReached)
		{
			std::queue<std::vector<unsigned char>> toWriteQueue;

			{
				std::scoped_lock l(imagesMut);
				toWriteQueue = std::move(images);
			}

			while (!toWriteQueue.empty())
			{
				auto buf = std::move(toWriteQueue.front());
				toWriteQueue.pop();
				save(std::move(buf), std::format("screen/{}.jpg", stat++));
			}
		}
	}

protected:
	std::queue<std::vector<unsigned char>> images;
	std::mutex imagesMut;
	size_t currentLimit = 0;
	size_t maxLimit = 10;

	virtual void save(std::vector<unsigned char> buf, std::string const& filename) = 0;
	
	/// <summary>
	/// Hook function that is being use in stop event handler, after processing of all remainig images,
	/// override in inherited classes
	/// </summary>
	virtual void stopHook()
	{
		;
	}

	virtual void pushToNext() override
	{
		;
	}
};