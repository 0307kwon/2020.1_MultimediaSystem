
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>	
#include <iostream>
#include <stdio.h>


using namespace cv;
using namespace std;


void make_edd(Mat* output) {

	int y;
	int x;
	double e; // 오차
	double** map; // 이미지맵

	map = (double**)malloc(sizeof(double)*output->rows);


	for (y = 0; y < output->rows; y++)
	{
		map[y] = (double*)malloc(sizeof(double) * output->cols);
	}
	for (y = 0; y < output->rows; y++) {
		for (x = 0; x < output->cols; x++) {
			map[y][x] = output->at<uchar>(y, x);
		}
	}


	//과정 1 : 오차확산 (좌우, 아래 1픽셀씩은 계산하지않음, x=1, y=0 부터 시작)
	//반드시 for 문은 y부터 시작해야 순서대로 오차확산됨
	for (y = 0; y < output->rows - 1; y++) {
		for (x = 1; x < output->cols - 1; x++) {
			if (map[y][x] > 127) {
				e = map[y][x] - 255;
			}
			else {
				e = map[y][x];
			}

			map[y][x + 1] = map[y][x + 1] + e * 7 / 16.0;
			map[y + 1][x + 1] = map[y + 1][x + 1] + e * 1 / 16.0;
			map[y + 1][x] += e * 5 / 16.0;
			map[y + 1][x - 1] += e * 3 / 16.0;
		}
	}

	//과정 2 : 1bit로 변환
	for (y = 0; y < output->rows; y++) {
		for (x = 0; x < output->cols; x++) {
			if (map[y][x] > 127) {
				output->at<uchar>(y, x) = 255;
			}
			else {
				output->at<uchar>(y, x) = 0;
			}
		}
	}

	free(map);
}

int main(int, char**)
{

	Mat image = imread("lena.jpg", IMREAD_COLOR);
	Mat image2 = imread("sejin.jpg", IMREAD_COLOR);


	//grayscale 오차확산디더링 - 이미지 1
	Mat gray_img;
	cvtColor(image, gray_img, COLOR_BGR2GRAY); // grayscale로 바꿔줌
	namedWindow("1", WINDOW_AUTOSIZE);
	imshow("1", gray_img);
	make_edd(&gray_img);
	namedWindow("2", WINDOW_AUTOSIZE);
	imshow("2", gray_img);
	imwrite("dithered lena.png", gray_img);


	//grayscale 오차확산디더링 - 이미지 2
	Mat gray_img2;
	cvtColor(image2, gray_img2, COLOR_BGR2GRAY); // grayscale로 바꿔줌
	namedWindow("3", WINDOW_AUTOSIZE);
	imshow("3", gray_img2);
	make_edd(&gray_img2);
	namedWindow("4", WINDOW_AUTOSIZE);
	imshow("4", gray_img2);
	imwrite("dithered sejin.png", gray_img2);



	//RGB color 디더링 - 이미지 1 

	Mat bgr_img[3];
	split(image, bgr_img);
	make_edd(&bgr_img[0]); // blue
	make_edd(&bgr_img[1]); // green
	make_edd(&bgr_img[2]); // red
	namedWindow("Blue");
	imshow("Blue", bgr_img[0]);
	namedWindow("Green");
	imshow("Green", bgr_img[1]);
	namedWindow("Red");
	imshow("Red", bgr_img[2]);

	Mat merged_img;
	merge(bgr_img, 3, merged_img);
	namedWindow("merged");
	imshow("merged", merged_img);

	imwrite("dithered RGB1.png", merged_img);

	//RGB color 디더링 - 이미지 2

	Mat bgr_img2[3];
	split(image2, bgr_img2);
	make_edd(&bgr_img2[0]); // blue
	make_edd(&bgr_img2[1]); // green
	make_edd(&bgr_img2[2]); // red
	namedWindow("Blue2");
	imshow("Blue2", bgr_img2[0]);
	namedWindow("Green2");
	imshow("Green2", bgr_img2[1]);
	namedWindow("Red2");
	imshow("Red2", bgr_img2[2]);

	Mat merged_img2;
	merge(bgr_img2, 3, merged_img2);
	namedWindow("merged2");
	imshow("merged2", merged_img2);

	imwrite("dithered RGB2.png", merged_img2);


	waitKey();
	return 0;
}