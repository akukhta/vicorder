#pragma once
#include <memory>
#include <opencv2/core/mat.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include "../../include/vicorder/TOTOutputTask.h"
#include "../../include/vicorder/TaskVisitor.h"

class VideoOutput : public TOTOutputTask
{
public:

	explicit VideoOutput(cv::Size size, std::weak_ptr<TOTask> next = std::weak_ptr<TOTask>()) : TOTOutputTask(next), a(std::make_unique<cv::VideoWriter>("out.avi",
		cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 30, size, TRUE)), size(size)
	{
		;
	}

private:
	std::unique_ptr<cv::VideoWriter> a;
	cv::Size size;

	/// <summary>
	/// Method to save image to disk
	/// </summary>
	/// <param name="buf">Buffer stores image</param>
	/// <param name="name">File name</param>
	virtual void save(std::vector<unsigned char> buf, [[maybe_unused]] std::string const& name) override final
	{
		cv::Mat imgMat = cv::imdecode(cv::Mat(1, buf.size(), CV_8UC1, buf.data()), cv::IMREAD_COLOR);
		a->write(imgMat);

		imgMat.release();
	}

	virtual void stopHook() override
	{
		a->release();
	}

	virtual void pushToNext() override final
	{
		;
	}
};