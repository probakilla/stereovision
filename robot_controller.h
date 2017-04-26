#pragma once

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>

class robot_controller
{
public:
  /// Use the provided left and right images to update the desired speed
  /// - vx: speed forward [m/s]
  /// - vy: lateral speed [m/s] (+ is left)
  /// - omega: angular speed [rad/s] (+ is rotating to left)

  virtual void process(const cv::Mat & left_img, const cv::Mat & right_img, float * vx, float * vy, float * omega);
  robot_controller ();

  cv::Mat disparity(const cv::Mat & left_image, const cv::Mat & right_image);
  cv::Mat diff(const cv::Mat & image1, const cv::Mat & image2);
  cv::Mat depthMap(cv::Mat disp);

  void test();

private:
    /// Return the distance from the object.
    /// todo
    float calc_dist (cv::Mat const & left_image, cv::Mat const & right_image);
	std::vector<cv::Mat> getChessImages();


    float _last_dist;
    float _last_speed;

    const float INITIAL_SPEED = 4.0;
    const float DT = 0.5;

    const float FOCAL_LENGTH = 0.0035;
    const float PIXEL_SIZE = 0.00000375;
    const float CAMERA_SPACING = 0.1;
    const float MAX_DISTANCE = 20;

    cv::Mat lastDisp;
};


