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
void imageprocessor::imageResize(const int & width,const int & height)  { _image = _image.scaled(width, height, Qt::KeepAspectRatio);                                                   }
QImage imageprocessor::getImage() const                     {   return _image;              }
void imageprocessor::setImage(const QImage & image)         {   _image = image.copy();      }
bool imageprocessor::getIsCroped() const                    {   return _is_croped;          }
QImage imageprocessor::getProcessedImage() const            {   return _processed_image;    }

void imageprocessor::splitImage(QPixmap *left, QPixmap *right)
{
    /*_image_alt = _image.copy(_image.width() / 2, 0, _image.width() / 2, _image.height());
    _image = _image.copy(0, 0, _image.width() / 2, _image.height());*/
    _image = left->toImage();
    _image_alt = right->toImage();

}

void imageprocessor::blur()
{
    cv::Mat dest;
    //The strength of the blur is determined by the cv::Size(w, h)
    cv::blur(imageprocessor::qimageToCvMat(getImage()), dest, cv::Size(5, 5));
    cv::imshow("Aperçu du flou", dest);
    cv::waitKey();
    _processed_image = imageprocessor::cvMatToQimage(dest);
}

void imageprocessor::canny()
{
    cv::Mat dest;
    cv::Canny(imageprocessor::qimageToCvMat(getImage()), dest, 100, 200);
    cv::imshow("Aperçu de Canny", dest);
    cv::waitKey();
    _processed_image = imageprocessor::cvMatToQimage(dest);
}

void imageprocessor::sobel()
{
    cv::Mat dest;
    cv::Sobel(imageprocessor::qimageToCvMat(getImage()), dest, -1, 1, 0);
    cv::imshow("Aperçu de Sobel", dest);
    cv::waitKey();
    _processed_image = imageprocessor::cvMatToQimage(dest);
}

void imageprocessor::disparity_map()
{
    cv::Mat left_image = qimageToCvMat(_image);
    cv::Mat right_image = qimageToCvMat(_image_alt);
    cv::Mat disp, disp8;

    cv::cvtColor(left_image, left_image, CV_BGR2GRAY);
    cv::cvtColor(right_image, right_image, CV_BGR2GRAY);

    cv::StereoSGBM sbm;

    int sadSize = 3;
    sbm.SADWindowSize = sadSize;
    sbm.numberOfDisparities = 144;//144; 128
    sbm.preFilterCap = 10; //63
    sbm.minDisparity = 0; //-39; 0
    sbm.uniquenessRatio = 10;
    sbm.speckleWindowSize = 100;
    sbm.speckleRange = 32;
    sbm.disp12MaxDiff = 1;
    sbm.fullDP = true;
    sbm.P1 = sadSize*sadSize*4;
    sbm.P2 = sadSize*sadSize*32;

    sbm(left_image, right_image, disp);
    normalize(disp, disp8, 0, 255, CV_MINMAX, CV_8U);

    cv::Mat dispSGBMscale;
    disp.convertTo(dispSGBMscale,CV_32F, 1./16);

    imshow("image", disp8);
}

void imageprocessor::featureDetection()
{
    cv::Mat left_image = qimageToCvMat(_image);
    cv::Mat right_image = qimageToCvMat(_image_alt);

    int minHessian = 400;
    cv::SurfFeatureDetector surf(minHessian);

    surf.detect(left_image, keypoints_left);
    surf.detect(right_image, keypoints_right);

    cv::Mat img_keypoints_left, img_keypoints_right;

    cv::drawKeypoints(left_image, keypoints_left, img_keypoints_left);
    cv::drawKeypoints(right_image, keypoints_right, img_keypoints_right);

    cv::imshow("Keypoints Left", img_keypoints_left);
    cv::waitKey();
    cv::imshow("Keypoints Right", img_keypoints_right);
    cv::waitKey();
}

void imageprocessor::featureMatching()
{
    cv::Mat left_image = qimageToCvMat(_image);
    cv::Mat right_image = qimageToCvMat(_image_alt);

    cv::SurfDescriptorExtractor extractor;
    cv::Mat descriptor_left, descriptor_right;
    extractor.compute(left_image, keypoints_left, descriptor_left);
    extractor.compute(right_image, keypoints_right, descriptor_right);

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
        if(matches[i].distance <= cv::max(2*min_dist, 0.02))
            correct_matches.push_back(matches[i]);
    }

    cv::Mat img_matches;
    cv::drawMatches(left_image, keypoints_left, right_image, keypoints_right, correct_matches, img_matches);

    imshow("Matches", img_matches);
    cv::waitKey();
}
