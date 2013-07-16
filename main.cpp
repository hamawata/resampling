#include <iostream>
#include "C:\OpenCV2.2\include\opencv2\opencv.hpp"
#include "improc.h"

#ifdef _DEBUG
    #pragma comment(lib,"C:\\OpenCV2.2\\lib\\opencv_core220d.lib")            // opencv_core
    #pragma comment(lib,"C:\\OpenCV2.2\\lib\\opencv_imgproc220d.lib")        // opencv_imgproc
    #pragma comment(lib,"C:\\OpenCV2.2\\lib\\opencv_highgui220d.lib")        // opencv_highgui
    #pragma comment(lib,"C:\\OpenCV2.2\\lib\\opencv_objdetect220d.lib")    // opencv_objdetect
#else
    #pragma comment(lib,"C:\\OpenCV2.2\\lib\\opencv_core220.lib")            // opencv_core
    #pragma comment(lib,"C:\\OpenCV2.2\\lib\\opencv_imgproc220.lib")        // opencv_imgproc
    #pragma comment(lib,"C:\\OpenCV2.2\\lib\\opencv_highgui220.lib")        // opencv_highgui
    #pragma comment(lib,"C:\\OpenCV2.2\\lib\\opencv_objdetect220.lib")    // opencv_objdetect
#endif

void resample(const cv::Mat src, cv::Mat &dst)
{
	improc *imp;
	imp = new improc(cv::INTER_CUBIC);
	imp->resizeImage(src, dst);
	delete imp;
}

int main()
{
	cv::Mat src;
	cv::imread("pic2.jpg").convertTo(src, CV_32FC3);
	cv::Mat dst = cv::Mat::zeros(src.rows * 1.4, src.cols * 1.4, src.type());

	resample(src, dst);
	dst.convertTo(dst, CV_8UC3);
	cv::imwrite("dst.jpg", dst);

	return 0;
}