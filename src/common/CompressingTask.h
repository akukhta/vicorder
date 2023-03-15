#pragma once
#include <Windows.h>
#include <queue>
#include <thread>
#include <mutex>
#include <atlimage.h>
#include "../../include/vicorder/TaskVisitor.h"
#include "../../include/vicorder/OutputTask.h"
#include "../../include/vicorder/TaskVisitor.h"

class CompressingTask : public TOTask
{
public:

	explicit CompressingTask(std::weak_ptr<TOTask> next = std::weak_ptr<TOTask>()) : TOTask(next) { ; }

	/// <summary>
	/// Add image to compressing queue
	/// </summary>
	/// <param name="bitmap">Bitmap to compress</param>
	void addToCommpress(HBITMAP bitmap)
	{
		std::scoped_lock l(qMuts);
		bitmaps.push(bitmap);
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

				while (!bitmaps.empty())
				{
					handle();
				}

				break;
			}

			default:
			{
				break;
			}
		}
	}

private:
	std::queue<HBITMAP> bitmaps;
	std::mutex qMuts;

	/// <summary>
	/// Method to convert image from BMP to JPEG
	/// </summary>
	/// <param name="map">Bitmap</param>
	/// <returns>Buffer stores image</returns>
	std::vector<unsigned char> compress(HBITMAP map)
	{
		std::vector<unsigned char> buf;
		IStream* stream = NULL;
		HRESULT hr = CreateStreamOnHGlobal(0, TRUE, &stream);
		CImage image;
		ULARGE_INTEGER liSize;

		image.Attach(map);
		image.Save(stream, Gdiplus::ImageFormatJPEG);
		IStream_Size(stream, &liSize);
		DWORD len = liSize.LowPart;
		IStream_Reset(stream);
		buf.resize(len);
		IStream_Read(stream, &buf[0], len);
		stream->Release();

		return buf;
	}

	/// <summary>
	/// Main activity (TM pattern)
	/// </summary>
	virtual void handle() override
	{
		HBITMAP bMap = 0;

		{
			std::scoped_lock l(qMuts);

			if (!bitmaps.empty())
			{
				bMap = bitmaps.front();
				bitmaps.pop();
			}

		}

		if (bMap != 0)
		{
			compressed = compress(bMap);
			stat++;

			pushToNext();
		}
	}

	virtual void pushToNext() override final
	{
		TaskVisitor::pushCompressingTask(*this);
	}

	std::vector<unsigned char> compressed;

	friend class TaskVisitor;
};