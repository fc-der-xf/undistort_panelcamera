#include <opencv2/opencv.hpp>
#include <string>

using namespace std;

int main() {
	string file_name = "frames_1.jpg";

	//畸变系数
	double k1 = 0.04205792769789696;
	double k2 = -0.02283179759979248;
	double p1 = -0.0015250984579324722;
	double p2 = 0.0004033915465697646;

	//内参
	int fisheye_scale = 8; //地平线中给出的参数就是这个
	double fx = 367.2713928222656;
	double fy = 353.8418273925781;
	double cx = 641.4365844726563;
	double cy = 382.6099853515625;

	cv::Mat gray_img = cv::imread(file_name, 0);
	int rows = gray_img.rows, cols = gray_img.cols;
	cv::Mat undistort_img = cv::Mat(rows, cols, CV_8UC1);
	
	for (int v = 0; v < rows; v++) {
		for (int u = 0; u < cols; u++) {
			double x = (u - cx) / fx, y = (v - cy) / fy;
			double r = sqrt(x * x + y * y);
			double x_distorted = x * (1 + k1 * r * r + k2 * r * r * r* r) + 2 * p1 * x * y + p2 * (r * r + 2 * x * x);
			double y_distorted = y * (1 + k1 * r * r + k2 * r * r * r * r) + 2 * p2 * x * y + p1 * (r * r + 2 * y * y);
			double u_distorted = fx * x_distorted + cx;
			double v_distorted = fy * y_distorted + cy;

			//最近邻插值
			if (u_distorted >= 0 && v_distorted >= 0 && u_distorted < cols && v_distorted < rows) {
				undistort_img.at<uchar>(v, u) = gray_img.at<uchar>((int)v_distorted, (int)u_distorted);
			}
			else {
				undistort_img.at<uchar>(v, u) = 0;
			}
		}
	}

	//不过看起来并不能直接用这种方式来做...
	cv::imshow("undistorted", undistort_img);

	cv::waitKey(0);
	return 0;
}