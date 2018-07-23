// functions.cpp
// Helper functions definitions

#include "stdafx.h"

#include "functions.h"

using namespace std;
using namespace cv;

// Scale src by the scale value
void scale(InputOutputArray& src, const double scaleVal) {
	resize(src, src, Size(), scaleVal, scaleVal, INTER_LINEAR);
}

// Scale src to the size provided
void scale(InputOutputArray& src, const Size size) {
	resize(src, src, size, 0, 0, INTER_LINEAR);
}

// Get a vector of the Point2f points from a vector of keypoints
void getPoints(vector<KeyPoint>& keypoints, vector<Point2f>& points) {
	points.resize(keypoints.size());
	for (int i = 0; i < keypoints.size(); i++) points[i] = keypoints[i].pt;
}