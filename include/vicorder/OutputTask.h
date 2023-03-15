#pragma once
#include <vector>
#include <mutex>
#include <queue>
#include <format>
#include <fstream>
#include <filesystem>
#include "TOTOutputTask.h"
#include "CommonUtiles.h"
#include "TaskVisitor.h"

/// <summary>
/// Output task, writes images to file
/// </summary>
class OutputTask final : public TOTOutputTask
{
public:
	explicit OutputTask(std::weak_ptr<TOTask> next = std::weak_ptr<TOTask>()) : TOTOutputTask(next) {};

private:
	std::ofstream out;

	/// <summary>
	/// Method to save image to disk
	/// </summary>
	/// <param name="buf">Buffer stores image</param>
	/// <param name="name">File name</param>
	virtual void save(std::vector<unsigned char> buf, std::string const & name) override final
	{
		HANDLE createdFile = CreateFile(name.c_str(), GENERIC_ALL, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		
		HANDLE mappedFile = CreateFileMapping(createdFile, NULL, PAGE_EXECUTE_READWRITE, 0, buf.size(), NULL);

		if (mappedFile == 0)
		{
			return;
		}

		unsigned char* buffer = static_cast<unsigned char*>(MapViewOfFile(mappedFile, FILE_MAP_ALL_ACCESS, 0, 0, buf.size()));

		if (buffer != nullptr)
		{
			std::copy(buf.begin(), buf.end(), buffer);
			
			buf.clear();
			buf.shrink_to_fit();

			FlushViewOfFile(buffer, buf.size());
			UnmapViewOfFile(buffer);
			CloseHandle(mappedFile);
			CloseHandle(createdFile);
		}
		else
		{
			CloseHandle(mappedFile);
			CloseHandle(createdFile);
		}
	}

	virtual void pushToNext() override final
	{
		;
	}
};