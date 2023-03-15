#pragma once
#include <Windows.h>

// Forward declaration to usable tasks
class CaptureTask;
class CompressingTask;
class FrameToDiskTask;

class TaskVisitor 
{
public:
	TaskVisitor() = delete;

	static void pushImage(CaptureTask &task, HBITMAP& bMap);
	static void pushCompressingTask(CompressingTask &task);
	static void pushToAvi(FrameToDiskTask &task);
};