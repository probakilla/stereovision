#include "imageprocessor.h"

#include <QImage>

imageprocessor::imageprocessor(const QImage & image) :
    _left_image{image},
    _right_image{image},
    _processed_image{image},
    _is_croped{false} {}


void imageprocessor::crop(const QRect & rect)
{

        _is_croped = true;
        QImage* tmp = new QImage ((rect.width() * 2), rect.height(), _left_image.format());

        int top_y = rect.topLeft().y();
        int top_x = rect.topLeft().x();

        // Copy the left rectangle of the image.
        for (int y = 0; y < rect.height(); ++y)
            for (int x = 0; x < rect.width(); ++x)
                tmp->setPixel(x, y, _left_image.pixel(x + top_x, y + top_y));

        int top_middle_x = (_left_image.width() / 2) + top_x;

        // Copy the right rectangle of the image.
        for (int y = 0; y < rect.height(); ++y)
            for (int x = 0; x < (tmp->width() / 2); ++x)
                tmp->setPixel(x + (tmp->width() / 2), y, _left_image.pixel(x + top_middle_x, y + top_y));

        _processed_image = tmp->copy();
}


cv::Mat imageprocessor::qimageToCvMat(const QImage & image)
{
    return cv::Mat(image.height(), image.width(),
                   CV_8UC4, const_cast<uchar*>(image.bits()),
                   image.bytesPerLine()).clone();
}

QImage imageprocessor::cvMatToQimage(const cv::Mat &src, bool canny)
{
    if (canny == false)
        return QImage((uchar*)src.data, src.cols, src.rows, src.step, _left_image.format()).copy();
    else
        return QImage((uchar*)src.data, src.cols, src.rows, src.step, QImage::Format_Indexed8).copy();
}

void imageprocessor::imageResize(const int & width,const int & height)
{
    _left_image = _left_image.scaled(width, height, Qt::KeepAspectRatio);
    _right_image = _right_image.scaled(width, height, Qt::KeepAspectRatio);
}
QImage imageprocessor::getLeftImage() const                 {   return _left_image;               }
QImage imageprocessor::getRightImage() const                {   return _right_image;              }
void imageprocessor::setImage(const QImage & image)         {   _left_image = image.copy();       }
void imageprocessor::setImageAlt(const QImage & imagealt)   {   _right_image = imagealt.copy();   }
bool imageprocessor::getIsCroped() const                    {   return _is_croped;                }
QImage imageprocessor::getProcessedImage() const            {   return _processed_image;          }

//Divise _image en deux et place la seconde moitié dans _image_alt
void imageprocessor::splitImage()
{
    _right_image = _left_image.copy(_left_image.width() / 2, 0, _left_image.width() / 2, _left_image.height());
    _left_image = _left_image.copy(0, 0, _left_image.width() / 2, _left_image.height());
}

//Floute l'image de gauche
void imageprocessor::blur()
{
    cv::Mat dest;
    //The strength of the blur is determined by the cv::Size(w, h)
    cv::blur(imageprocessor::qimageToCvMat(getLeftImage()), dest, cv::Size(BLUR_X_SIZE, BLUR_Y_SIZE));
    cv::imshow("Aperçu du flou", dest);
    cv::waitKey();
    _processed_image = imageprocessor::cvMatToQimage(dest);
}


void imageprocessor::test()
{
    //std::string left = "img/6cm/left_0.png";
    //cv::Mat leftMat = cv::imread(left ,CV_LOAD_IMAGE_COLOR);
    cv::Mat left_image = qimageToCvMat(_image);
    imshow("left", left_image);
}

//Applique Canny à l'image de gauche
void imageprocessor::canny()
{
    cv::Mat dest;
    cv::Canny(imageprocessor::qimageToCvMat(getLeftImage()), dest, THRESHOLD_1, THRESHOLD_2);
    cv::imshow("Aperçu de Canny", dest);
    cv::waitKey();
    _processed_image = imageprocessor::cvMatToQimage(dest, true);
}

//Applique Sobel à l'image de gauche.
void imageprocessor::sobel()
{
    cv::Mat dest;
    cv::Sobel(imageprocessor::qimageToCvMat(getLeftImage()), dest, -1, 1, 0);
    cv::imshow("Aperçu de Sobel", dest);
    cv::waitKey();
    _processed_image = imageprocessor::cvMatToQimage(dest);
    cv::Mat processed_image = qimageToCvMat(_processed_image);
    cv::imshow("_processed_image", processed_image);
}

