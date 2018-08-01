// calib.cpp
// Function definitions for stereo calibration

#include "stdafx.h"

#include "calib.h"
#include "consts.h"

using namespace std;
using namespace cv;


// validatePairs
// Checks provided image vectors (assumed to be parallel) for chessboard points and generates imagePoints and validPairs vectors
// Returns assumed size of image
Size validatePairs(
	vector<String>& urls1,
	vector<String>& urls2,
	vector<vector<vector<Point2f>>>& imagePoints,
	vector<bool>& validPairs,
	const Size chessboardSize,
	const int chessboardFlags
) {
	Size imageSize;

	vector<Mat> frames;
	frames.resize(2);
	for (int i = 0; i < urls1.size(); i++) {
		frames[0] = imread(urls1[i], IMREAD_GRAYSCALE);
		frames[1] = imread(urls2[i], IMREAD_GRAYSCALE);
		imageSize = frames[0].size();
		cout << "[INFO] Detecting chessboard for pair " << i + 1 << ": ";
		validPairs[i] =
			findChessboardCorners(frames[0], chessboardSize, imagePoints[0][i], chessboardFlags) &&
			findChessboardCorners(frames[1], chessboardSize, imagePoints[1][i], chessboardFlags);
		if (validPairs[i]) {
			cout << "found" << endl;
			for (int j = 0; j < 2; j++) {
				cornerSubPix(frames[j], imagePoints[j][i], Size(11, 11), Size(-1, -1), TermCriteria(TermCriteria::COUNT + TermCriteria::EPS, 30, 0.01));
			}
		}
		else cout << "not found" << endl;
		if (frames[0].size() != frames[1].size()) cout << "[WARNING] Calibration image sizes are not the same for pair " << i + 1 << endl;
	}

	return imageSize;
}

