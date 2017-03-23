#include "imageprocessor.h"

#include <QImage>

imageprocessor::imageprocessor(const QImage & image) : _image{image}, _image_alt{image}, _processed_image{image}, _is_croped{false} {}


void imageprocessor::crop(const QRect & rect)
{

        _is_croped = true;
        QImage* tmp = new QImage ((rect.width() * 2), rect.height(), _image.format());

        int top_y = rect.topLeft().y();
        int top_x = rect.topLeft().x();

        // Copy the left rectangle of the image.
        for (int y = 0; y < rect.height(); ++y)
            for (int x = 0; x < rect.width(); ++x)
                tmp->setPixel(x, y, _image.pixel(x + top_x, y + top_y));

        int top_middle_x = (_image.width() / 2) + top_x;

        // Copy the right rectangle of the image.
        for (int y = 0; y < rect.height(); ++y)
            for (int x = 0; x < (tmp->width() / 2); ++x)
                tmp->setPixel(x + (tmp->width() / 2), y, _image.pixel(x + top_middle_x, y + top_y));

        _processed_image = tmp->copy();
}


cv::Mat imageprocessor::qimageToCvMat(const QImage & image) {   return cv::Mat(image.height(), image.width(), CV_8UC4, const_cast<uchar*>(image.bits()), image.bytesPerLine()).clone(); }

QImage imageprocessor::cvMatToQimage(const cv::Mat &src, bool canny)
{
    if (canny == false)
        return QImage((uchar*)src.data, src.cols, src.rows, src.step, _image.format()).copy();
    else
        return QImage((uchar*)src.data, src.cols, src.rows, src.step, QImage::Format_Indexed8).copy();
}

void imageprocessor::imageResize(const int & width,const int & height)  { _image = _image.scaled(width, height, Qt::KeepAspectRatio); _image_alt = _image_alt.scaled(width, height, Qt::KeepAspectRatio);                                                   }
QImage imageprocessor::getImage() const                     {   return _image;               }
QImage imageprocessor::getImageAlt() const                  {   return _image_alt;           }
void imageprocessor::setImage(const QImage & image)         {   _image = image.copy();       }
void imageprocessor::setImageAlt(const QImage & imagealt)   {   _image_alt = imagealt.copy();}
bool imageprocessor::getIsCroped() const                    {   return _is_croped;           }
QImage imageprocessor::getProcessedImage() const            {   return _processed_image;     }

//Divise _image en deux et place la seconde moitié dans _image_alt
void imageprocessor::splitImage()
{
    _image_alt = _image.copy(_image.width() / 2, 0, _image.width() / 2, _image.height());
    _image = _image.copy(0, 0, _image.width() / 2, _image.height());
}

//Floute l'image de gauche
void imageprocessor::blur()
{
    cv::Mat dest;
    //The strength of the blur is determined by the cv::Size(w, h)
    cv::blur(imageprocessor::qimageToCvMat(getImage()), dest, cv::Size(5, 5));
    cv::imshow("Aperçu du flou", dest);
    cv::waitKey();
    _processed_image = imageprocessor::cvMatToQimage(dest);
}

//Applique Canny à l'image de gauche
void imageprocessor::canny()
{
    cv::Mat dest;
    cv::Canny(imageprocessor::qimageToCvMat(getImage()), dest, 100, 200);
    cv::imshow("Aperçu de Canny", dest);
    cv::waitKey();
    _processed_image = imageprocessor::cvMatToQimage(dest, true);
}

//Applique Sobel à l'image de gauche.
void imageprocessor::sobel()
{
    cv::Mat dest;
    cv::Sobel(imageprocessor::qimageToCvMat(getImage()), dest, -1, 1, 0);
    cv::imshow("Aperçu de Sobel", dest);
    cv::waitKey();
    _processed_image = imageprocessor::cvMatToQimage(dest);
    cv::Mat processed_image = qimageToCvMat(_processed_image);
    cv::imshow("_processed_image", processed_image);
}

