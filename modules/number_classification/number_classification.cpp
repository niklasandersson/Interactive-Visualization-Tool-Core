#include "number_classification.h"

NumberClassification::NumberClassification() {

  if(!mean_digits.load("means.txt")) {
    print_line("Could read mean digits!");
  }
}

int NumberClassification::Classify(const Ref<Image> &p_image, Rect2 rect) {
  try {

  Ref<Image> img = p_image->duplicate();
  int width = img->get_width();
  int height = img->get_height();

  if (img->is_compressed()) {
    print_line("Is compressed");
    img->decompress();
  }

  if (img->detect_alpha()) {
    print_line("Has alpha");
  }
  img->convert(Image::FORMAT_RGBA8);
  img->flip_y();
  PoolVector<uint8_t> data = img->get_data();
  PoolVector<uint8_t>::Read r = data.read();
  cv::Mat image = cv::Mat(height, width, CV_8UC4, const_cast<unsigned char*>(r.ptr())).clone();
  cv::Rect roi(rect.position.x, rect.position.y,
               rect.size.width, rect.size.height);
  cv::Mat clipped_image(image, roi);
  cv::Mat inverted_image;
  cv::bitwise_not(clipped_image, inverted_image);

  cv::Mat gray_image;
  cv::cvtColor(inverted_image, gray_image, cv::COLOR_RGB2GRAY);

  std::vector<cv::Point> points;
  cv::Mat_<uchar>::iterator it = gray_image.begin<uchar>();
  cv::Mat_<uchar>::iterator end = gray_image.end<uchar>();
  for (; it != end; ++it)
  {
      if (*it) points.push_back(it.pos());
  }

  cv::RotatedRect bounding_box = cv::minAreaRect(cv::Mat(points));


  cv::Rect bounding_rect;
  bounding_rect.x = bounding_box.center.x - (bounding_box.size.width / 2);
  bounding_rect.y = bounding_box.center.y - (bounding_box.size.height / 2);
  bounding_rect.width = bounding_box.size.width;
  bounding_rect.height = bounding_box.size.height;

  cv::Mat croped_image(inverted_image, bounding_rect);

  cv::Mat croped_image_open;
  /* cv::morphologyEx(croped_image, croped_image_open, cv::MORPH_OPEN, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(2,2))); */

  cv::Mat number_image_int;
  cv::resize(croped_image_open, number_image_int, cv::Size(16,16), CV_INTER_LINEAR);
  cv::Mat number_image;
  number_image_int.convertTo(number_image, CV_32FC1, 1.f/255);

  /* cv::Mat number_image_transposed = number_image.t(); */
  /* arma::mat arma_mat(reinterpret_cast<double*>(number_image_transposed.data), number_image_transposed.rows, number_image_transposed.cols); */

  /* arma::mat arma_mat = cv2arma(number_image); */
  /* cv::FileStorage file("some_name.ext", cv::FileStorage::WRITE); */
  /* file << "hej" << number_image; */
  cv::Mat blurred_image;
  cv::GaussianBlur(number_image, blurred_image, cv::Size(5,5), 1);

  arma::mat arma_mat(number_image.rows, number_image.cols);
  for (int i = 0; i < number_image.rows; ++i) {
    for (int j = 0; j < number_image.cols; ++j) {
      arma_mat(i,j) = blurred_image.at<float>(i,j);
      /* print_line(std::to_string(number_image.at<float>(i,j)).c_str()); */
    }
  }

  arma::vec number_vector = arma::vectorise(arma_mat);
  /* for (int i = 0; i < 16; ++i) { */
  /*   print_line(std::to_string(number_image.at<double>(i)).c_str()); */
  /* } */
  int classification = ClassifyMean(number_vector);

  /* print_line(std::to_string(mean_digits.n_rows).c_str()); */
  /* print_line(std::to_string(mean_digits.n_cols).c_str()); */

  cv::namedWindow( "Display window", cv::WINDOW_AUTOSIZE );
  cv::imshow( "Display window", number_image_int);
  cv::waitKey(0);
    return classification;
  }
  catch ( cv::Exception & e ) {
  print_line(e.msg.c_str());
  return -1;
  }
}

void NumberClassification::_bind_methods() {
  ClassDB::bind_method(D_METHOD("Classify", "img", "rect"), &NumberClassification::Classify);
}

int NumberClassification::ClassifyMean(arma::vec number) {
  int smallest_index = -1;
  float smallest_distance = 1000000;

  for (int i = 0; i < 10; ++i) {
    arma::vec mean_vector(mean_digits.col(i));

    /* print_line(std::to_string(mean_digits.n_rows).c_str()); */
    /* print_line(std::to_string(mean_digits.n_cols).c_str()); */

    /* print_line(std::to_string(mean_vector.n_rows).c_str()); */
    /* print_line(std::to_string(mean_vector.n_cols).c_str()); */
    float distance = arma::norm(mean_vector - number);

    print_line(std::to_string(distance).c_str());

    if (distance < smallest_distance) {
      smallest_distance = distance;
      smallest_index = i;
    }
  }

  return smallest_index;
}

arma::mat NumberClassification::cv2arma(cv::Mat& cv_mat) {
  arma::mat arma_mat(cv_mat.rows, cv_mat.cols);

  for (int i = 0; i < cv_mat.rows; ++i) {
    for (int j = 0; j < cv_mat.cols; ++j) {
      arma_mat(i,j) = cv_mat.at<double>(i,j);
    }
  }

  return arma_mat;
}