// calibStereo
// Runs a stereo calibration, calculates stereo rectification matrices, and computes undistortion maps
// Iteratively reduces stereo calibration error
//     stereoCalibrate         -> RTEF, errors
//     stereoRectify           -> rectifyMats, projMats, Q, rois
//     initUndistortRectifyMap -> undistortRectifyMaps
// Returns final overall error of calibration
double calibStereo(
	vector<String>& imgs1, // List of image paths for first camera
	vector<String>& imgs2, // List of image paths for second camera
	const double epsilon, // Desired overall error
	const double errorThresholdScalar, // Scalar on the average error of a calibration to threshold pairs for elimination
	const Size chessboardSize, // Size of chessboard by interior corners
	const double chessboardLength, // Side length of a chessboard square
	const int chessboardFlags, // Flags for chessboard detection
	const int calibrationFlags // Flags for stereo calibration
) {

	double overallError = 1000;

	int numFiles = imgs1.size();

	Size imageSize;

	vector<vector<Mat>> cameraMatrices; // Camera intrinsics
	vector<Mat> RTEF; // R, T, E, and F matrices
	Mat errors; // Nx2 matrix of errors for each pair
	vector<Rect> rois; // Regions of interest in rectified views
	vector<Mat> rectifyMats; // Rectification matrices
	vector<Mat> projMats; // Projection matrices
	Mat Q; // 4x4 disparity-to-depth mapping matrix
	vector<vector<Mat>> undistortRectifyMaps; // Undistort and rectify maps for cv remap

	cameraMatrices.resize(2);
	RTEF.resize(4);
	rois.resize(2);
	rectifyMats.resize(2);
	projMats.resize(2);
	undistortRectifyMaps.resize(2);

	for (int i = 0; i < 2; i++) {
		cameraMatrices[i].resize(2);
		undistortRectifyMaps[i].resize(2);
	}

	FileStorage fs; // For settings

	vector<vector<vector<Point2f>>> imagePoints;
	imagePoints.resize(2);
	imagePoints[0].resize(numFiles);
	imagePoints[1].resize(numFiles);

	vector<bool> validPairs;
	validPairs.resize(numFiles);

	// Validate chessboard pairs
	imageSize = validatePairs(imgs1, imgs2, imagePoints, validPairs, chessboardSize, chessboardFlags);

	// Iterate to reduce error
	double lastError = overallError;
	while (overallError > epsilon) {

		vector<string> validUrls1, validUrls2;
		vector<vector<Point2f>> points1, points2;
		for (int i = 0; i < validPairs.size(); i++) {
			if (validPairs[i]) {
				validUrls1.push_back(imgs1[i]);
				validUrls2.push_back(imgs2[i]);
				points1.push_back(imagePoints[0][i]);
				points2.push_back(imagePoints[1][i]);
			}
		}

		numFiles = validUrls1.size();

		if (numFiles < 2) {
			cout << "[FATAL] Calibration failed. Calibration needs more at least 2 valid pairs of images" << endl;
			return -1;
		}

		// Generate 3D chessboard points
		vector<vector<Point3f>> chessboardPoints3D;
		chessboardPoints3D.resize(numFiles);
		for (int i = 0; i < numFiles; i++) {
			for (int y = 0; y < chessboardSize.height; y++) {
				for (int x = 0; x < chessboardSize.width; x++) {
					chessboardPoints3D[i].push_back(Point3f(chessboardLength*x, chessboardLength*y, 0));
				}
			}
		}

		cameraMatrices[0][0] = initCameraMatrix2D(chessboardPoints3D, points1, imageSize, 0);
		cameraMatrices[1][0] = initCameraMatrix2D(chessboardPoints3D, points2, imageSize, 0);

		cout << "[INFO] Calibrating on " << numFiles << " pairs" << endl;
		// Perform calibration
		overallError = stereoCalibrate(
			chessboardPoints3D,
			points1, points2,
			cameraMatrices[0][0], cameraMatrices[0][1],
			cameraMatrices[1][0], cameraMatrices[1][1],
			imageSize,
			RTEF[0], RTEF[1], RTEF[2], RTEF[3],
			errors,
			calibrationFlags,
			TermCriteria(TermCriteria::COUNT + TermCriteria::EPS, 100, 1e-5)
		);

		if (overallError > epsilon) cout << "[INFO] Overall error greater than epsilon (" << overallError << " > " << epsilon << ")" << endl;

		bool reduced = false;
		for (int i = 0, j = 0; i < validPairs.size(); i++) {
			if (validPairs[i]) {
				double errorPair = max(errors.at<double>(j, 0), errors.at<double>(j, 1));
				if (errorPair > errorThresholdScalar * overallError) {
					cout << "[INFO] Eliminated pair " << i + 1 << endl;
					validPairs[i] = false;
					reduced = true;
				}
				j++;
			}
		}

		if (overallError > lastError) {
			cout << "[WARNING] Overall error is increasing. Consider increasing threshold error proportion constant" << endl;
		}
		lastError = overallError;

		if (!reduced) {
			cout << "[WARNING] Calibration could not meet epsilon. Continuing with current error" << endl;
			break;
		}
	}

	cout << "[INFO] Overall RMS reprojection error of calibration: " << overallError << " pixels" << endl;

	// Compute stereo rectification
	cout << "[INFO] Computing rectification" << endl;
	stereoRectify(
		cameraMatrices[0][0], cameraMatrices[0][1],
		cameraMatrices[1][0], cameraMatrices[1][1],
		imageSize,
		RTEF[0], RTEF[1],
		rectifyMats[0], rectifyMats[1],
		projMats[0], projMats[1],
		Q, 0, 1, imageSize,
		&(rois[0]), &(rois[1])
	);

	// Compute undistort maps
	undistortRectifyMaps.resize(2);
	for (int i = 0; i < 2; i++) {
		undistortRectifyMaps[i].resize(2);
		initUndistortRectifyMap(
			cameraMatrices[i][0], cameraMatrices[i][1],
			rectifyMats[i], projMats[i],
			imageSize,
			CV_16SC2,
			undistortRectifyMaps[i][0], undistortRectifyMaps[i][1]
		);
	}

	cout << "[INFO] Writing calibration info to 'calib_settings.yml'" << endl;
	fs.open("calib_settings.yml", FileStorage::WRITE);
	if (fs.isOpened()) {
		fs
			<< "overallError" << overallError
			<< "errors" << errors
			<< "imageSize" << imageSize
			<< "M1" << cameraMatrices[0][0]
			<< "D1" << cameraMatrices[0][1]
			<< "M2" << cameraMatrices[1][0]
			<< "D2" << cameraMatrices[1][1]
			<< "R" << RTEF[0]
			<< "T" << RTEF[1]
			<< "E" << RTEF[2]
			<< "F" << RTEF[3]
			<< "rect1" << rectifyMats[0]
			<< "rect2" << rectifyMats[1]
			<< "proj1" << projMats[0]
			<< "proj2" << projMats[1]
			<< "Q" << Q;
		fs.release();
	}
	
	cout << "[INFO] Writing undistort rectify maps to 'undistort_maps.yml'" << endl;
	fs.open("undistort_maps.yml", FileStorage::WRITE);
	if (fs.isOpened()) {
		fs
			<< "map1_1" << undistortRectifyMaps[0][0]
			<< "map1_2" << undistortRectifyMaps[0][1]
			<< "map2_1" << undistortRectifyMaps[1][0]
			<< "map2_2" << undistortRectifyMaps[1][1];
		fs.release();
	}

	return overallError;
}