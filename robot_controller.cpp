#include "robot_controller.h"

robot_controller::robot_controller () : _last_dist (-1.0) {}

void robot_controller::test()
{

    std::string right_im = "julpilleux/img/damier_10cm/right_10.png";
    std::string left_im = "julpilleux/img//damier_10cm//left_10.png";
    std::string right_im2 = "julpilleux/img/damier_10cm/right_11.png";
    std::string left_im2 = "julpilleux/img/damier_10cm/left_11.png";

    cv::Mat right = cv::imread(right_im, cv::IMREAD_COLOR);
    if ( right.empty() ){
        //std::cout<<"Cannot read image file: "<< right_im;
        return ;
    }
    cv::Mat left = cv::imread(left_im, cv::IMREAD_COLOR);
    if ( left.empty() ){
        //std::cout<<"Cannot read image file: "<< left_im;
        return ;
    }

    cv::Mat right2 = cv::imread(right_im2, cv::IMREAD_COLOR);
    if ( right2.empty() ){
        //std::cout<<"Cannot read image file: "<< right_im2;
        return ;
    }
    cv::Mat left2 = cv::imread(left_im2, cv::IMREAD_COLOR);
    if ( left2.empty() ){
        //std::cout<<"Cannot read image file: "<< left_im2;
        return ;
    }

    auto disp = disparity( left, right );
    auto disp2 = disparity( left2, right2 );
    auto diff_map = diff( disp, disp2);
    auto depth_map = depthMap(disp);
    calc_dist(depth_map,  diff_map);

    cv::imshow("base", right);
    cv::imshow("base2", right2);
    cv::imshow("disp", disp);
    cv::imshow("disp2", disp2);
    cv::imshow("diff", diff_map);
    cv::imshow("depth", depth_map);

    cv::waitKey();
}

float robot_controller::calc_dist (const cv::Mat & depth_map, const cv::Mat & diff_map)
{
    using namespace std;
    vector< vector<int> > xy_diff;
    //On parcours la carte des différence entre les matrices de disparité (de la caméra gauche et droites) pour voir si quelque chose a bougé
    for (int i = 0; i < diff_map.rows; ++i)
    {
        for (int j = 0; j < diff_map.cols; ++j)
           {
                //Cela signifie que l'élément à bougé donc on récupère ses coordonnées
               if ((int)diff_map.at<uchar>(i, j) == 255)
               {
                    vector<int> row;
                    row.push_back(i);
                    row.push_back(j);
                    xy_diff.push_back(row);
               }

            }
    }
    int size = xy_diff.size();
    long int nb_elem = 0;
    double add = 0.0;
    for (int i = 0; i < size; ++i)
    {
        add += depth_map.at<double>(cv::Point(xy_diff[i][0], xy_diff[i][1]));;
        ++nb_elem;
    }
    double average = add/nb_elem;
    cout << "distance moyenne " << average << endl;
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


     cv::GaussianBlur(left_image_cpy, left_image_cpy, cv::Size(21,21),5);
     cv::GaussianBlur(right_image_cpy, right_image_cpy, cv::Size(21,21),5);

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

    return disp8;
}

cv::Mat robot_controller::depthMap( cv::Mat disp )
{
    cv::Mat depth_map( disp.size(), CV_32F, 0.2f);

    for(int y=0;y<depth_map.rows;y++)
    {
        for(int x=0;x<depth_map.cols;x++)
        {
            float distance = 1;
            float disparity = disp.at<uchar>(cv::Point(x,y));
            if( disparity != 0 )
                distance = (FOCAL_LENGTH * CAMERA_SPACING) / ( 2 * PIXEL_SIZE * disparity);

            std::cout << distance << " " ;

            depth_map.at<float>(cv::Point(x,y)) = distance;
        }
    }
    return depth_map;
}

cv::Mat robot_controller::diff(const cv::Mat & image1, const cv::Mat & image2)
{
    using namespace cv;

    Mat_<uchar> motion;
    absdiff(image1, image2, motion);
    motion = motion > 125;
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
