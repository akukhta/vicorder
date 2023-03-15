#pragma once
#include <Windows.h>
#include <string>
#include <vector>
#include <chrono>
#include <filesystem>
#include <mutex>
#include <span>
#include "../common/CommonUtiles.h"

template <typename T>
concept is_container = requires (T a)
{
	a.begin();
	a.end();
	a.size();
};

class ExternalStorageManager;

class ExternalStorage
{
public:
	ExternalStorage();
	ExternalStorage(ExternalStorage const &) = delete;
	ExternalStorage(ExternalStorage&& strg) noexcept;

	size_t getFramesCount();

	friend void operator<<(ExternalStorage& storage, std::vector<unsigned char> const& data);
	
	friend class ExternalStorageManager;

	cv::Mat readFrame();

	~ExternalStorage();

private:
	static size_t const capacity;

	std::string name;
	HANDLE createdFile;
	HANDLE fileMapping;

	unsigned char* memPtr;

	size_t framesCount;
	size_t framesRead;
	size_t memoryAvailable;
	size_t writeOffset;
	size_t readOffset;

	std::mutex IOMut;

	template <class T> requires std::is_pod_v<T>
	void save(T val)
	{
		unsigned char* indxPtr = reinterpret_cast<unsigned char*>(&val);
		std::copy(indxPtr, indxPtr + sizeof(val), memPtr + writeOffset);

		size_t written = sizeof(val);

		writeOffset += written;
		memoryAvailable -= written;
	}

	template <class T> requires is_container<T>
	void save(T &&ctr)
	{
		std::copy(ctr.begin(), ctr.end(), memPtr + writeOffset);

		size_t written = ctr.size() * sizeof(typename std::remove_cvref<T>::type::value_type);

		writeOffset += written;
		memoryAvailable -= written;
	}
};

