
#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

using namespace std;
// comparison function object

bool compareContourAreas ( std::vector<cv::Point> contour1, std::vector<cv::Point> contour2 ) {
    double i = fabs( cv::contourArea(cv::Mat(contour1)) );
    double j = fabs( cv::contourArea(cv::Mat(contour2)) );
    return ( i < j );
}

int main() {

  cv::Mat matrix = cv::imread("ark.jpg", 1);
  cv::Mat resized;          
  cv::Mat gray;
  cv::Mat blurred;
  cv::Mat canny;
  std::vector<std::vector<cv::Point> > contours;
  std::vector<cv::Point> screenCnt;

  double rescalefactor = 300/matrix.size().width;
  cv::resize(matrix, resized, cv::Size(600, 600), 0, 0);
  cv::cvtColor(resized, gray, cv::COLOR_BGR2GRAY);
  cv::GaussianBlur(gray, blurred, cv::Size(5, 5), 0);
  cv::Canny(blurred, canny, 75, 200);
  cv::findContours(canny, contours, cv::RetrievalModes::RETR_LIST, cv::ContourApproximationModes::CHAIN_APPROX_SIMPLE);
  std::sort(contours.begin(), contours.end(), compareContourAreas);
  for(size_t i=0; i<contours.size(); i++) {
       // use contours[i] for the current contour
       double arclen = cv::arcLength(contours[i], true);
       cv::approxPolyDP(contours[i], screenCnt, 0.02*arclen, true);   
       if (screenCnt.size()==4) {
           break;
       }
  }
  std::vector<std::vector<cv::Point> > draw_contours;
  draw_contours.push_back(screenCnt);
  cv::drawContours(resized, draw_contours, -1, cv::Scalar(0, 255, 0));
  cv::imshow("Image", resized);
  cv::waitKey(10000);
}
