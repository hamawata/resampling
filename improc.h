#include <iostream>
#include "C:\OpenCV2.2\include\opencv2\opencv.hpp"

class improc
{

private:
	int inter;
	int kernel_size;
	int k_size_harf;
	cv::Size src_size;
	cv::Size dst_size;
	double scale_x;
	double scale_y;

	cv::Mat &_getKernel(cv::Point point);
	cv::Mat &_getKernel(cv::Point point, cv::Rect src_rect);
	cv::Rect _getTargetRect(cv::Point point);
	cv::Point2f _dstPointToSrc(cv::Point point);
	double _distanceToWeight(double distance, int interpolate = NULL);

public:
	improc();
	int resizeImage(const cv::Mat src, cv::Mat &dst);

};