//calibre les caméras à partir d'image de damier
void imageprocessor::calibrateCameras()
{
    std::vector< std::vector< cv::Point3f > > objects_points;
    std::vector< std::vector< cv::Point2f > > left_image_points, right_image_points;
    std::vector< cv::Point2f > corners1, corners2;

    cv::Size board_size = cv::Size(CALIBRATE_X_SIZE ,CALIBRATE_Y_SIZE);

    cv::Mat gray1, gray2;
    cv::Mat left_image = qimageToCvMat(_left_image);
    cv::Mat right_image = qimageToCvMat(_right_image);
    cvtColor(left_image, gray1, CV_BGR2GRAY);
    cvtColor(right_image, gray2, CV_BGR2GRAY);
    bool found1 = false, found2 = false;
    found1 = cv::findChessboardCorners(left_image, board_size, corners1,
				       CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);
    found2 = cv::findChessboardCorners(right_image, board_size, corners2,
				       CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);
    if(!found1 || !found2)
    {
        std::cout << "Je ne trouve pas de damier" << std::endl;
        return;
    }


    //on affine les coordonnées obtenues
    cv::cornerSubPix(gray1, corners1,
                     cv::Size(CORNER_WIN_X, CORNER_WIN_Y),
                     cv::Size(CORNER_ZERO_X, CORNER_ZERO_Y),
                     cv::TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, CALI_MAX_COUNT, CALI_EPSILON));
    //cv::drawChessboardCorners(gray1, board_size, corners1, found1);


    cv::cornerSubPix(gray2, corners2,
                     cv::Size(CORNER_WIN_X, CORNER_WIN_Y),
                     cv::Size(CORNER_ZERO_X, CORNER_ZERO_Y),
                     cv::TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, CALI_MAX_COUNT, CALI_EPSILON));
    //cv::drawChessboardCorners(gray2, board_size, corners2, found2);

    float square_size = 1.75;
    std::vector< cv::Point3f > obj;
    for (int i = 0; i < CALIBRATE_Y_SIZE; i++)
      for (int j = 0; j < CALIBRATE_X_SIZE; j++)
          obj.push_back(cv::Point3f((float)j * square_size, (float)i * square_size, 0));

    if (found1 && found2) {
      left_image_points.push_back(corners1);
      right_image_points.push_back(corners2);
      objects_points.push_back(obj);
    }
    cv::Mat left_camera_matrix, right_camera_matrix, left_dist_coeffs, right_dist_coeffs;
    cv::Size img_size = left_image.size();
    cv::Mat R, T, E, F;
    std::vector<cv::Mat>leftRvecs, leftTvecs, rightRvecs, rightTvecs;

    //on calbre les deux caméra séparément et ensuite avec les résultat obtenu on les calibre ensemble
    cv::calibrateCamera(objects_points, left_image_points, img_size, left_camera_matrix,
                        left_dist_coeffs, leftRvecs, leftTvecs);
    cv::calibrateCamera(objects_points, right_image_points, img_size, right_camera_matrix,
                        right_dist_coeffs, rightRvecs, rightTvecs);
    std::cout << "icule" << std::endl;
    cv::stereoCalibrate(objects_points, left_image_points, right_image_points, left_camera_matrix,
                        left_dist_coeffs, right_camera_matrix, right_dist_coeffs, img_size, R, T, E, F);

    cv::Mat R1, R2, P1, P2, Q;
    cv::stereoRectify(left_camera_matrix, left_dist_coeffs, right_camera_matrix, right_dist_coeffs, img_size,
                      R, T, R1, R2, P1, P2, Q, 0);
    std::cout << "Calibration effectué" << std::endl;

    //On enregistre les informations obtenu par la calibration qui nous seront néscessaire
    cv::FileStorage fs("Calibration.yml", cv::FileStorage::WRITE);
    fs << "leftCameraMatrix" << left_camera_matrix;
    fs << "rightCameraMatrix" << right_camera_matrix;
    fs << "leftDistCoeffs" << left_dist_coeffs;
    fs << "rightDistCoeffs" << right_dist_coeffs;
    fs.release();
}

