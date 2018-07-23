#ifndef CONSTS_H
#define CONSTS_H

// Camera 1
static const std::string CAM_1_SOURCE = "\\\\atlas.shore.mbari.org/BioInspirLab2018/180709_MiniRov_TestTank/july_11_2018/left_2018-07-11_09.32.35.933.m4v";
static const std::string CAM_1_NAME = "Camera Left";
static const std::string FRAME_1_NAME = "Frame Left";

// Camera 2
static const std::string CAM_2_SOURCE = "\\\\atlas.shore.mbari.org/BioInspirLab2018/180709_MiniRov_TestTank/july_11_2018/right_2018-07-11_09.32.35.933.m4v";
static const std::string CAM_2_NAME = "Camera Right";
static const std::string FRAME_2_NAME = "Frame Right";

static const int ARROWS_LRUD[] = { 2424832, 2555904, 2490368, 2621440 };
static const int SPACEBAR = 32;

static const cv::Size CHESSBOARD_SIZE(9, 6);

static const double DISPLAY_SCALE_VAL = 0.4; // Value to scale images post-process

static const int CHESSBOARD_FLAGS =
	cv::CALIB_CB_ADAPTIVE_THRESH +
	cv::CALIB_CB_NORMALIZE_IMAGE +
	cv::CALIB_CB_FAST_CHECK;

static const int CALIBRATION_FLAGS =
	cv::CALIB_FIX_ASPECT_RATIO + // Optimize fy of cam R, fix ratio fx/fy of cam R
	cv::CALIB_ZERO_TANGENT_DIST + // Fix tangential distortion coeffs to 0
	cv::CALIB_USE_INTRINSIC_GUESS + // Optimize intrinsic params
	cv::CALIB_USE_LU + // Solve using Gaussian elimination (very fast)
	// cv::CALIB_USE_QR + // Solve using QR decomposition (fast)
	cv::CALIB_SAME_FOCAL_LENGTH + // Assume same focal length (fx L = fx R, fy L = fY R)
	cv::CALIB_RATIONAL_MODEL + // Enable k4,5,6
	// cv::CALIB_FIX_K1 +
	// cv::CALIB_FIX_K2 +
	cv::CALIB_FIX_K3 +
	cv::CALIB_FIX_K4 +
	cv::CALIB_FIX_K5;
	// cv::CALIB_FIX_K6;

static const double CHESSBOARD_LENGTH = 0.0225; // Square side length in meters

static const cv::Scalar
	COLOR_WHITE(255, 255, 255),
	COLOR_GRAY = COLOR_WHITE - cv::Scalar(150, 150, 150);

#endif