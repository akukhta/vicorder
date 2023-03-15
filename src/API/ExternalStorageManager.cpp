#include "ExternalStorageManager.h"

void ExternalStorageManager::write(std::vector<unsigned char> const& data)
{
    {
        std::scoped_lock sLock(stateMutex);

        if (!currentWriteStorage)
        {
            currentWriteStorage = std::make_shared<ExternalStorage>();
            currentReadStorage = currentWriteStorage;

        }
        else if (data.size() > currentWriteStorage->memoryAvailable)
        {
            if (currentWriteStorage != currentReadStorage)
            {
                usedStorages.push_back(std::move(currentWriteStorage));
            }
            currentWriteStorage = std::make_shared<ExternalStorage>();
        }
    }
    
    totalFramesCount++;
    (*currentWriteStorage) << data;
}

cv::Mat ExternalStorageManager::read()
{
    if (framesRead++ >= totalFramesCount)
    {
        throw std::runtime_error("Trying to read more frames than we actually have");
    }

    if (!validateCurrentReadStorage())
    {
        throw std::runtime_error("Nothing to read");
    }

    return currentReadStorage->readFrame();
}

size_t ExternalStorageManager::framesCount()
{
    return totalFramesCount;
}

bool ExternalStorageManager::hasToRead()
{
    std::scoped_lock sLock(stateMutex);
    return (!currentReadStorage && !usedStorages.empty()) || currentReadStorage && currentReadStorage->framesCount > currentReadStorage->framesRead;
}

bool ExternalStorageManager::isReadAllFrames()
{
    return framesRead >= totalFramesCount;
}

bool ExternalStorageManager::validateCurrentReadStorage()
{
    if (currentReadStorage == nullptr && !usedStorages.empty())
    {
        currentReadStorage = std::move(usedStorages.front());
        usedStorages.pop_front();
        return true;
    }
    else if (currentReadStorage && currentReadStorage->framesRead >= currentReadStorage->framesCount)
    {
        currentReadStorage = std::move(usedStorages.front());
        usedStorages.pop_front();
        return true;
    }
    else if (currentReadStorage && currentReadStorage->framesRead < currentReadStorage->framesCount)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void ExternalStorageManager::finishWrite()
{
    if (currentWriteStorage != currentReadStorage)
    {
        usedStorages.push_back(std::move(currentWriteStorage));
    }
}