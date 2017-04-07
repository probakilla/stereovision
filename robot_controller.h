#ifndef ROBOT_CONTROLLER_H
#define ROBOT_CONTROLLER_H

#pragma once

#include "controler.h"

class FakeControler : public Controler
{
public:
  virtual void process(const cv::Mat & left_img, const cv::Mat & right_img, float * vx, float * vy, float * omega) override;
};

#endif // ROBOT_CONTROLLER_H
