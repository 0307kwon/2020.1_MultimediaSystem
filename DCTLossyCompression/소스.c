#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>	
#include <iostream>
#include <stdio.h>


using namespace cv;
using namespace std;

#define PI 3.1415926535897

double DCTbasis(int u, int v, int i, int j) {
	double cu, cv, out;
	if (u == 0) {
		cu = sqrtf(2) / 2.0;
	}
	else {
		cu = 1.0;
	}
	if (v == 0) {
		cv = sqrtf(2) / 2.0;
	}
	else {
		cv = 1.0;
	}
	out = cu * cv / 4 * cos((2 * i + 1)*u*PI / 16)*cos((2 * j + 1)*v*PI / 16);
	return out;
}


Mat* DCTcompression(Mat& image) {
	// �Է� : ���� ���� 8�� ����� �̹���, ��� : ���� �Ϸ�� �̹���

	if (image.cols % 8 == 0 && image.rows % 8 == 0) { //���μ��ΰ� 8�� ����� ����
		Mat* out = new Mat(image.size(), CV_64FC1, Scalar(0));
		uchar* image_ptr;
		double* out_ptr;

		double* temp_ptr = out->ptr<double>(1);

		cout << image.cols << ", " << image.rows << endl;

		std::cout << "DCT ����" << std::endl;;
		//x,y�� 8*8 compression ������ 
		for (int y = 0; y < image.rows; y += 8) {
			for (int x = 0; x < image.cols; x += 8) {

				for (int v = y; v < y + 8; v++) { // 8*8 DCT
					out_ptr = out->ptr<double>(v);
					for (int u = x; u < x + 8; u++) {

						double temp = 0;
						for (int j = y; j < y + 8; j++) { // F(u,v) �ϳ� ����
							image_ptr = image.ptr<uchar>(j);
							for (int i = x; i < x + 8; i++) {
								temp += DCTbasis(u - x, v - y, i - x, j - y)*image_ptr[i]; // DCTbasis�� ���ڰ��� 0~7������ ���̹Ƿ� �׿� �°� �־���
							}
						}

						out_ptr[u] = temp;

					}
				}
			}
		}
		out_ptr = out->ptr<double>(1);
		cout << out_ptr[0] << "�̰�" << endl;
		return out;
	}
	else {
		cout << "8�� ����� ũ�⸦ ���� �̹����� �����մϴ�." << endl;
		return 0;
	}
}

Mat* IDCTcompression(Mat& image, int compression_ratio) { // compression_ratio : ����� n ( n*n�� �״�� �ΰ� ������ 0 )
	if (image.cols % 8 == 0 && image.rows % 8 == 0) { //���μ��ΰ� 8�� ����� ����
		cout << image.cols << "*" << image.rows << endl;
		Mat* out = new Mat(image.size(), CV_8UC1, Scalar(0));
		double* image_ptr;
		uchar* out_ptr;

		std::cout << "IDCT ���� ����" << std::endl;
		for (int y = 0; y < image.rows; y += 8) {
			for (int x = 0; x < image.cols; x += 8) {

				for (int j = y; j < y + 8; j++) {
					out_ptr = out->ptr<uchar>(j);
					for (int i = x; i < x + 8; i++) { // 8*8 IDCT

						double temp = 0;
						for (int v = y; v < y + compression_ratio; v++) {
							image_ptr = image.ptr<double>(v);
							for (int u = x; u < x + compression_ratio; u++) {


								temp += DCTbasis(u - x, v - y, i - x, j - y)*image_ptr[u];
							}
						}

						out_ptr[i] = floor(temp + 0.5); // �������� �����ϱ����� �ݿø�
					}
				}
			}
		}
		return out;
	}
	else {
		cout << "8�� ����� ũ�⸦ ���� �̹����� �����մϴ�." << endl;
		return 0;
	}
}

void print_basisft(int u, int v) {

	Mat* image = new Mat(8, 8, CV_64FC1, Scalar(0));

	double* image_ptr;

	for (int j = 0; j < 8; j++) {
		image_ptr = image->ptr<double>(j);
		for (int i = 0; i < 8; i++) {
			image_ptr[i] = DCTbasis(u, v, i, j);
		}
	}
	cout << "u = " << u << " v = " << v << endl;
	cout << *image << endl << endl;
}


int main(int, char**)
{

	Mat image = imread("lena.jpg", IMREAD_COLOR);

	Mat gray_img;
	cvtColor(image, gray_img, COLOR_BGR2GRAY); // grayscale�� �ٲ���
	imshow("1", gray_img);


	Mat& DCT_image = *DCTcompression(gray_img);



	Mat& IDCT_image_original = *IDCTcompression(DCT_image, 8);
	Mat& IDCT_image_4x4 = *IDCTcompression(DCT_image, 4);
	Mat& IDCT_image_2x2 = *IDCTcompression(DCT_image, 2);

	imshow("8x8", IDCT_image_original);
	imshow("4x4", IDCT_image_4x4);
	imshow("2x2", IDCT_image_2x2);

	imwrite("8x8.png", IDCT_image_original);
	imwrite("4x4.png", IDCT_image_4x4);
	imwrite("2x2.png", IDCT_image_2x2);


	waitKey();

}