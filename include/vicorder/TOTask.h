#pragma once
#include <Windows.h>
#include <thread>
#include <iostream>

/// <summary>
/// a enum represents events
/// </summary>
enum class NOT_EVENT : char {STOP, FRAME_DUMPED};

/// <summary>
/// Class represents threaded observable task 
/// Templated method:
///		1. Create new tread 
///		2. Run templated task
/// </summary>
class TOTask
{
public:

	explicit TOTask(std::weak_ptr<TOTask> next) : next(next), stat(0), isRunning(false) { ; }
	/// <summary>
	/// Create new thread and run task
	/// </summary>
	virtual void run()
	{
		isRunning = true;
		tHandler = std::thread(&TOTask::execute, this);
	}

	/// <summary>
	/// While command is running, execute the handler function
	/// </summary>
	virtual void execute()
	{
		while (isRunning)
		{
			handle();
		}
	}

	/// <summary>
	/// Base stoping activities
	/// </summary>
	void stop()
	{
		isRunning = false;
		tHandler.join();
	}

	/// <summary>
	/// Returns stats
	/// </summary>
	/// <returns>Counter of handled events</returns>
	size_t getStat()
	{
		return stat;
	}

	/// <summary>
	/// Templated method
	/// </summary>
	virtual void handle() = 0;

	virtual void pushToNext() = 0;

	/// <summary>
	/// Handle events from observable object
	/// </summary>
	/// <param name="event">Event</param>
	virtual void notify(NOT_EVENT event) = 0;

protected:

	std::thread tHandler;
	std::atomic_bool isRunning;
	size_t stat;
	std::weak_ptr<TOTask> next;
};