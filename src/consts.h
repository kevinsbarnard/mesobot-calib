#ifndef CONSTS_H
#define CONSTS_H

static const cv::Size CHESSBOARD_SIZE(9, 6);

static const int CHESSBOARD_FLAGS =
// cv::CALIB_CB_ADAPTIVE_THRESH +
cv::CALIB_CB_NORMALIZE_IMAGE +
// cv::CALIB_CB_FAST_CHECK +
0;

static const int CALIBRATION_FLAGS =
cv::CALIB_FIX_ASPECT_RATIO + // Optimize fy of cam R, fix ratio fx/fy of cam R
cv::CALIB_ZERO_TANGENT_DIST + // Fix tangential distortion coeffs to 0
cv::CALIB_USE_INTRINSIC_GUESS + // Optimize intrinsic params
// cv::CALIB_USE_LU + // Solve using Gaussian elimination (very fast)
// cv::CALIB_USE_QR + // Solve using QR decomposition (fast)
cv::CALIB_SAME_FOCAL_LENGTH + // Assume same focal length (fx L = fx R, fy L = fY R)
cv::CALIB_RATIONAL_MODEL + // Enable k4,5,6
0;

static const double CHESSBOARD_LENGTH = 0.0225; // Square side length in meters

#endif