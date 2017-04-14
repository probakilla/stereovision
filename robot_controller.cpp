#include "robot_controller.h"

robot_controller::robot_controller () : _last_dist (-1.0) {}

float robot_controller::calc_dist (const cv::Mat & left_image, const cv::Mat & right_image)
{
    // Todo.
    return 0;
}

void robot_controller::process(const cv::Mat & left_img, const cv::Mat & right_img, float * vx, float * vy, float * omega)
{
// Todo.
    float dist (calc_dist(left_img, right_img));     // Distance from the object.
    // Case of the first call.
    if (_last_dist == -1)
    {
        *vx = INITIAL_SPEED;
        _last_speed = INITIAL_SPEED;
        _last_dist = dist;
    }
    else
    {
        float delta_d (dist - _last_dist);

        float delta_v (delta_d / DT);
        *vx = _last_speed + delta_v;
        _last_speed = *vx;
        _last_dist = dist;
    }
}

std::vector<cv::Mat> robot_controller::getChessImages()
{

	std::vector<cv::Mat> chessImages;

	//Right
	for (unsigned i = 0; i < 20; ++i)
	{
		std::stringstream ss;
		ss << "img/damier_10cm/right_" << i << ".png";
        chessImages.push_back(cv::imread(ss.str()));
		assert(!chessImages.at(i).empty());
	}

	//Left
	for (unsigned i = 0; i < 0; ++i)
	{
		std::stringstream ss;
		ss << "img/damier_10cm/left_" << i << ".png";
        chessImages.push_back(cv::imread(ss.str()));
		assert(!chessImages.at(i).empty());
	}

	return chessImages;
}
