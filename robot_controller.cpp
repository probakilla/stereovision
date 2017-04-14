#include "robot_controller.h"

robot_controller::robot_controller () : _last_dist (-1.0) {}

float robot_controller::calc_dist (const cv::Mat & left_image, const cv::Mat & right_image)
{

    // Todo.
    return 0;
}

cv::Mat robot_controller::disparity(const cv::Mat & left_image, const cv::Mat & right_image)
{
    float downSize = 0.8f;

    cv::Mat disp, disp8;
    cv::Mat left_image_cpy, right_image_cpy;

    cv::cvtColor(left_image, left_image_cpy, CV_BGR2GRAY);
    cv::cvtColor(right_image, right_image_cpy, CV_BGR2GRAY);

    cv::resize( left_image_cpy, left_image_cpy, cv::Size(), downSize,downSize);
    cv::resize( right_image_cpy, right_image_cpy, cv::Size(), downSize,downSize);


    // cv::GaussianBlur(left_image_undistord, left_image_undistord, cv::Size(21,21),5);
    // cv::GaussianBlur(right_image_undistord, right_image_undistord, cv::Size(21,21),5);

    //On entre les paramètres pour la carte de disparité
    cv::StereoSGBM sgbm;
    int sadSize = 5;
    sgbm.SADWindowSize = 5;
    sgbm.numberOfDisparities = 192;
    sgbm.preFilterCap = 4;
    sgbm.minDisparity = -52;
    sgbm.uniquenessRatio = 5;
    sgbm.speckleWindowSize = 150;
    sgbm.speckleRange = 2;
    sgbm.disp12MaxDiff = 10;
    sgbm.fullDP = false;
    sgbm.P1 = sadSize*sadSize*8*3;
    sgbm.P2 = sadSize*sadSize*32*3;

    sgbm(left_image_cpy, right_image_cpy, disp);
    normalize(disp, disp8, 0, 255, CV_MINMAX, CV_8U);

    /*int dilation_size = 2;

    //erosion
    cv::erode(disp8, disp8, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(2*dilation_size + 1,2*dilation_size + 1)));

    //Dilatation

    int dilation_type = cv::MORPH_RECT; // MORPH_CROSS MORPH_ELLIPSE

    cv::Mat element = cv::getStructuringElement( dilation_type,
                                         cv::Size( 2*dilation_size + 1, 2*dilation_size+1 ),
                                         cv::Point( dilation_size, dilation_size ) );
    dilate( disp8, disp8, element );*/

    return disp8;
}

cv::Mat robot_controller::diff(const cv::Mat & image1, const cv::Mat & image2)
{
    cv::Mat motion;
    cv::absdiff(image1, image2, motion);
    cv::threshold(motion, motion, 80, 255, cv::THRESH_BINARY);
    cv::erode(motion, motion, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5,5)));

    return motion;
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