//Affiche la carte de disparité
void imageprocessor::disparityMap()
{
    //on ouvre le fichier de calibration et on y récupère les données nescessaire pour enlever la distortion des images
    cv::FileStorage fs("Calibration.yml", cv::FileStorage::READ);
    cv::Mat left_camera_matrix, right_camera_matrix, left_dist_coeffs, right_dist_coeffs;
    fs["leftCameraMatrix"] >> left_camera_matrix;
    fs["rightCameraMatrix"] >> right_camera_matrix;
    fs["leftDistCoeffs"] >> left_dist_coeffs;
    fs["rightDistCoeffs"] >> right_dist_coeffs;


    cv::Mat left_image = qimageToCvMat(_left_image);
    cv::Mat right_image = qimageToCvMat(_right_image);
    cv::Mat left_image_undistord(left_image.size(), left_image.type());
    cv::Mat right_image_undistord(right_image.size(), right_image.type());
    cv::undistort(left_image, left_image_undistord, left_camera_matrix, left_dist_coeffs);
    cv::undistort(right_image, right_image_undistord, right_camera_matrix, right_dist_coeffs);
    cv::imshow("image droite", right_image_undistord);
    fs.release();//on ferme le fichier

    // Note a propos de stereo BM : il semble inverser le blanc et le noir.
    // A voir si ça joue dans la précision de la carte de disparité.
    cv::Mat disp, disp8;

    cv::resize( left_image_undistord, left_image_undistord, cv::Size(), 0.5,0.5);
    cv::resize( right_image_undistord, right_image_undistord, cv::Size(), 0.5,0.5);

   // cv::GaussianBlur(left_image_undistord, left_image_undistord, cv::Size(21,21),5);
   // cv::GaussianBlur(right_image_undistord, right_image_undistord, cv::Size(21,21),5);

    cv::cvtColor(left_image_undistord, left_image_undistord, CV_BGR2GRAY);
    cv::cvtColor(right_image_undistord, right_image_undistord, CV_BGR2GRAY);

    //On entre les paramètres pour la carte de disparité
    cv::StereoSGBM sgbm;

    sgbm.SADWindowSize = SAD_SIZE;
    sgbm.numberOfDisparities = NB_DISPARITIES;
    sgbm.preFilterCap = PRE_FILTER_CAP;
    sgbm.minDisparity = MIN_DISP;
    sgbm.uniquenessRatio = UNI_RATIO;
    sgbm.speckleWindowSize = SPECKLE_WIN_SIZE;
    sgbm.speckleRange = SPECKLE_RANGE;
    sgbm.disp12MaxDiff = MAX_DIFF;
    sgbm.fullDP = false;
    sgbm.P1 = DISP_SMOOTHNESS_1;
    sgbm.P2 = DISP_SMOOTHNESS_2;

    sgbm(left_image_undistord, right_image_undistord, disp);
    normalize(disp, disp8, RANGE_INF, RANGE_SUP, CV_MINMAX, CV_8U);
    imshow("image", disp8);
}


//Détecte les features des images chargées.
void imageprocessor::featureDetection()
{
    cv::Mat left_image = qimageToCvMat(_left_image);
    cv::Mat right_image = qimageToCvMat(_right_image);
    cv::cvtColor(left_image, left_image, CV_BGR2GRAY);
    cv::cvtColor(right_image, right_image, CV_BGR2GRAY);

    int minHessian = MIN_HESSIAN;
    cv::SurfFeatureDetector surf(minHessian);

    surf.detect(left_image, left_keypoints);
    surf.detect(right_image, right_keypoints);
}

//Affiche les "features"/key points
void imageprocessor::showKeyPoints()
{
    featureDetection();

    cv::Mat img_keypoints_left, img_keypoints_right;
    cv::Mat left_image = qimageToCvMat(_left_image);
    cv::Mat right_image = qimageToCvMat(_right_image);
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
    cv::Mat left_image = qimageToCvMat(_left_image);
    cv::Mat right_image = qimageToCvMat(_right_image);
    extractor.compute(left_image, left_keypoints, descriptor_left);
    extractor.compute(right_image, right_keypoints, descriptor_right);

    cv::FlannBasedMatcher matcher;
    std::vector<cv::DMatch> matches;
    matcher.match(descriptor_left, descriptor_right, matches);

    double max_dist = MAX_DIST;
    double min_dist = MIN_DIST;

    for(int i = 0; i < descriptor_left.rows; i++)
    {
        double dist = matches[i].distance;
        if(dist < min_dist)
            min_dist = dist;
        if(dist > max_dist)
            max_dist = dist;
    }

    std::vector<cv::DMatch> correct_matches;
    for(int i = 0; i < descriptor_left.rows; ++i)
    {
        if(matches[i].distance <= cv::max(2*min_dist, 0.10))
            correct_matches.push_back(matches[i]);
    }

    cv::Mat img_matches;
    cv::drawMatches(left_image, left_keypoints, right_image, right_keypoints, correct_matches, img_matches);

    imshow("Matches", img_matches);
    cv::waitKey();
}
