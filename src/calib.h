#ifndef CALIB_INCLUDE_H
#define CALIB_INCLUDE_H

#include "consts.h"

// Detect corners
cv::Size validatePairs(
	std::vector<cv::String>& urls1,
	std::vector<cv::String>& urls2,
	std::vector<std::vector<std::vector<cv::Point2f>>>& imagePoints,
	std::vector<bool>& validPairs,
	const cv::Size chessboardSize = CHESSBOARD_SIZE,
	const int chessboardFlags = CHESSBOARD_FLAGS
);

// Calibrate a stereo camera pair
double calibStereo(
	std::vector<cv::String>& urls1,
	std::vector<cv::String>& urls2,
	std::vector<std::vector<cv::Mat>>& cameraMatrices,
	std::vector<cv::Mat>& RTEF,
	cv::Mat& errors,
	std::vector<cv::Rect>& rois,
	std::vector<cv::Mat>& rectifyMats,
	std::vector<cv::Mat>& projMats,
	cv::Mat& Q,
	std::vector<std::vector<cv::Mat>>& undistortRectifyMaps,
	const bool doCalibration = true,
	const double epsilon = 0.5,
	const cv::Size chessboardSize = CHESSBOARD_SIZE,
	const double chessboardLength = CHESSBOARD_LENGTH,
	const int chessboardFlags = CHESSBOARD_FLAGS,
	const int calibrationFlags = CALIBRATION_FLAGS
);

#endif