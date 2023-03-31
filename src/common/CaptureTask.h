#pragma once
#include <Windows.h>
#include "CompressingTask.h"
#include "../../include/vicorder/TOTask.h"
#include "../../include/vicorder/TaskVisitor.h"

/// <summary>
/// Screen's capture task
/// </summary>
class CaptureTask : public TOTask 
{
public:
	/// <summary>
	/// Init winapi lib
	/// </summary>
	/// <param name="hmonitor">Descriptor of monitor selected to record</param>
	/// <param name="cpTask">Pointer to compressing command object</param>
	explicit CaptureTask(HMONITOR hmonitor, std::weak_ptr<TOTask> next = std::weak_ptr<TOTask>()) noexcept :
		TOTask(next)
	{
		this->monitor = hmonitor;
		mi.cbSize = sizeof(mi);
		GetMonitorInfo(this->monitor, &mi);
		resolution.first = mi.rcMonitor.right - mi.rcMonitor.left;
		resolution.second = mi.rcMonitor.bottom - mi.rcMonitor.top;

		hHeap = GetProcessHeap();

		ZeroMemory(&bfHeader, sizeof(BITMAPFILEHEADER));
		ZeroMemory(&biHeader, sizeof(BITMAPINFOHEADER));
		ZeroMemory(&bInfo, sizeof(BITMAPINFO));
		ZeroMemory(&bAllDesktops, sizeof(BITMAP));

		bfHeader.bfType = (WORD)('B' | ('M' << 8));
		bfHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
		biHeader.biSize = sizeof(BITMAPINFOHEADER);
		biHeader.biBitCount = 24;
		biHeader.biCompression = BI_RGB;
		biHeader.biPlanes = 1;
		biHeader.biWidth = static_cast<LONG>(resolution.first);
		biHeader.biHeight = static_cast<LONG>(resolution.second);

		bInfo.bmiHeader = biHeader;

		cbBits = (((24 * static_cast<DWORD>(resolution.first) + 31) & ~31) / 8) * static_cast<DWORD>(resolution.second);
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
			break;
		}

		default:
		{
			break;
		}
		}
	}

private:

	/// <summary>
	/// Mathod to take screenshot
	/// </summary>
	/// <returns>Descriptor to an Image</returns>
	HBITMAP getScreen() noexcept
	{
		hDC = CreateDC(NULL, mi.szDevice, NULL, NULL);

		hMemDC = CreateCompatibleDC(hDC);
		HBITMAP hBitmap = CreateDIBSection(hDC, &bInfo, DIB_RGB_COLORS, (VOID**)&bBits, NULL, 0);
		
		if (hBitmap != 0)
		{
			SelectObject(hMemDC, hBitmap);
		}

		BitBlt(hMemDC, 0, 0, static_cast<int>(resolution.first), static_cast<int>(resolution.second), hDC, 0, 0, SRCCOPY);

		CURSORINFO cinfo;
		ZeroMemory(&cinfo, sizeof(CURSORINFO));
		cinfo.cbSize = sizeof(CURSORINFO);
		GetCursorInfo(&cinfo);

		DrawIconEx(hMemDC, cinfo.ptScreenPos.x, cinfo.ptScreenPos.y, cinfo.hCursor, 0, 0, 0, NULL, DI_DEFAULTSIZE | DI_NORMAL);

		DeleteDC(hMemDC);
		DeleteDC(hDC);

		return hBitmap;
	}

	HMONITOR monitor;
	std::pair<size_t, size_t> resolution;
	BITMAPFILEHEADER bfHeader;
	BITMAPINFOHEADER biHeader;
	BITMAPINFO bInfo;
	HGDIOBJ hTempBitmap;
	BITMAP bAllDesktops;
	HDC hDC, hMemDC;
	LONG lWidth, lHeight;
	BYTE* bBits = NULL;
	HANDLE hHeap;
	DWORD cbBits, dwWritten = 0;
	MONITORINFOEX mi;

	HBITMAP bMap;

	/// <summary>
	/// Main activity (TM pattern)
	/// </summary>
	virtual void handle() override
	{
		bMap = getScreen();
		stat++;

		pushToNext();
	}

	virtual void pushToNext() override final
	{
		TaskVisitor::pushImage(*this, bMap);
	}

	friend class TaskVisitor;
};