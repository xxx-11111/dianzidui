#include <opencv2/core/core.hpp>              
#include <opencv2/highgui/highgui.hpp>              
#include <opencv2/imgproc/imgproc.hpp>             
#include <iostream>            
using namespace std;
using namespace cv;
 
int main()
{
    //------------【1】读取源图像并检查图像是否读取成功------------    
    Mat srcImage = imread("d:/downloadapp/VsOpencv/123(1).png");
    if (!srcImage.data)
    {
        cout << "读取图片错误，请重新输入正确路径！\n";
        system("pause");
        return -1;
    }
    imshow("【源图像】", srcImage);
    //------------【2】灰度转换------------    
    Mat srcGray;
    cvtColor(srcImage, srcGray, cv::COLOR_RGB2GRAY);
    imshow("【灰度图】", srcGray);
    //------------【3】初始化相关变量---------------  
    Mat dstImage;        //初始化自适应阈值参数
    const int maxVal = 255;
    int blockSize = 3;    //取值3、5、7....等
    int constValue = 3;   //偏移调整
    int adaptiveMethod = 0;
    int thresholdType = 0;
    /*
        自适应阈值算法
        0:ADAPTIVE_THRESH_MEAN_C
        1:ADAPTIVE_THRESH_GAUSSIAN_C
        --------------------------------------
        阈值类型
        0:THRESH_BINARY
        1:THRESH_BINARY_INV
    */
    //---------------【4】图像自适应阈值操作-------------------------
    adaptiveThreshold(srcGray, dstImage, maxVal, adaptiveMethod, thresholdType, blockSize, constValue);
 
    imshow("【自适应阈值】", dstImage);
    waitKey(0);
    return 0;
}