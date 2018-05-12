#ifndef NUMBERCLASSIFICATION_H
#define NUMBERCLASSIFICATION_H

#include "scene/main/node.h"
#include "image.h"
#include "variant.h"

#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <armadillo>

#include <string>

class NumberClassification : public Node {
    GDCLASS(NumberClassification, Node);

public:
    NumberClassification();
    int Classify(const Ref<Image> &p_image, Rect2 rect);

protected:
    static void _bind_methods();

private:
    int ClassifyMean(arma::vec number);
    arma::Mat<double> mean_digits;

    arma::mat cv2arma(cv::Mat& cv_mat);


};

#endif
