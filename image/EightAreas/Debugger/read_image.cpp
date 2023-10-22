#include <opencv2/opencv.hpp>
#include <vector>
#include <time.h>
 
using namespace cv;
using namespace std;
 
// 8邻域
const Point neighbors[8] = { { 0, 1 }, { 1, 1 }, { 1, 0 }, { 1, -1 }, 
                             { 0, -1 }, { -1, -1 }, { -1, 0 }, {-1, 1} };
 
int main()
{
	// 生成随机数  
	RNG rng(time(0));
 
	Mat src = imread("d:/downloadapp/VsOpencv/123(1).png");
	Mat gray;
	cvtColor(src, gray, CV_BGR2GRAY);
 
	Mat edges;
	Canny(gray, edges, 30, 100);
 
	vector<Point> seeds;
	vector<Point> contour;
	vector<vector<Point>> contours;
 
	int i, j, k;
	for (i = 0; i < edges.rows; i++)
		for (j = 0; j < edges.cols; j++)
		{
			Point c_pt = Point(i, j);
 
			//如果当前点为轮廓点
			if (edges.at<uchar>(c_pt.x, c_pt.y) == 255)
			{
 
				contour.clear();
				// 当前点清零
				edges.at<uchar>(c_pt.x, c_pt.y) = 0;
 
				// 存入种子点及轮廓
				seeds.push_back(c_pt);
				contour.push_back(c_pt);
 
				// 区域生长
				while (seeds.size() > 0)
				{
					// 遍历8邻域
					for (k = 0; k < 8; k++)
					{	
						// 更新当前点坐标
						c_pt.x = seeds[0].x + neighbors[k].x;
						c_pt.y = seeds[0].y + neighbors[k].y;
 
						// 边界界定
						if ((c_pt.x >= 0)  && (c_pt.x <= edges.rows - 1) &&
							(c_pt.y >= 0) && (c_pt.y <= edges.cols - 1))
						{
							if (edges.at<uchar>(c_pt.x, c_pt.y) == 255)
							{
								// 当前点清零
								edges.at<uchar>(c_pt.x, c_pt.y) = 0;
 
								// 存入种子点及轮廓
								seeds.push_back(c_pt);
								contour.push_back(c_pt);
							}// end if		
						}										
					} // end for
 
					// 删除第一个元素
					seeds.erase(seeds.begin());
 
				}// end while
 
				contours.push_back(contour);
 
			}// end if
		}
 
 
	// 显示一下  
	Mat trace_edge = Mat::zeros(edges.rows, edges.cols, CV_8UC1);
	Mat trace_edge_color;
	cvtColor(trace_edge, trace_edge_color, CV_GRAY2BGR);
	for (i = 0; i < contours.size(); i++)
	{
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
 
		//cout << edges[i].size() << endl;  
		// 过滤掉较小的边缘  
		if (contours[i].size() > 5)
		{
			for (j = 0; j < contours[i].size(); j++)
			{
				trace_edge_color.at<Vec3b>(contours[i][j].x, contours[i][j].y)[0] = color[0];
				trace_edge_color.at<Vec3b>(contours[i][j].x, contours[i][j].y)[1] = color[1];
				trace_edge_color.at<Vec3b>(contours[i][j].x, contours[i][j].y)[2] = color[2];
			}
		}
 
	}
 
 
 
	imshow("edge", trace_edge_color);
 
 
	waitKey();
 
	return 0;
 
 
 
 
}