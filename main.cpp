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


int main()
{
	cv::Mat a = cv::Mat::eye(3, 3, CV_8U);


	return 0;
}

void resample(const cv::Mat src, cv::Mat &dst)
{
	improc imp;
	imp.resizeImage(src, dst);
}
