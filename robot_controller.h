#ifndef ROBOT_CONTROLLER_H
#define ROBOT_CONTROLLER_H

#pragma once

#include "opencv2/core/core.hpp"

class robot_controller
{
public:
  /// Use the provided left and right images to update the desired speed
  /// - vx: speed forward [m/s]
  /// - vy: lateral speed [m/s] (+ is left)
  /// - omega: angular speed [rad/s] (+ is rotating to left)

  virtual void process(const cv::Mat & left_img, const cv::Mat & right_img, float * vx, float * vy, float * omega) = 0;
  robot_controller ();

private:
    /// Return the distance from the object.
    /// todo
    float calc_dist (cv::Mat const & left_image, cv::Mat const & right_image);
	std::vector<cv::Mat> getChessImages();


    float _last_dist;
    float _last_speed;

    const float INITIAL_SPEED = 4.0;
    const float DT = 0.5;
};

#endif // ROBOT_CONTROLLER_H


