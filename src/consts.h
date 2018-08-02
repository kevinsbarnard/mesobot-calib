#ifndef CONSTS_H
#define CONSTS_H

static const cv::Size CHESSBOARD_SIZE(9, 6);

static const int CHESSBOARD_FLAGS =
// cv::CALIB_CB_ADAPTIVE_THRESH + // Use adaptive thresholding to convert the image to grayscale
// cv::CALIB_CB_NORMALIZE_IMAGE + // Normalize the image gamma with equalizeHist before applying fixed or adaptive thresholding
// cv::CALIB_CB_FILTER_QUADS + // Use additional criteria (like contour area, perimeter, square-like shape) to filter out false quads extracted at the contour retrieval stage
// cv::CALIB_CB_FAST_CHECK + // Run a fast check on the image that looks for chessboard corners, and shortcut the call if none is found
0;

static const int CALIBRATION_FLAGS =
cv::CALIB_USE_INTRINSIC_GUESS + // Optimize some or all of the intrinsic parameters according to the specified flags
// cv::CALIB_FIX_ASPECT_RATIO + // Optimize f(j)y . Fix the ratio f(j)x/f(j)y
// cv::CALIB_FIX_PRINCIPAL_POINT + // Fix the principal points during the optimization
// cv::CALIB_ZERO_TANGENT_DIST + // Fix tangential distortion coeffs to 0
// cv::CALIB_FIX_FOCAL_LENGTH + // Fix f(j)x and f(j)y
// cv::CALIB_FIX_K1 +
// cv::CALIB_FIX_K2 +
// cv::CALIB_FIX_K3 +
// cv::CALIB_FIX_K4 +
// cv::CALIB_FIX_K5 +
// cv::CALIB_FIX_K6 +
// cv::CALIB_RATIONAL_MODEL + // Enables k4, k5, k6
// cv::CALIB_THIN_PRISM_MODEL + // Enables s1, s2, s3, s4
// cv::CALIB_FIX_S1_S2_S3_S4 + // Only if using thin prism model
// cv::CALIB_TILTED_MODEL + // Enables tauX, tauY
// cv::CALIB_FIX_TAUX_TAUY + // Only if using tilted model
// cv::CALIB_USE_QR + // QR decomposition solution method
// cv::CALIB_FIX_TANGENT_DIST + // Fix tangential distortion coeffs
// cv::CALIB_FIX_INTRINSIC + // Fix cameraMatrix and distCoeffs so that only R, T, E , and F matrices are estimated
// cv::CALIB_SAME_FOCAL_LENGTH + // Enforce f(0)x=f(1)x and f(0)y=f(1)y
// cv::CALIB_USE_LU + // Gaussian elimination solution method
0;

static const double CHESSBOARD_LENGTH = 0.0225; // Square side length in meters

#endif