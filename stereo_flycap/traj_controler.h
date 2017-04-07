#pragma once

#include "controler.h"

#include <vector>

class TrajControler : public Controler
{
public:
  TrajControler();

  virtual void process(const cv::Mat & left_img,
                       const cv::Mat & right_img,
                       float * vx, float * vy, float * omega) override;
private:
  /// Current index
  int idx;

  /// Number of points at a given position
  int period;

  // Trajectory is piecewise constant speed
  static std::vector<float> traj_vx;
  static std::vector<float> traj_vy;
  static std::vector<float> traj_omega;
};
