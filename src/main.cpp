// main.cpp : Defines the entry point for the console application.
// 
// Author : Kevin Barnard, Monterey Bay Aquarium Research Institute

#include "stdafx.h"

#include "calib.h"
#include "consts.h"
#include "functions.h"


using namespace std;
using namespace cv;

int main(int argc, char** argv) {

	//-----------------------------------------
	// Stereo calibration
	//-----------------------------------------
	vector<vector<String>> urls;
	urls.resize(2);
	
	glob("\\\\atlas.shore.mbari.org\\BioInspirLab2018\\180709_MiniRov_TestTank\\calibration\\left\\*.png", urls[0]);
	glob("\\\\atlas.shore.mbari.org\\BioInspirLab2018\\180709_MiniRov_TestTank\\calibration\\right\\*.png", urls[1]);
	vector<vector<Mat>> cameraMatrices;

	cameraMatrices.resize(2);
	for (auto & m : cameraMatrices) m.resize(2);

	vector<Mat> RTEF;
	vector<vector<Mat>> undistortRectifyMaps;
	Mat errors, Q;
	vector<Rect> rois;
	vector<Mat> rectifyMats, projMats;

	rois.resize(2);
	rectifyMats.resize(2);
	projMats.resize(2);

	calibStereo(urls[0], urls[1], cameraMatrices, RTEF, errors, rois, rectifyMats, projMats, Q, undistortRectifyMaps, true, 0.4);
	

	//-----------------------------------------
	// Video capture
	//-----------------------------------------
	VideoCapture cam1, cam2;

	cam1.open(CAM_1_SOURCE);
	cam2.open(CAM_2_SOURCE);

	vector<Mat> frames, grayFrames, displayFrames;
	frames.resize(2);
	grayFrames.resize(2);
	displayFrames.resize(2);

	vector<vector<KeyPoint>> keypoints;

	SimpleBlobDetector::Params blobDetectorParams;
	blobDetectorParams.blobColor = 255;
	blobDetectorParams.filterByArea = true;
	blobDetectorParams.minArea = 400;
	blobDetectorParams.maxArea = 10000;
	blobDetectorParams.filterByCircularity = false;
	blobDetectorParams.filterByConvexity = false;
	blobDetectorParams.filterByInertia = false;
	blobDetectorParams.minThreshold = 140;
	blobDetectorParams.maxThreshold = 255;
	blobDetectorParams.thresholdStep = 20;
	Ptr<SimpleBlobDetector> blobDetector = SimpleBlobDetector::create(blobDetectorParams);

	Mat display;

	Size frameSize;

	double preprocScalar = 1.0;
	double displayScalar = 0.4;

	bool tracking = true;

	vector<vector<Point2f>> trackedPoints;
	trackedPoints.resize(2);
	Mat triangulatedPoints, cartesianPoints;

	vector<Mat> detectionMasks;
	detectionMasks.resize(2);

	VideoWriter writer;

	bool freshStart = true;

	Size graphSize(500, 300);
	Mat graph, emptyGraph = Mat::zeros(graphSize, CV_8UC3);

	vector<double> lastAbsDistances, absDistances;

	Mat view3D(Size(min(rois[0].width, rois[1].width), min(rois[0].height, rois[1].height)), CV_8UC3);

	unsigned int skipFrames = 2;

	// Initial frame skip
	for (int i = 0; i < 300; i++) {
		cam1.grab();
		cam2.grab();
	}

	//-----------------------------------------
	// Display loop
	//-----------------------------------------
	for (int k = 0, frameNumber = 0; k != 27 && cam1.read(frames[0]) && cam2.read(frames[1]); k = waitKeyEx(30)) {

		if (frameNumber % graphSize.width == 0) emptyGraph.copyTo(graph);
		
		if (k == SPACEBAR) tracking = !tracking;

		// Draw view3D lines
		Rect minRoi(0, 0, view3D.cols, view3D.rows);
		rectangle(view3D, minRoi, Scalar(128, 0, 0), CV_FILLED);
		Point origin(minRoi.width/2, minRoi.height/2);
		int hw = minRoi.width / 2, hh = minRoi.height / 2;
		Scalar color_to_use = COLOR_GRAY;
		for (int i = 0; i < max(hw, hh); i += 20) {
			line(view3D, origin + Point(i, -hh), origin + Point(i, hh), color_to_use);
			line(view3D, origin + Point(-i, -hh), origin + Point(-i, hh), color_to_use);
			line(view3D, origin + Point(-hw, i), origin + Point(hw, i), color_to_use);
			line(view3D, origin + Point(-hw, -i), origin + Point(hw, -i), color_to_use);
		}
		line(view3D, origin + Point(0, -hh), origin + Point(0, hh), Scalar(255, 255, 255));
		line(view3D, origin + Point(-hw, 0), origin + Point(hw, 0), Scalar(255, 255, 255));
		
		for (int i = 0; i < 2; i++) {
			remap(frames[i], frames[i], undistortRectifyMaps[i][0], undistortRectifyMaps[i][1], INTER_LINEAR);
		}
		
		frameSize = frames[0].size();
		Size 
			outputFrameSize(frameSize.width * displayScalar, frameSize.height * displayScalar),
			outputSize((int)outputFrameSize.width * 2, outputFrameSize.height);

		for (int i = 0; i < frames.size(); i++) {
			scale(frames[i], preprocScalar);
			frames[i].copyTo(displayFrames[i]);
			cvtColor(frames[i], grayFrames[i], COLOR_BGR2GRAY);
			detectionMasks[i] = Mat::zeros(frameSize, CV_8U);
			detectionMasks[i](rois[i]) = 255;
			rectangle(displayFrames[i], rois[i], Scalar(0, 255, 0), 2);
		}

		if (tracking) {
			blobDetector->detect(grayFrames, keypoints, detectionMasks);

			for (int i = 0; i < keypoints.size(); i++) {
				for (int j = 0; j < keypoints[i].size(); j++) drawMarker(displayFrames[i], keypoints[i][j].pt, Scalar(0, 0, 255), MarkerTypes::MARKER_CROSS, displayFrames[i].rows / 30, 2);
			}

			// Triangulate points into cartesian
			cartesianPoints.release();
			if (keypoints[0].size() == keypoints[1].size() && min(keypoints[0].size(), keypoints[1].size()) != 0) {
				getPoints(keypoints[0], trackedPoints[0]);
				getPoints(keypoints[1], trackedPoints[1]);
				for (int i = 0; i < trackedPoints[0].size(); i++) {
					vector<Point2f> tpl, tpr;
					tpl.push_back(trackedPoints[0][i]);
					tpr.push_back(trackedPoints[1][i]);
					Mat triP;
					triangulatePoints(projMats[0], projMats[1], tpl, tpr, triP);
					transpose(triP, triP);
					Mat homP;
					convertPointsFromHomogeneous(triP, homP);
					cartesianPoints.push_back(homP);
				}
				cout << cartesianPoints << endl << endl;
			}
		}

		// Draw graph and 3D view
		absDistances.resize(cartesianPoints.rows);
		for (int i = 0; i < displayFrames.size(); i++) {
			scale(displayFrames[i], outputFrameSize);
			if (tracking) {
				putText(displayFrames[i], "Blobs: " + to_string(keypoints[i].size()), Point(10, 20), 0, 0.5, COLOR_WHITE);
				for (int j = 0; j < cartesianPoints.rows; j++) {
					double x, y, z;
					x = cartesianPoints.at<float>(j, 0);
					y = cartesianPoints.at<float>(j, 1);
					z = cartesianPoints.at<float>(j, 2);
					putText(displayFrames[i], to_string(j) +": ("+ to_string(x) +", "+ to_string(y) +", "+ to_string(z) +")", Point(10, j*20+40), 0, 0.5, COLOR_WHITE);
					circle(view3D, Point(view3D.cols/2, view3D.rows/2) + Point(x*400, y*400), (z>0 && 1/z>0 ? (1/z * 20) : 1), COLOR_WHITE, CV_FILLED);
					absDistances[j] = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
				}
			}
		}

		for (int i = 0; i < min(absDistances.size(), lastAbsDistances.size()); i++) {
			int
				x = frameNumber % graphSize.width,
				y = graphSize.height - (int)(absDistances[i] * 100) - 1,
				lastX = max(0, frameNumber % graphSize.width - 1),
				lastY = graphSize.height - (int)(lastAbsDistances[i] * 100) - 1;
			line(graph, Point(lastX, lastY), Point(x, y), Scalar(255,255,255));
		}

		lastAbsDistances.resize(absDistances.size());
		for (int i = 0; i < absDistances.size(); i++) lastAbsDistances[i] = absDistances[i];

		hconcat(displayFrames[0], displayFrames[1], display);
		line(display, Point(display.cols / 2, 0), Point(display.cols / 2, display.rows), COLOR_WHITE);
		
		writer.write(display);
		imshow("Display", display);
		imshow("Distance graph", graph);
		imshow("3D View", view3D);

		for (int i = 0; i < skipFrames; i++) {
			cam1.grab();
			cam2.grab();
		}

		frameNumber++;
	}
	//-----------------------------------------
	cam1.release();
	cam2.release();
	writer.release();
	destroyAllWindows();

    return 0;
}

