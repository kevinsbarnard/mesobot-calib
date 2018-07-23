#ifndef FUNCTIONS_INCLUDE_H
#define FUNCTIONS_INCLUDE_H

void scale(cv::InputOutputArray& src, const double scaleVal);
void scale(cv::InputOutputArray& src, const cv::Size size);
void getPoints(std::vector<cv::KeyPoint>& keypoints, std::vector<cv::Point2f>& points);

#endif
