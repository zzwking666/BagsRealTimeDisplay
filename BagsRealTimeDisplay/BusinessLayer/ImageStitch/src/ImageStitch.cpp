#include "ImageStitch.hpp"
#include <algorithm>
#include <vector>
#include <cmath>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include "imgqt/imgqt.hpp"

#include "Modules.hpp"


ImageStitch::ImageStitch(QObject* parent)
{

}

ImageStitch::~ImageStitch()
{

}

void ImageStitch::onFrameCaptured(rw::rqwc::MatInfo matInfo, size_t index)
{
	/*if (matInfos.size() > matInfosSize)
	{
		matInfos.pop_front();
	}
	matInfos.push_back(matInfo);*/

	// 更新最新的图像的帧号
	//newFrameNum = static_cast<int>(matInfo.frameInfo.frameNum);

	//auto& setConfig = Modules::getInstance().configModule.setConfig;
	//auto stitchNum = setConfig.qiehuanzhangshu;

	//auto stitchedMat = stitchImages(stitchNum);

	auto testImg1 = cv::imread("C:/Users/zfkj4090/Downloads/test_ocr.png");
	auto testImg2 = cv::imread("C:/Users/zfkj4090/Downloads/222.jpg");
	static bool emitFirst = true;
	if (testImg1.empty() || testImg2.empty())
	{
		return;
	}

	if (emitFirst)
	{
		const QImage qimg = rw::img::cvMatToQImage(testImg1);
		emit imageReady(1, QPixmap::fromImage(qimg));
	}
	else
	{
		const QImage qimg = rw::img::cvMatToQImage(testImg2);
		emit imageReady(2, QPixmap::fromImage(qimg));
	}

	emitFirst = !emitFirst;
}

cv::Mat ImageStitch::stitchImages(int stitchCount)
{
	if (stitchCount <= 0 || matInfos.isEmpty())
	{
		return {};
	}

	const int latestFrame = newFrameNum;
	if (latestFrame <= 0)
	{
		return {};
	}

	const int minFrame = std::max(1, latestFrame - stitchCount + 1);

	QVector<rw::rqwc::MatInfo> selected;
	selected.reserve(stitchCount);

	for (int i = matInfos.size() - 1; i >= 0; --i)
	{
		const auto& info = matInfos[i];
		const int frameNum = static_cast<int>(info.frameInfo.frameNum);

		if (frameNum > latestFrame)
		{
			continue;
		}
		if (frameNum < minFrame)
		{
			break;
		}
		if (info.mat.empty())
		{
			continue;
		}

		selected.push_back(info);
		if (selected.size() == stitchCount)
		{
			break;
		}
	}

	if (selected.size() < stitchCount)
	{
		return {};
	}

	std::sort(selected.begin(), selected.end(),
		[](const rw::rqwc::MatInfo& a, const rw::rqwc::MatInfo& b)
		{
			return a.frameInfo.frameNum < b.frameInfo.frameNum;
		});

	for (const auto& info : selected)
	{
		if (static_cast<int>(info.frameInfo.frameNum) == lastStitchFrameNum)
		{
			return {};
		}
	}

	const int targetCols = selected.front().mat.cols;
	const int targetType = selected.front().mat.type();

	std::vector<cv::Mat> mats;
	mats.reserve(static_cast<size_t>(selected.size()));

	for (const auto& info : selected)
	{
		cv::Mat m = info.mat;

		if (m.type() != targetType)
		{
			cv::Mat converted;
			m.convertTo(converted, targetType);
			m = std::move(converted);
		}

		if (m.cols != targetCols)
		{
			const int newH = std::max(1, static_cast<int>(
				std::llround(static_cast<double>(m.rows) * targetCols / std::max(1, m.cols))));
			cv::resize(m, m, cv::Size(targetCols, newH), 0, 0, cv::INTER_LINEAR);
		}

		mats.push_back(std::move(m));
	}

	cv::Mat stitched;
	cv::vconcat(mats, stitched);

	lastStitchFrameNum = latestFrame;
	return stitched;
}
