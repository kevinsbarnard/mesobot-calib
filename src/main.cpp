// main.cpp : Defines the entry point for the console application.
// 
// Author : Kevin Barnard, Monterey Bay Aquarium Research Institute

#include "stdafx.h"

#include "calib.h"
#include "consts.h"


using namespace std;
using namespace cv;

int main(int argc, char** argv) {

	vector<vector<String>> imgs;
	imgs.resize(2);

	int calibFlags = CALIBRATION_FLAGS;
	int chessboardFlags = CHESSBOARD_FLAGS;
	Size chessboardSize = CHESSBOARD_SIZE;
	int chessboardLength = CHESSBOARD_LENGTH;

	string leftFolder, rightFolder;

	unsigned int width, height;
	double length = CHESSBOARD_LENGTH, targetVal = 0, threshVal = 0;

	for (int i = 0; i < argc; i++) {
		string arg = string(argv[i]);

		// Folders
		if (arg == "-l") leftFolder = string(argv[i + 1]);
		else if (arg == "-r") rightFolder = string(argv[i + 1]);

		// Chessboard params
		else if (arg == "-w") width = atoi(argv[i + 1]);
		else if (arg == "-h") height = atoi(argv[i + 1]);
		else if (arg == "-length") length = atof(argv[i + 1]);

		// Calibration threshold params
		else if (arg == "-target") targetVal = atof(argv[i + 1]);
		else if (arg == "-thresh") threshVal = atof(argv[i + 1]);

		// Models
		else if (arg == "-rational") calibFlags |= CALIB_RATIONAL_MODEL;
		else if (arg == "-thinprism") calibFlags |= CALIB_RATIONAL_MODEL;
		else if (arg == "-tilted") calibFlags |= CALIB_RATIONAL_MODEL;

		// Solution method
		else if (arg == "-lu") calibFlags |= CALIB_USE_LU;
		else if (arg == "-qr") calibFlags |= CALIB_USE_QR;

		// Fix distortion parameters
		else if (arg == "-k1") calibFlags |= CALIB_FIX_K1;
		else if (arg == "-k2") calibFlags |= CALIB_FIX_K2;
		else if (arg == "-k3") calibFlags |= CALIB_FIX_K3;
		else if (arg == "-k4") calibFlags |= CALIB_FIX_K4;
		else if (arg == "-k5") calibFlags |= CALIB_FIX_K5;
		else if (arg == "-k6") calibFlags |= CALIB_FIX_K6;
		else if (arg == "-s") calibFlags |= CALIB_FIX_S1_S2_S3_S4;
		else if (arg == "-tau") calibFlags |= CALIB_FIX_TAUX_TAUY;
		
		// Other calibration parameters
		else if (arg == "-fixar") calibFlags |= CALIB_FIX_ASPECT_RATIO;
		else if (arg == "-fixp") calibFlags |= CALIB_FIX_PRINCIPAL_POINT;
		else if (arg == "-fixtan") calibFlags |= CALIB_FIX_TANGENT_DIST;
		else if (arg == "-zerotan") calibFlags |= CALIB_ZERO_TANGENT_DIST;
		else if (arg == "-fixf") calibFlags |= CALIB_FIX_FOCAL_LENGTH;
		else if (arg == "-samef") calibFlags |= CALIB_SAME_FOCAL_LENGTH;


		// Chessboard detection
		else if (arg == "-at") chessboardFlags |= CALIB_CB_ADAPTIVE_THRESH;
		else if (arg == "-ni") chessboardFlags |= CALIB_CB_NORMALIZE_IMAGE;
		else if (arg == "-fq") chessboardFlags |= CALIB_CB_FILTER_QUADS;
		else if (arg == "-check") chessboardFlags |= CALIB_CB_FAST_CHECK;
	}

	if (leftFolder == "" || rightFolder == "") {
		cout << "[FATAL] Must specify folders. Specify with -l [LEFT_FOLDER] -r [RIGHT_FOLDER]" << endl;
		return -1;
	}

	if (threshVal == 0) {
		cout << "[FATAL] Threshold value must be non-zero. Specify with -thresh [THRESHOLD_VALUE]" << endl;
		return -1;
	}

	// Glob to find image paths
	glob(leftFolder + "/*.png", imgs[0]);
	glob(rightFolder + "/*.png", imgs[1]);
	if (imgs[0].size() != imgs[1].size()) {
		cout << "[FATAL] Directories must contain same number of images. Terminating." << endl;
		return -1;
	}
	cout << "[INFO] Found " << imgs[0].size() << " pairs" << endl;

	// Run calibration
	calibStereo(imgs[0], imgs[1], targetVal, threshVal, Size(width, height), length, chessboardFlags, calibFlags);

    return 0;
}
