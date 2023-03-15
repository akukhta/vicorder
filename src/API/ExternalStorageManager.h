#pragma once
#include <mutex>
#include <memory>
#include <list>
#include <cassert>
#include "ExternalStorage.h"

/// <summary>
/// Singleton Manager of External Storage TMP files
/// </summary>

class ExternalStorageManager
{
public:
	ExternalStorageManager() = default;
	
	void write(std::vector<unsigned char> const & data);
	void finishWrite();

	cv::Mat read();
	size_t framesCount();

	bool hasToRead();
	bool isReadAllFrames();
	
private:

	std::list <std::shared_ptr<ExternalStorage>> usedStorages;
	std::shared_ptr<ExternalStorage> currentWriteStorage;
	std::shared_ptr<ExternalStorage> currentReadStorage;

	size_t totalFramesCount = 0;
	size_t framesRead = 0;

	std::mutex stateMutex;

	bool validateCurrentReadStorage();
};

