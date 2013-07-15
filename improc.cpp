#include <iostream>
#include <math.h>
#include "C:\OpenCV2.2\include\opencv2\opencv.hpp"
#include "improc.h"

improc::improc()
{
	inter		= cv::INTER_CUBIC;
	kernel_size = 7;
	k_size_harf	= (int)(kernel_size / 2);
}

int improc::resizeImage(const cv::Mat src, cv::Mat &dst)
{
	scale_x = (double)dst.cols / (double)src.cols;
	scale_y = (double)dst.rows / (double)src.rows;
	src_size = src.size();
	dst_size = dst.size();
		
	for (int i = 0; i < dst.rows; i++) {
		for (int j = 0; j < dst.cols; j++) {
			cv::Mat weighted;			
			cv::Rect src_rect = _getTargetRect(cv::Point(j, i));

			cv::multiply(src(src_rect), _getKernel(cv::Point(j, i), src_rect), weighted);
			dst.at<cv::Scalar>(i, j) = cv::sum(weighted);
		}
	}
	return 0;
}

/**
 * _getKernel ��ݍ��݂��s���J�[�l�����擾����
 *
 * point	cv::Point	�o�͉摜�̍��W
 * src_rect	cv::Rect	���͉摜�̏�ݍ��݂��s���͈�
 * return	cv::Mat		���͉摜�ɑ΂���d�݃}�b�v
 *
 **/
cv::Mat &improc::_getKernel(cv::Point point, cv::Rect src_rect)
{
	cv::Mat kernel = cv::Mat::zeros(kernel_size, kernel_size, CV_32F);
	cv::Point2f center2f = _dstPointToSrc(point);

	for (int i = 0; i < src_rect.height; i++) {
		for (int j = 0; j < src_rect.width; j++) {
			double distance_x = center2f.x - (double)(j + src_rect.x);
			double distance_y = center2f.y - (double)(i + src_rect.y);
			double dxy = std::sqrtf(distance_x * distance_x + distance_y * distance_y);

			kernel.at<float>(i, j) = _distanceToWeight(dxy);
		}
	}
	cv::normalize(kernel, kernel, 1., NULL, cv::NORM_L1);
	return kernel;
}

/**
 * _getKernel ��ݍ��݂��s���J�[�l�����擾����
 *
 * point	cv::Point	�o�͉摜�̍��W
 * return	cv::Mat		���͉摜�ɑ΂���d�݃}�b�v
 *
 **/
cv::Mat &improc::_getKernel(cv::Point point)
{
	return _getKernel(point, _getTargetRect(point));
}

/**
 * _getTargetRect ���͉摜�̃J�[�l���Ƃ̏�ݍ��݂��s���͈͂��擾����
 *
 * point	cv::Point	�o�͉摜�̍��W
 * return	cv::Rect	���͉摜�̏�ݍ��݂��s���͈�
 *
 **/
cv::Rect improc::_getTargetRect(cv::Point point)
{
	cv::Rect rect;
	cv::Point center = (cv::Point)_dstPointToSrc(point);

	if (center.x - k_size_harf < 0) {
		rect.x		= 0;
		rect.width	= kernel_size - (center.x - k_size_harf);
	} else if (center.x + k_size_harf >= src_size.width) {
		rect.x		= center.x - k_size_harf;
		rect.width	= src_size.width - rect.x;
	} else {
		rect.x		= center.x - k_size_harf;
		rect.width	= kernel_size;
	}

	if (center.y - k_size_harf < 0) {
		rect.y		= 0;
		rect.height	= kernel_size - (center.y - k_size_harf);
	} else if (center.y + k_size_harf >= src_size.height) {
		rect.y		= center.y - k_size_harf;
		rect.height	= src_size.height - rect.y;
	} else {
		rect.y		= center.y - k_size_harf;
		rect.height	= kernel_size;
	}

	return rect;
}

/**
 * _dstPointToSrc �o�͉摜�̍��W����͉摜�̑Ή�������W�ɕϊ�����
 *
 * point	cv::Point	�o�͉摜�̍��W
 * return	cv::Point2f	���͉摜�̑Ή����鏭�����x���W
 *
 * TODO:���S�_�Ōv�Z
 **/
cv::Point2f improc::_dstPointToSrc(cv::Point point)
{
	double x = point.x / scale_x + 0.5 / scale_x - 0.5;
	double y = point.y / scale_y + 0.5 / scale_y - 0.5;

	return cv::Point2f(x, y);
}

/**
 * _distanceToWeight
 *
 **/
double improc::_distanceToWeight(double distance, int interpolate)
{
	if (interpolate != NULL) {
		inter = interpolate;
	}
	
	switch (inter) {
	case cv::INTER_CUBIC:
		if (distance >= 2.0) {
			return 0.;
		} else if (distance > 1.) {
			return 4. - 8. * distance + 5. * distance * distance - distance * distance * distance;
		} else {
			return 1. - 2 * distance * distance + distance * distance * distance;
		}
	default:
		return 0;
	}
}
