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
QImage imageprocessor::cvMatToQimage(const cv::Mat &src)    {   return QImage(src.data, src.cols, src.rows, src.step, _image.format()).copy();                                          }
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
    _processed_image = imageprocessor::cvMatToQimage(dest);
}

//Applique Sobel à l'image de gauche.
void imageprocessor::sobel()
{
    cv::Mat dest;
    cv::Sobel(imageprocessor::qimageToCvMat(getImage()), dest, -1, 1, 0);
    cv::imshow("Aperçu de Sobel", dest);
    cv::waitKey();
    _processed_image = imageprocessor::cvMatToQimage(dest);
}

//Affiche la carte de disparité
void imageprocessor::disparity_map()
{
    left_image = qimageToCvMat(_image);
    right_image = qimageToCvMat(_image_alt);

    cv::Mat disp, disp8;

    cv::cvtColor(left_image, left_image, CV_BGR2GRAY);
    cv::cvtColor(right_image, right_image, CV_BGR2GRAY);

    cv::StereoSGBM sbm;


    int sadSize = 5;//in range 3-11
    sbm.SADWindowSize = sadSize;
    sbm.numberOfDisparities = 16*sadSize;
    sbm.preFilterCap = 10;
    sbm.minDisparity = 0;
    sbm.uniquenessRatio = 10;//in range 5-15
    sbm.speckleWindowSize = 100;//in range 50-200
    sbm.speckleRange = 2;
    sbm.disp12MaxDiff = 1;
    sbm.P1 = sadSize*sadSize*8*4;
    sbm.P2 = sadSize*sadSize*32*4;

    sbm(left_image, right_image, disp);
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
