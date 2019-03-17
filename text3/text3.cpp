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
	SrcImage = imread("1.png");
	/*namedWindow("原图",2);
	imshow("原图", SrcImage);*/

	cvtColor(SrcImage, GrayImage, COLOR_RGB2GRAY);
	threshold(GrayImage, DstImage, 100, 255, CV_THRESH_BINARY);
	/*namedWindow("二值化", 2);
	imshow("二值化", DstImage);*/

	int height = DstImage.rows;
	int width = DstImage.cols;
	vector<vector<int>> state(height,vector<int>(width,0));       //建立一个动态数组，标记这个点是否为交叉点
	vector<vector<int>> search(height, vector<int>(width, 0));    //建立一个动态数组，标记这个点是否被搜索过
	vector<vector<int>> quantity(height, vector<int>(width, 0));  //统计交点周围有几个前景点
	vector<vector<int>> nerghbor(height, vector<int>(width, 0));  //标记是否为交点八邻域内的点
	vector<vector<int>> remove(height, vector<int>(width, 0));    //标记点是否应该被消除
	int size = 10;                       //阈值
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
			uchar *q = CopyImage.ptr<uchar>(i,j);
			if (state[i][j] == 1)
			{
				q[0] = 0;
			}
			else
			{
				q[0] = 255;
			}
		}
	}

	namedWindow("交点图", 2);
	imshow("交点图", CopyImage);

	namedWindow("原图", 2);
	imshow("原图", DstImage);
	
	//显示图像像素值
	int number = 0;
	//for (int i = 0; i < height; i++)
	//{
	//	for (int j = 0; j < width; j++)
	//	{
	//		number++;
	//		int ok = data[i*DstImage.step + j];
	//		cout << ok << " ";
	//		if (number % 20 == 0)
	//		{
	//			cout << endl;
	//		}
	//	}
	//}


	//统计交点周围前景点数量
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			int num = 0;
			if (state[i][j] == 1)
			{
				for (int m = i - 1; m <= i + 1; m++)
				{
					for (int n = j - 1; n <= j + 1; n++)
					{
						uchar *p = DstImage.ptr<uchar>(m, n);
						if (p[0] == 0)
						{
							nerghbor[m][n] = 1;
							num++;
						}
						
					}
				}
				nerghbor[i][j] = 0;
				quantity[i][j] = num-1;
				//cout << quantity[i][j] << endl;            //输出交点八邻域内前景点的数量
			}
		}
	}


	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			int m=0, n=0;                  //中间变量
			int num;                       //统计点的数量
			bool exist = true;             //判断线段上是否还有点
			bool control = true;

			if (state[i][j] == 1 && i > 0 && j > 0 && i < height - 1 && j < width - 1)   //找到交点
			{
				search[i][j] = 1;
				for (int number = 0; number < quantity[i][j]; number++)             //根据交点周围的前景点数量，判断交点应该搜索多少次
				{
					num = 0;
					m = i;
					n = j;
					do
					{
						control = true;
						for (int x = m - 1; x <= m + 1; x++)
						{
							if (control == false) break;
							for (int y = n - 1; y <= n + 1; y++)
							{
								
								int mm = data[x*DstImage.step + y];
								cout << mm << " " << endl;
								if (mm == 0 && search[x][y] == 0 && x > 0 && y > 0 && x < height - 1 && n < width - 1)  //点为前景点，且不越界，没有被搜索过
								{
									cout << "ok" << endl;
									num++;
									if (nerghbor[x][y] == 1)
									{
										search[x][y] = 1;
									}
									Xstorage.push_back(x);   //将x的值存入Xstorage数组
									YStorage.push_back(y);   //将y的值存入Xstorage数组
									m = x;
									n = y;
									control = false;
									break;
								}
								//else
								//{
								//	exist = false;          //当搜索不到八邻域内的前景点之后，结束while循环
								//	control = false;
								//	break;
								//}
							}
						}
						if (num >= size)
						{
							exist = false;                  //当点得数量超过阈值时，跳出循环
						}
					} while (exist == true);
					cout << num << " " << endl;
				

					if (num < size)    //当数量小于阈值，则消除
					{
						for (int k = 0; k < Xstorage.size(); k++)
						{
							remove[Xstorage[k]][YStorage[k]] = 1;    //将需要消除的标记为1
						}
					}
					Xstorage.clear();
					YStorage.clear();
				}

				for (int i = 0; i < height; i++)   //将交点周围八邻域点的状态重置
				{
					for (int j = 0; j < width; j++)
					{
						search[i][j] = 0;
					}
				}

			}
		}
	}

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			number++;
			uchar *p = DstImage.ptr<uchar>(i, j);
			if (remove[i][j] == 1)
			{
				p[0] = 255;
			}
			//cout << remove[i][j] <<" ";
			//cout << nerghbor[i][j] << " ";
			if (number % 20 == 0)
			{
				cout << endl;
			}
		}
	}


	namedWindow("结果图", 2);
	imshow("结果图", DstImage);
	waitKey(0);
    return 0;
}

