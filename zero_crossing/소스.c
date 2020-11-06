#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>	
#include <iostream>
#include <stdio.h>


using namespace cv;
using namespace std;


void makeLogZero(Mat& input, Size size, double sigma);
void FindZeroCrossings(Mat& laplacian, Mat& zero_crossings);

int main(int, char**)
{

	Mat image = imread("road.jpg");


	makeLogZero(image, Size(7, 7), 1.0);
	makeLogZero(image, Size(7, 7), 2.0);
	makeLogZero(image, Size(7, 7), 3.0);
	makeLogZero(image, Size(7, 7), 4.0);
	makeLogZero(image, Size(7, 7), 5.0);



	waitKey();
	return 0;
}

void makeLogZero(Mat& input, Size size, double sigma) {
	Mat input2;
	Mat gray_img, laplac_img, laplac_img2, zero_img, convert_img;

	GaussianBlur(input, input2, size, sigma, 0.0, BORDER_DEFAULT);
	cvtColor(input2, gray_img, COLOR_BGR2GRAY); // grayscale�� �ٲ���	

	Laplacian(gray_img, laplac_img, CV_32F, 3, 1, 0, BORDER_DEFAULT); // ������� 256�� �Ѱ�
																	  //zeroCrossing�Լ��� float ���¸� �����Ƿ� CV_32F�� ���

	FindZeroCrossings(laplac_img, zero_img); // ������� unsigned char ���·� ��µǹǷ� 0~255���� ���� ����

	imshow("sigma" + to_string(sigma), zero_img); // imshow�� 0~256 ������ ������ 0~1������ float�� ������, �׷��Ƿ� zero_img�� �״�� �Է�
	convertScaleAbs(laplac_img, laplac_img); // 8bit unsigned int�� �ٲ���
											 //imshow("laplac", laplac_img);

	imwrite("zero_crossing.png", zero_img); // �̹����� ���Ϸ� ����

}

void FindZeroCrossings(Mat& laplacian, Mat& zero_crossings)
{
	Mat* result = new Mat(laplacian.size(), CV_8U, Scalar(0));
	zero_crossings = *result;
	int image_rows = laplacian.rows;
	int image_channels = laplacian.channels();
	cout << image_rows << endl;
	int values_on_each_row = laplacian.cols * image_channels;
	float laplacian_threshold = 20.0; // ������ Ȧ�� 20�� ���
									  // Find Zero Crossings
	for (int row = 1; row < image_rows; row++) {
		float* prev_row_pixel = laplacian.ptr<float>(row - 1) + 1;
		float* curr_row_pixel = laplacian.ptr<float>(row);
		uchar* output_pixel = zero_crossings.ptr<uchar>(row) + 1;
		for (int column = 1; column < values_on_each_row; column++)
		{
			float prev_value_on_row = *curr_row_pixel;
			curr_row_pixel++;
			float curr_value = *curr_row_pixel;
			float prev_value_on_column = *prev_row_pixel;
			float difference = 0.0;
			if (((curr_value > 0) && (prev_value_on_row < 0)) ||
				((curr_value < 0) && (prev_value_on_row > 0)))
				difference = abs(curr_value - prev_value_on_row);
			if ((((curr_value > 0) && (prev_value_on_column < 0)) ||
				((curr_value < 0) && (prev_value_on_column > 0))) &&
				(abs(curr_value - prev_value_on_column) > difference))
				difference = abs(curr_value - prev_value_on_column);
			*output_pixel = (difference > laplacian_threshold) ? 255 : 0;// (int) ((100 * difference) / laplacian_threshold);
			prev_row_pixel++;
			output_pixel++;
		}
	}
}