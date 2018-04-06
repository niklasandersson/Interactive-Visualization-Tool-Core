#include "number_classification.h"

NumberClassification::NumberClassification() {

}

int NumberClassification::Classify(const Ref<Image> &p_image, Rect2 rect) {
  Ref<Image> img = p_image->duplicate();
  int width = img->get_width();
  int height = img->get_height();
  print_line(itos(height));
  print_line(itos(width));
  print_line(itos(p_image->get_format()));
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

  cv::Mat number_image;
  cv::resize(croped_image, number_image, cv::Size(16,16), CV_INTER_LINEAR);

  cv::namedWindow( "Display window", cv::WINDOW_AUTOSIZE );
  cv::imshow( "Display window", number_image);
  cv::waitKey(0);
  return 1;
}

void NumberClassification::_bind_methods() {
  ClassDB::bind_method(D_METHOD("Classify", "img", "rect"), &NumberClassification::Classify);
}
