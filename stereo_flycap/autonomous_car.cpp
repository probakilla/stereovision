#include "car_controler.h"
#include "stereo_camera.h"

#include "fake_controler.h"
#include "traj_controler.h"

#include <opencv2/highgui/highgui.hpp>

#include <chrono>
#include <functional>
#include <iostream>
#include <map>
#include <sstream>

#include <stdlib.h>
#include <unistd.h>

using namespace std::chrono;

std::map<std::string, std::function<Controler*()> > builders =
{
  {"fake", [](){return new FakeControler();}},
  {"traj", [](){return new TrajControler();}}
};

void usage(char ** argv)
{
  std::cerr << "Usage: " << argv[0] << " <analyzer_name> <nb_images> <frequency>" << std::endl;
  exit(EXIT_FAILURE);
}

void saveImage(const cv::Mat & img, const std::string & side, int no)
{
  std::ostringstream oss;
  oss << side << "_" << no << ".png";
  cv::imwrite(oss.str(), img);
}

int main(int argc, char ** argv)
{
  // Checking args
  if (argc < 4) {
    usage(argv);
  }

  // Reading config from args
  std::string analyzer_name = argv[1];
  int nb_images = atoi(argv[2]);
  float frequency = atof(argv[3]);

  Controler * controler;

  try {
    controler = builders.at(analyzer_name)();
  }
  catch (const std::out_of_range & exc) {
    std::cerr << "Cannot find '" << analyzer_name << "' among analyzers" << std::endl;
    exit(EXIT_FAILURE);
  }

  // Computing period
  float period_us = 1000 * 1000 / frequency;

  // Initialize car
  CarControler cc("/dev/ttyUSB0");
  std::cout << "Connexion with car established" << std::endl;

  // Initialize camera
  FlyCapture2::PGRGuid left_guid, right_guid;
  // Left camera GUID
  left_guid.value[0] = 2022556694;
  left_guid.value[1] = 506125062;
  left_guid.value[2] = 2627711216;
  left_guid.value[3] = 2463930288;
  // Right camera GUID
  right_guid.value[0] = 932990310;
  right_guid.value[1] = 323408622;
  right_guid.value[2] = 1096954096;
  right_guid.value[3] = 2825689008;
  StereoCamera sc(left_guid, right_guid);
  sc.startCapture();
  std::cout << "Connexion with cameras established" << std::endl;

  // Initializing command
  cc.setSpeed(0,0,0);
  sleep(1);

  for (int image_id = 0; image_id < nb_images; image_id++) {
    time_point<steady_clock> tick_start, tick_end;
    tick_start = steady_clock::now();

    // Retrieving images from camera
    cv::Mat left_img, right_img;
    sc.getImages(&left_img, &right_img);

    // Analyzing images and sending command to car controller
    float vx, vy, omega;
    controler->process(left_img, right_img, &vx, &vy, &omega);
    cc.setSpeed(vx, vy, omega);

    saveImage(left_img, "left", image_id);
    saveImage(right_img, "right", image_id);

    // Computing elapsed time
    tick_end = steady_clock::now();
    double elapsed_ticks = (tick_end - tick_start).count();
    double elapsed_secs = elapsed_ticks * steady_clock::period::num / steady_clock::period::den;
    int elapsed_us = (int)(elapsed_secs * 1000*1000);

    // Debug
    std::cout << "Image " << (image_id+1) << "/" << nb_images
              << ": processed in " << elapsed_secs << "[s]" << std::endl;

    // Computing sleeping time
    int sleeping_us = period_us - elapsed_us;

    // Sleeping
    if (sleeping_us > 0) {
      std::cout << "\tSleeping for " << sleeping_us << " microseconds" << std::endl;
      usleep(sleeping_us);
    }
  }

  // Stop
  cc.setSpeed(0,0,0);
}
