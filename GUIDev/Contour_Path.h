#pragma once
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d.hpp>


enum color_space {
	RED,
	GREEN,
	YELLOW,
};

struct Map_Color {
	std::vector<ImVec2> points;
	color_space color_region;
};

namespace vis {
	using namespace cv;
	void vectorize_reds(Mat hsv, std::vector<Map_Color> &vec_points)
	{
		Mat1b mask1, mask2;
		inRange(hsv, Scalar(0, 20, 20), Scalar(10, 255, 255), mask1);
		inRange(hsv, Scalar(170, 59, 79), Scalar(180, 255, 255), mask2);
		Mat1b mask = mask1 | mask2;
		Mat canny_mask;
		std::vector<std::vector<Point>> canny_contour;
		Canny(mask, canny_mask, 0, 255);
		findContours(mask, canny_contour, RETR_TREE, CHAIN_APPROX_SIMPLE);
		for (auto& vector_points : canny_contour)
		{
			std::vector<ImVec2> points;
			for (int i = 0; i < vector_points.size(); i++)
			{
				points.push_back(ImVec2(vector_points[i].x, vector_points[i].y));
			}
			if (points.size() > 0)
				vec_points.push_back({ points,RED });
		}
	}
	void vectorize_greens(Mat hsv, std::vector<Map_Color>& vec_points)
	{
		Mat1b mask1;
		inRange(hsv, Scalar(40, 80, 57), Scalar(149, 255, 255), mask1);
		Mat1b mask = mask1;
		Mat canny_mask;
		std::vector<std::vector<Point>> canny_contour;
		Canny(mask, canny_mask, 0, 255);
		findContours(mask, canny_contour, RETR_TREE, CHAIN_APPROX_SIMPLE);
		for (auto& vector_points : canny_contour)
		{
			std::vector<ImVec2> points;
			for (int i = 0; i < vector_points.size(); i++)
			{
				points.push_back(ImVec2(vector_points[i].x, vector_points[i].y));
			}
			if (points.size() > 0)
				vec_points.push_back({ points,GREEN });
		}
	}
	void vectorize_yellow(Mat hsv, std::vector<Map_Color>& vec_points)
	{
		Mat1b mask1;
		inRange(hsv, Scalar(22.5, 75, 93), Scalar(33, 255, 255), mask1);
		Mat1b mask = mask1;
		Mat canny_mask;
		std::vector<std::vector<Point>> canny_contour;
		Canny(mask, canny_mask, 0, 255);
		findContours(mask, canny_contour, RETR_TREE, CHAIN_APPROX_SIMPLE);
		for (auto& vector_points : canny_contour)
		{
			std::vector<ImVec2> points;
			for (int i = 0; i < vector_points.size(); i++)
			{
				points.push_back(ImVec2(vector_points[i].x, vector_points[i].y));
			}
			if (points.size() > 0) {
				vec_points.push_back({ points,YELLOW });
			}
		}
	}
	void vectorize_image(std::vector<Map_Color>& vec_points,cv::Mat& stitched)
	{
		vec_points.clear();
		Mat hsv;
		cvtColor(stitched, hsv, COLOR_BGR2HSV);
		//dilate(hsv, hsv, Mat(), Point(-1, -1));
		/*imshow("mapgaussian", hsv);
		waitKey();*/
		vectorize_greens(hsv, vec_points);
		vectorize_yellow(hsv, vec_points);
		vectorize_reds(hsv, vec_points);


	}
}
