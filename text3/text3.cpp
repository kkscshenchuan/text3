// text3.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "opencv.hpp"
#include "iostream"
#include "vector"
using namespace std;
using namespace cv;


int main()
{
	Mat SrcImage, GrayImage, DstImage;
	SrcImage = imread("text.png");
	/*namedWindow("原图",2);
	imshow("原图", SrcImage);*/

	cvtColor(SrcImage, GrayImage, COLOR_RGB2GRAY);
	threshold(GrayImage, DstImage, 100, 255, CV_THRESH_BINARY);
	namedWindow("二值化", 2);
	imshow("二值化", DstImage);

	int height = DstImage.rows;
	int width = DstImage.cols;
	vector<vector<int>> state(height,vector<int>(width,0));  //建立一个动态数组，标记这个点是否为交叉点
	 
	uchar *data = DstImage.data;                     //获取图片首地址

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			int num = 0;     //统计点的八邻域内黑色像素的个数 大于等于3个则为交叉点
			if (data[i*DstImage.step + j] == 0 && i > 0 && j > 0 && i < height - 1 && j < width - 1)
			{
				int p1 = data[(i - 1)*DstImage.step + (j - 1)];
				int p2 = data[(i - 1)*DstImage.step + j];
				int p3 = data[(i - 1)*DstImage.step + (j + 1)];
				int p4 = data[i*DstImage.step + (j - 1)];
				int p6 = data[i*DstImage.step + (j + 1)];
				int p7 = data[(i + 1)*DstImage.step + (j - 1)];
				int p8 = data[(i + 1)*DstImage.step + j];
				int p9 = data[(i + 1)*DstImage.step + (j + 1)];

				if (p1 == 0 && p3 == 0 && p7 == 0 && p2 != 0 && p4 != 0)  //1,3,7
				{
					state[i][j] = 1;
				}
				else if (p1 == 0 && p3 == 0 && p8 == 0 && p2 != 0)      //1 3 8 
				{
					state[i][j] = 1;
				}
				else if (p1 == 0 && p3 == 0 && p9 == 0 && p2 != 0 && p6 != 0)      //1 3 9
				{
					state[i][j] = 1;
				}
				else if (p1 == 0 && p6 == 0 && p7 == 0 && p4 != 0)      //1 6 7
				{
					state[i][j] = 1;
				}
				else if (p1 == 0 && p6 == 0 && p8 == 0)      //1 6 8
				{
					state[i][j] = 1;
				}
				else if (p1 == 0 && p7 == 0 && p9 == 0 && p4 != 0 && p8 != 0)      //1 7 9
				{
					state[i][j] = 1;
				}
				else if (p2 == 0 && p4 == 0 && p6 == 0)      //2 4 6
				{
					state[i][j] = 1;
				}
				else if (p2 == 0 && p4 == 0 && p8 == 0)      //2 4 8
				{
					state[i][j] = 1;
				}
				else if (p2 == 0 && p4 == 0 && p9 == 0)      //2 4 9
				{
					state[i][j] = 1;
				}
				else if (p2 == 0 && p6 == 0 && p7 == 0)      //2 6 7
				{
					state[i][j] = 1;
				}
				else if (p2 == 0 && p6 == 0 && p8 == 0)      //2 6 8
				{
					state[i][j] = 1;
				}
				else if (p2 == 0 && p7 == 0 && p9 == 0 && p8 != 0)      //2 7 9
				{
					state[i][j] = 1;
				}
				else if (p3 == 0 && p4 == 0 && p8 == 0)      //3 4 8
				{
					state[i][j] = 1;
				}
				else if (p3 == 0 && p4 == 0 && p9 == 0 && p6 != 0)      //3 4 9
				{
					state[i][j] = 1;
				}
				else if (p3 == 0 && p7 == 0 && p9 == 0 && p6 != 0 && p8 != 0)      //3 7 9
				{
					state[i][j] = 1;
				}
				else if (p4 == 0 && p6 == 0 && p8 == 0)      //4 6 8
				{
					state[i][j] = 1;
				}

			}
		}
	}
	
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			uchar *p = DstImage.ptr<uchar>(i,j);
			if (state[i][j] == 1)
			{
				p[0] = 0;
			}
			else
			{
				p[0] = 255;
			}
		}
	}


	namedWindow("处理图", 2);
	imshow("处理图", DstImage);

	waitKey(0);
    return 0;
}

