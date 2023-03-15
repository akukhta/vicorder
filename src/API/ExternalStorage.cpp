#include "ExternalStorage.h"

size_t const ExternalStorage::capacity = 300;

ExternalStorage::ExternalStorage() : memoryAvailable(CommonUtiles::mbToBytes(capacity)), writeOffset(0),
	readOffset(0), framesCount(0), framesRead(0),
	name(CommonUtiles::getRandomString() + ".tmp")
{
	createdFile = CreateFile(name.c_str(), GENERIC_ALL, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	fileMapping = CreateFileMapping(createdFile, NULL, PAGE_EXECUTE_READWRITE, 0, memoryAvailable, NULL);

	if (fileMapping != 0)
	{
		memPtr = static_cast<unsigned char*>(MapViewOfFile(fileMapping, FILE_MAP_ALL_ACCESS, 0, 0, memoryAvailable));
	}
}

ExternalStorage::ExternalStorage(ExternalStorage&& strg) noexcept : 
	name(std::move(strg.name)), createdFile(strg.createdFile), fileMapping(strg.fileMapping), 
	memPtr(strg.memPtr), framesCount(strg.framesCount), memoryAvailable(strg.memoryAvailable),
	writeOffset(strg.writeOffset), readOffset(strg.readOffset), framesRead(strg.framesRead)

{
	strg.createdFile = 0;
	strg.fileMapping = 0;
	strg.memPtr = nullptr;
	strg.framesCount = 0;
	strg.memoryAvailable = 0;
	strg.writeOffset = 0;
	strg.readOffset = 0;
	strg.framesRead = 0;
}

size_t ExternalStorage::getFramesCount()
{
	return framesCount;
}

cv::Mat ExternalStorage::readFrame()
{
	{
		std::scoped_lock l(IOMut);
	}

	size_t volatile sizeOfFrame = *reinterpret_cast<size_t*>(memPtr + readOffset);
	
	readOffset += sizeof(size_t);

	std::span<unsigned char> frame(memPtr + readOffset, memPtr + readOffset + sizeOfFrame);

	readOffset += sizeOfFrame;

	framesRead++;

	return cv::imdecode(cv::Mat(1, static_cast<int>(frame.size()), CV_8UC1, frame.data()), cv::IMREAD_COLOR);
}

ExternalStorage::~ExternalStorage()
{
	FlushViewOfFile(memPtr, writeOffset);
	UnmapViewOfFile(memPtr);
	CloseHandle(fileMapping);
	CloseHandle(createdFile);

	std::filesystem::remove(name);
}

void operator<<(ExternalStorage& storage, std::vector<unsigned char> const& data)
{
	if (storage.memPtr != 0)
	{
		std::scoped_lock l(storage.IOMut);
		storage.save(data.size());
		storage.save(data);
		storage.framesCount++;
	}
}
