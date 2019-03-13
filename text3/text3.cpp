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
	Mat SrcImage, GrayImage, DstImage,CopyImage;
	SrcImage = imread("text.png");
	/*namedWindow("原图",2);
	imshow("原图", SrcImage);*/

	cvtColor(SrcImage, GrayImage, COLOR_RGB2GRAY);
	threshold(GrayImage, DstImage, 100, 255, CV_THRESH_BINARY);
	namedWindow("二值化", 2);
	imshow("二值化", DstImage);

	int height = DstImage.rows;
	int width = DstImage.cols;
	vector<vector<int>> state(height,vector<int>(width,0));       //建立一个动态数组，标记这个点是否为交叉点
	vector<vector<int>> search(height, vector<int>(width, 0));    //建立一个动态数组，标记这个点是否被搜索过
	vector<vector<int>> quantity(height, vector<int>(width, 0));  //统计交点周围有几个前景点
	int size = 20;                       //阈值
	vector<int> Xstorage(size, 0);       //存储x坐标
	vector<int> YStorage(size, 0);       //存储y坐标

	CopyImage = DstImage.clone();
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
	
	//显示交点图
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			uchar *p = CopyImage.ptr<uchar>(i,j);
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

	namedWindow("交点图", 2);
	imshow("交点图", CopyImage);

	//统计交点周围前景点数量
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			uchar *p = DstImage.ptr<uchar>(i, j);
			int num = 0;
			if (state[i][j] == 1)
			{
				for (int m = i - 1; m <= i + 1; m++)
				{
					for (int n = j - 1; n <= j + 1; n++)
					{
						if (p[0] == 0)
						{
							num++;
						}
					}
				}
				quantity[i][j] = num;
				cout << quantity[i][j] << endl;
			}
		}
	}



	//for (int i = 0; i < height; i++)
	//{
	//	for (int j = 0; j < width; j++)
	//	{
	//		uchar *p = DstImage.ptr<uchar>(i, j);
	//		uchar *data = DstImage.data;
	//		int m=0, n=0;
	//		int num = 0;
	//		bool exist = true;             //判断线段上是否还有点
	//		bool control = true;

	//		if (state[i][j] == 1 && i > 0 && j > 0 && i < height - 1 && j < width - 1)   //找到交点
	//		{
	//			search[i][j] = 1;
	//			m = i;
	//			n = j;
	//			while (exist == true)
	//			{
	//				for (int x = m - 1; x <= m + 1; x++)
	//				{
	//					if(control==false) break;
	//					for (int y = n - 1; y <= n + 1; y++)
	//					{
	//						if (data[x*DstImage.step + y] == 0 && search[x][y] == 0)
	//						{
	//							num++;
	//							m = x;
	//							n = y;
	//							search[x][y] = 1;
	//							control = false;
	//							break;
	//						}
	//					}
	//				}

	//				if (num == 0)            
	//				{
	//					exist = false;
	//				}
	//			} 

	//		}
	//	}
	//}


	waitKey(0);
    return 0;
}

