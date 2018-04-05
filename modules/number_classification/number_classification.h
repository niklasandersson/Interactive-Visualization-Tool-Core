#ifndef NUMBERCLASSIFICATION_H
#define NUMBERCLASSIFICATION_H

#include "scene/main/node.h"
#include "image.h"
#include "variant.h"

#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>

class NumberClassification : public Node {
    GDCLASS(NumberClassification, Node);

public:
    NumberClassification();
    /* hej */
    /* hej */
    int Classify(const Ref<Image> &p_image);

protected:
    static void _bind_methods();


};

#endif
