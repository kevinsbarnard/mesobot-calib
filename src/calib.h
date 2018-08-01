#ifndef CALIB_INCLUDE_H
#define CALIB_INCLUDE_H

#include "consts.h"

// Detect corners
cv::Size validatePairs(
	std::vector<cv::String>& imgs1,
	std::vector<cv::String>& imgs2,
	std::vector<std::vector<std::vector<cv::Point2f>>>& imagePoints,
	std::vector<bool>& validPairs,
	const cv::Size chessboardSize = CHESSBOARD_SIZE,
	const int chessboardFlags = CHESSBOARD_FLAGS
);

// Calibrate a stereo camera pair
double calibStereo(
	std::vector<cv::String>& imgs1,
	std::vector<cv::String>& imgs2,
	const double epsilon = 0.5,
	const double errorThresholdScalar = 1.4,
	const cv::Size chessboardSize = CHESSBOARD_SIZE,
	const double chessboardLength = CHESSBOARD_LENGTH,
	const int chessboardFlags = CHESSBOARD_FLAGS,
	const int calibrationFlags = CALIBRATION_FLAGS
);

#endif