void imageprocessor::calibrate_cameras()
{
    std::vector< std::vector< cv::Point3f > > objects_points;
    std::vector< std::vector< cv::Point2f > > left_image_points, right_image_points;
    std::vector< cv::Point2f > corners1, corners2;

    cv::Size board_size = cv::Size(13, 9);

    cv::Mat gray1, gray2;
    cvtColor(left_image, gray1, CV_BGR2GRAY);
    cvtColor(right_image, gray2, CV_BGR2GRAY);
    bool found1 = false, found2 = false;

    found1 = cv::findChessboardCorners(left_image, board_size, corners1,
				       CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);
    found2 = cv::findChessboardCorners(right_image, board_size, corners2,
				       CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);

    if (found1)
      {
	cv::cornerSubPix(gray1, corners1, cv::Size(5, 5), cv::Size(-1, -1),
			 cv::TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 30, 0.1));
	cv::drawChessboardCorners(gray1, board_size, corners1, found1);
      }
    if (found2)
      {
	cv::cornerSubPix(gray2, corners2, cv::Size(5, 5), cv::Size(-1, -1),
			 cv::TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 30, 0.1));
	cv::drawChessboardCorners(gray2, board_size, corners2, found2);
      }
    float square_size = 5;
    std::vector< cv::Point3f > obj;
    for (int i = 0; i < 13; i++)
      for (int j = 0; j < 9; j++)
	obj.push_back(cv::Point3f((float)j * square_size, (float)i * square_size, 0));

    if (found1 && found2) {
      //cout << i << ". Found corners!" << endl;
      left_image_points.push_back(corners1);
      right_image_points.push_back(corners2);
      objects_points.push_back(obj);
    }
    cv::Mat left_camera_matrix, right_camera_matrix, left_dist_coeffs, right_dist_coeffs;
    cv::Size img_size = left_image.size();
    cv::Mat R, T, E, F;
    std::vector<cv::Mat>leftRvecs, leftTvecs, rightRvecs, rightTvecs;

    cv::calibrateCamera(objects_points, left_image_points, img_size, left_camera_matrix, left_dist_coeffs, leftRvecs, leftTvecs);
    cv::calibrateCamera(objects_points, right_image_points, img_size, right_camera_matrix, right_dist_coeffs, rightRvecs, rightTvecs);

    cv::stereoCalibrate(objects_points, left_image_points, right_image_points, left_camera_matrix, left_dist_coeffs, right_camera_matrix, right_dist_coeffs, img_size, R, T, E, F, cv::TermCriteria(cv::TermCriteria::COUNT+cv::TermCriteria::EPS,30,0.000001),cv::CALIB_FIX_INTRINSIC);
    cv::Mat R1, R2, P1, P2, Q;
    cv::stereoRectify(left_camera_matrix, left_dist_coeffs, right_camera_matrix, right_dist_coeffs, img_size, R, T, R1, R2, P1, P2, Q, 0);
    cv::Mat left_map1, left_map2, right_map1, right_map2;
    cv::initUndistortRectifyMap(left_camera_matrix, left_dist_coeffs, R1, P1, img_size, CV_32FC1, left_map1, left_map2);
    cv::initUndistortRectifyMap(right_camera_matrix, right_dist_coeffs, R2, P2, img_size, CV_32FC1, right_map1, right_map2);
    cv::remap(left_image, left_image, left_map1, left_map2, cv::INTER_LINEAR);
    cv::remap(right_image, right_image, right_map1, right_map2, cv::INTER_LINEAR);
}



//Affiche la carte de disparité
void imageprocessor::disparity_map()
{
    //featureMatching();
    left_image = qimageToCvMat(_image);
    right_image = qimageToCvMat(_image_alt);
    // Note a propos de stereo BM : il semble inverser le blanc et le noir.
    // A voir si ça joue dans la précision de la carte de disparité.
    cv::Mat disp, disp8;

    cv::cvtColor(left_image, left_image, CV_BGR2GRAY);
    cv::cvtColor(right_image, right_image, CV_BGR2GRAY);


    cv::StereoSGBM sgbm;
    int sadSize = 5;
    sgbm.SADWindowSize = 5;
    sgbm.numberOfDisparities = 192;
    sgbm.preFilterCap = 4;
    sgbm.minDisparity = -64;
    sgbm.uniquenessRatio = 5;
    sgbm.speckleWindowSize = 150;
    sgbm.speckleRange = 2;
    sgbm.disp12MaxDiff = 10;
    sgbm.fullDP = false;
    sgbm.P1 = sadSize*sadSize*8*4;
    sgbm.P2 = sadSize*sadSize*32*4;

    sgbm(left_image, right_image, disp);
    normalize(disp, disp8, 0, 255, CV_MINMAX, CV_8U);
    imshow("image", disp8);
}

//Détecte les features des images chargées.
void imageprocessor::featureDetection()
{
    left_image = qimageToCvMat(_image);
    right_image = qimageToCvMat(_image_alt);
    cv::cvtColor(left_image, left_image, CV_BGR2GRAY);
    cv::cvtColor(right_image, right_image, CV_BGR2GRAY);

    int minHessian = 400;
    cv::SurfFeatureDetector surf(minHessian);

    surf.detect(left_image, left_keypoints);
    surf.detect(right_image, right_keypoints);
}

//Affiche les "features"/key points
void imageprocessor::showKeyPoints()
{
    featureDetection();

    cv::Mat img_keypoints_left, img_keypoints_right;

    cv::drawKeypoints(left_image, left_keypoints, img_keypoints_left);
    cv::drawKeypoints(right_image, right_keypoints, img_keypoints_right);

    imshow("Left Image", img_keypoints_left);
    imshow("Right Image", img_keypoints_right);
}

//Affiche les bonnes correspondances entre les features des deux images
void imageprocessor::featureMatching()
{
    featureDetection();

    cv::SurfDescriptorExtractor extractor;
    cv::Mat descriptor_left, descriptor_right;
    extractor.compute(left_image, left_keypoints, descriptor_left);
    extractor.compute(right_image, right_keypoints, descriptor_right);

    cv::FlannBasedMatcher matcher;
    std::vector<cv::DMatch> matches;
    matcher.match(descriptor_left, descriptor_right, matches);

    double max_dist = 0;
    double min_dist = 100;

    for(int i = 0; i < descriptor_left.rows; i++)
    {
        double dist = matches[i].distance;
        if(dist < min_dist)
            min_dist = dist;
        if(dist > max_dist)
            max_dist = dist;
    }

    std::vector<cv::DMatch> correct_matches;
    for(int i = 0; i < descriptor_left.rows; i++)
    {
        if(matches[i].distance <= cv::max(2*min_dist, 0.10))
            correct_matches.push_back(matches[i]);
    }

    cv::Mat img_matches;
    cv::drawMatches(left_image, left_keypoints, right_image, right_keypoints, correct_matches, img_matches);

    imshow("Matches", img_matches);
    cv::waitKey();

}
