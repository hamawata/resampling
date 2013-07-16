#include <iostream>
#include <math.h>
#include "C:\OpenCV2.2\include\opencv2\opencv.hpp"
#include "improc.h"

improc::improc()
{
	inter		= cv::INTER_CUBIC;
	kernel_size = 5;
	k_size_harf	= (int)(kernel_size / 2);
}

improc::improc(int interpolate)
{
	inter		= interpolate;
	kernel_size = 5;
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
			weighted = src(src_rect).mul(_getKernel(cv::Point(j, i), src_rect));
			cv::Scalar sum = cv::sum(weighted); 
			dst.at<cv::Vec3f>(i, j) = cv::Vec3f(sum(0), sum(1), sum(2));
		}
	}
	return 0;
}

/**
 * _getKernel ��ݍ��݂��s���J�[�l�����擾����
 *d
 * point	cv::Point	�o�͉摜�̍��W
 * src_rect	cv::Rect	���͉摜�̏�ݍ��݂��s���͈�
 * return	cv::Mat		���͉摜�ɑ΂���d�݃}�b�v
 *
 **/
cv::Mat improc::_getKernel(cv::Point point, cv::Rect src_rect)
{
	cv::Mat kernel = cv::Mat::zeros(src_rect.height, src_rect.width, CV_32FC3);
	cv::Point2f center2f = _dstPointToSrc(point);

	double sum = 0., weight;
	for (int i = 0; i < src_rect.height; i++) {
		for (int j = 0; j < src_rect.width; j++) {
			double distance_x = abs(center2f.x - (double)(j + src_rect.x));
			double distance_y = abs(center2f.y - (double)(i + src_rect.y));
			sum += weight =_distanceToWeight(distance_x, distance_y);
			kernel.at<cv::Vec3f>(i, j) = cv::Vec3f::all(weight);
		}
	}
	kernel.convertTo(kernel, CV_32FC3, 1./sum);
	return kernel;
}

/**
 * _getKernel ��ݍ��݂��s���J�[�l�����擾����
 *
 * point	cv::Point	�o�͉摜�̍��W
 * return	cv::Mat		���͉摜�ɑ΂���d�݃}�b�v
 *
 **/
cv::Mat improc::_getKernel(cv::Point point)
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
		rect.width	= kernel_size + (center.x - k_size_harf);
	} else if (center.x + k_size_harf >= src_size.width) {
		rect.x		= center.x - k_size_harf;
		rect.width	= src_size.width - rect.x;
	} else {
		rect.x		= center.x - k_size_harf;
		rect.width	= kernel_size;
	}

	if (center.y - k_size_harf < 0) {
		rect.y		= 0;
		rect.height	= kernel_size + (center.y - k_size_harf);
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
 **/
cv::Point2f improc::_dstPointToSrc(cv::Point point)
{
	double x = point.x / scale_x + 0.5 / scale_x - 0.5;
	double y = point.y / scale_y + 0.5 / scale_y - 0.5;

	return cv::Point2f(x, y);
}

/**
 * _distanceToWeight �������d�݌W���ɕϊ�����
 *
 * dist_x		double	x���̋���
 * dist_y		double	y���̋���
 * interpolate	int		�t�B���^�̎��
 * return		double	�d�݌W��
 *
 **/
double improc::_distanceToWeight(double dist_x, double dist_y, int interpolate)
{
	if (interpolate != NULL) {
		inter = interpolate;
	}
	
	double weight = 0., dxy;
	switch (inter) {
	case cv::INTER_CUBIC:
		if (dist_x >= 2.) {
			return 0.;
		} else if (dist_x > 1.) {
			weight = 4. - 8. * dist_x + 5. * dist_x * dist_x - dist_x * dist_x * dist_x;
		} else {
			weight = 1. - 2 * dist_x * dist_x + dist_x * dist_x * dist_x;
		}
		if (dist_y >= 2.) {
			return 0.;
		} else if (dist_y > 1.) {
			return weight * (4. - 8. * dist_y + 5. * dist_y * dist_y - dist_y * dist_y * dist_y);
		} else {
			return weight * (1. - 2 * dist_y * dist_y + dist_y * dist_y * dist_y);
		}

	case cv::INTER_LINEAR:
		dxy = sqrt(dist_x * dist_x + dist_y * dist_y);
		if (dxy < 1.) {
			return  (1. - dxy);
		} else {
			return 0.;
		}
	default:
		return 0;
	}
}
