// test.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <opencv2/opencv.hpp>
#include "opencv2/core/core.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "opencv2/imgcodecs.hpp"
#include <iostream>
#include <Windows.h>
using namespace cv;
using namespace std;

int main()
{
	/*
	Mat img;
	VideoCapture cap(0);
	while (true)
	{
		cap >> img;
		Mat edges;
		cvtColor(img, edges, COLOR_BGR2GRAY);
		Canny(edges, edges, 30, 60);
		imshow("webcam.io", edges);
		waitKey(1);
	}
	*/
	struct objData
	{
		int objid[64];
		int topy[64];
		int leftx[64];
		int bottomy[64];
		int rightx[64];
		int size[64];
	};


	Mat img = imread("C:\\test\\9b8dfes-960.jpg", IMREAD_GRAYSCALE);
	Mat edges;
	for (int j = 0; j < img.rows; j++)
	{
		for (int i = 0; i < img.cols; i++)
		{
			if (img.at<uchar>(j,i) <=230 ) {
				img.at<uchar>(j,i) = 0;
			}
		}
	}
	Canny(img, edges, 45, 135);
	imshow("window label", edges);
	imwrite("C:\\test\\pic1.jpg", edges);
	int objCount=0;
	int edgesMet=0;
	objData data1{};
	for (int j = 1; j < edges.rows; j++)
	{
		for (int i = 1; i < edges.cols; i++)
		{
			edgesMet = 0;
// parsing till top is met			
			if (edges.at<uchar>(j, i) > 230) {
				cout << "working"<<objCount << endl;
				data1.topy[objCount] = j;
				edgesMet++;
// parsing till bottom is met
				for (int k = j+2; k < edges.rows; k++) {
					if (edges.at<uchar>(k, i) > 230) {
						data1.bottomy[objCount] = k;
						edgesMet++;
						break;
					}
					else if (edges.at<uchar>(k, i) > 230) break;
				}
// parse middleY to find right edge
				int middleY = (int)(data1.bottomy[objCount] + data1.topy[objCount]) / 2;
				for (int k = i+2; k < edges.cols; k++) {
					if (edges.at<uchar>(middleY, k) > 230) {
						data1.rightx[objCount] = k;
						edgesMet++;
						break;
					}
					else if (edges.at<uchar>(middleY, k) > 230) break;
				}
// parse middleY to find left edge
				for (int k = i-2; k > 0; k--) {
					if (edges.at<uchar>(middleY, k) > 230) {
						data1.leftx[objCount] = k;
						edgesMet++;
						break;
					}
					else if (edges.at<uchar>(middleY, k) > 230) break;
				}
// draw new edges
				if (edgesMet == 4) {
					for (int y = data1.topy[objCount]; y > data1.bottomy[objCount]; y--) {
						edges.at<uchar>(y, data1.leftx[objCount]) = 230;
						edges.at<uchar>(y, data1.rightx[objCount]) = 230;
					}
					for (int x = data1.leftx[objCount]; x < data1.rightx[objCount]; x++) {
						edges.at<uchar>(data1.topy[objCount], x) = 230;
						edges.at<uchar>(data1.bottomy[objCount], x) = 230;
					}
					data1.size[objCount] = (data1.topy[objCount] - data1.bottomy[objCount]) * (data1.leftx[objCount] - data1.rightx[objCount]);
					objCount++;
				}

			}

		}
	}
	imshow("window label111", edges);
	imwrite("C:\\test\\pic2.jpg", edges);
	cout << "objects found : " << objCount << endl;
	for (int i = 0; i < objCount; i++) {
		cout << "id " << i << "; ";
		cout << "ty " << data1.topy[i] << "; ";
		cout << "by " << data1.bottomy[i] << "; ";
		cout << "lx " << data1.leftx[i] << "; ";
		cout << "rx " << data1.rightx[i] << "; ";
		cout << "size " << data1.size[i] << endl;
	}
	return 0;
}